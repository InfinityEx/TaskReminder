#include "updateinfo.h"
#include "ui_updateinfo.h"
#include <QFileInfo>
#include <QFile>

updateinfo::updateinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateinfo)
{
    ui->setupUi(this);

    ui->textEdit->clear();
        QFileInfo updatefile(QCoreApplication::applicationDirPath()+"/update.txt");
        QString udtxt=QCoreApplication::applicationDirPath()+"/update.txt";
        if(updatefile.exists()==true)
        {
            QFile udfile(udtxt);
            udfile.open(QIODevice::ReadOnly|QIODevice::Text);
            ui->textEdit->setPlainText(udfile.readAll());
            //updatefile.~QFileInfo();
            udtxt.clear();
            udfile.close();
        }
        else
        {
            ui->textEdit->setPlainText("更新文件已丢失");
        }
}

updateinfo::~updateinfo()
{
    delete ui;
}
