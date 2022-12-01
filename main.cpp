#include "mainwindow.h"
#include <QtWidgets>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QApplication>
#include <QStandardItemModel>

//Qt与C标准相同，不在main之前的函数需要预先声明
//bool OpenDatabase();
//void ExecQuery();

//主程序入口
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss("Ubuntu.qss");
           if( qss.open(QFile::ReadOnly))
           {
               qDebug("open success");
               QString style = QLatin1String(qss.readAll());
               a.setStyleSheet(style);
               qss.close();
           }
           else
           {
               qDebug("Open failed");
           }

    QString appPath=QApplication::applicationDirPath();
    appPath+="/hnnd.ico";
    a.setWindowIcon(QIcon(appPath));
    MainWindow w;
    w.show();

    //Program Startup_Ensure DB Valid
    /*
     * qDebug()<<"Available DataBase: ";
    QStringList drivers=QSqlDatabase::drivers();
    foreach(QString driver,drivers)
    {
        qDebug()<<driver;
    }
    QSqlDatabase dbc=QSqlDatabase::addDatabase("QODBC");
    qDebug()<<"odbc valid?"<<dbc.isValid();
    */

    //OpenDatabase();
    //ExecQuery();

    return a.exec();
}

