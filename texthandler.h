#ifndef TEXTHANDLER_H
#define TEXTHANDLER_H

#include <QObject>
#include <windows.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

class textHandler : public QObject
{
    Q_OBJECT
public:
    textHandler();

private slots:
    bool writeFile(QString filename);

public slots:
    void getText(QString s, QString a);
    void handler();
    void run();

signals:
    void sendFlag(QString);

private:
    QString message;
    QString number;
    QString filePath;
    bool flag = false;

};

#endif // TEXTHANDLER_H
