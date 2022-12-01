#include "aupdate.h"
#include "ui_about.h"
#include <QFileInfo>
#include <QFile>

aupdate::aupdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aupdate)
{
    ui->setupUi(this);

    ui->textEdit->clear();
    QFileInfo updatefile(QCoreApplication::applicationDirPath()+"update.dat");
    QString udtxt=QCoreApplication::applicationDirPath()+"update.dat";
    if(updatefile.exists()==true)
    {
        QFile udfile(udtxt);
        udfile.open(QIODevice::ReadOnly|QIODevice::Text);
        ui->textEdit->setPlainText(udfile.readAll());
        udfile.close();
    }
    else
    {
        ui->textEdit->setPlainText("更新文件已丢失");
    }
}

aupdate::~aupdate()
{
    delete ui;
}
