#include "texthandler.h"

textHandler::textHandler()
{

}

bool textHandler::writeFile(QString filename)
{
    QDir dir("./task");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    emit sendFlag("идет запись");
    QFile file(filename);
    QTextStream stream(&file);
    QStringList list = message.split("\n");

    if(file.exists()) // if the file exists then overwrite it
    {
        file.open(QFile::WriteOnly);
        file.write("");
        file.close();
    }

    for(int i = 0; i < list.count(); ++i)
    {
        if(!file.open(QFile::Append))
        {
            emit sendFlag("идет запись(ошибка: " + file.errorString() + "");

            for(int j = 0; j < 30; ++j)
            {
                for(int k = 0; k < list.count(); ++k)
                {
                    file.open(QFile::Append);
                    stream << list.value(k) << "\n";
                    file.close();
                    Sleep(1000);
                }

            }
            emit sendFlag("завершено с ошибкой");
            return false;
        }

        stream << list.value(i) << "\n";

        file.close();
        Sleep(1000);
    }

    return true;
}

// writing data with which the thread will work
void textHandler::getText(QString s, QString a)
{
    message = s;
    number = a;
    filePath = "./task/" + a + ".txt";
}

void textHandler::handler()
{
    writeFile(filePath) ? emit sendFlag("завершено") : emit sendFlag("завершено с ошибкой");
    Sleep(1000);
}

void textHandler::run()
{
    handler();
}
