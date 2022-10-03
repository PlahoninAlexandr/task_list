#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include "dialog.h"
#include "texthandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void sendTextToThread();

public slots:
    void slot(QString s);
    void getFlag(QString);

signals:
    void sendText(QString, QString);
    void handlerRun();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    Dialog *dialog;
    QThread *th;
    textHandler *handler;

    int row;
    QString line_id;
    QString message;
    QString command;
    int key_value;
    QDateTime cdt;
    //QMap <int,QString> map;
    //QMap <int, QString>::Iterator it;

    struct Values{
        QString number;
        QString date;
        QString size;
        QString state;
        QString message;
    };

    Values values;
};
#endif // MAINWINDOW_H
