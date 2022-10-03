#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog();
    dialog->setModal(true);

    connect(dialog, &Dialog::signal, this, &MainWindow::slot);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./task_state.db");
    db.open() ? qDebug("open") : qDebug("no open");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE tasks(taskNumber INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, dateTime TEXT, dataSize INT, taskState TEXT, message TEXT);");

    model = new QSqlTableModel(this, db);
    model->setTable("tasks");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    th = new QThread();
    handler = new textHandler();

    connect(this, &MainWindow::sendText, handler, &textHandler::getText);
    connect(this, &MainWindow::handlerRun, handler, &textHandler::run);
    connect(handler, &textHandler::sendFlag, this, &MainWindow::getFlag);
    connect(th, &QThread::started, handler, &textHandler::handler);

    sendTextToThread();
    handler->moveToThread(th);
    th->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    dialog->show();
}

void MainWindow::getFlag(QString s)
{
    command = "UPDATE tasks SET taskState = '" + s + "' WHERE (taskNumber == " + line_id + ");";
    query->exec(command);

    model->setTable("tasks");       //
    model->select();                // update tableView
    ui->tableView->setModel(model); //

    if(s == "завершено" || s == "завершено с ошибкой")
    {
        command = "SELECT * FROM tasks WHERE (taskState == 'в очереди');";
        query->exec(command);
        query->first();
        line_id = query->value(0).toString();
        message = query->value(4).toString();

        emit sendText(message, line_id);
        emit handlerRun();
    }
}

// add task
void MainWindow::slot(QString s)
{
    QString tmp_size = QString::number(s.size());
    cdt = QDateTime::currentDateTime();
    QString tmp_date = cdt.toString();


    command = "INSERT INTO tasks(dateTime, dataSize, taskState, message) VALUES('" + tmp_date + "', " + tmp_size + ", 'в очереди', '" + s + "');";
    query->exec(command);

    model->setTable("tasks");
    model->select();
    ui->tableView->setModel(model);

}

// remove task
void MainWindow::on_pushButton_3_clicked()
{
    model->removeRow(row);

    model->setTable("tasks");
    model->select();
    ui->tableView->setModel(model);
}

// data from the selected row is copied into a structure to repeat the task
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
    line_id = model->data(model->index(ui->tableView->currentIndex().row(), 0)).toString();

    command = "SELECT * FROM tasks WHERE(taskNumber == " + line_id + ")";
    query->exec(command);
    query->next();

    values.number = query->value("taskNumber").toString();
    values.date = query->value("dateTime").toString();
    values.size = query->value("dataSize").toString();
    values.state = query->value("taskState").toString();
    values.message = query->value("message").toString();
}

// repeat the task
void MainWindow::on_pushButton_2_clicked()
{
    cdt = QDateTime::currentDateTime();
    QString tmp_date = cdt.toString();

    command = "INSERT INTO tasks(dateTime, dataSize, taskState, message) "
                      "VALUES('" + tmp_date + "', " + values.size + ", 'в очереди', '" + values.message + "');";
    query->exec(command);

    model->setTable("tasks");
    model->select();
    ui->tableView->setModel(model);

}

// message is written to a file, and id is needed for the file name
void MainWindow::sendTextToThread()
{

    command = "SELECT * FROM tasks WHERE (taskState == 'в очереди');";
    query->exec(command);
    query->first();
    line_id = query->value(0).toString();
    message = query->value(4).toString();

    emit sendText(message, line_id);
}

