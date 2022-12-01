#include "bfh_modify.h"
#include "ui_bfh_modify.h"
#include "mainwindow.h"


bfh_modify::bfh_modify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bfh_modify)
{
    ui->setupUi(this);

}

bfh_modify::~bfh_modify()
{
    delete ui;
}

//1-3 更新窗口数据
void bfh_modify::fromdata()
{
    ui->mod_name->setText(bfhmq_name);
    ui->mod_id->setText(bfhmq_id);
    ui->mod_age->setText(bfhmq_age);
    ui->mod_applycr->setText(bfhmq_applycr);
    ui->mod_applyproduct->setCurrentText(bfhmq_applyproduct);
    ui->mod_cpnumber->setText(bfhmq_cpnumber);
    ui->mod_loopitem->setCurrentText(bfhmq_loopitem);
    ui->mod_sxstarttime->setDateTime(QDateTime::fromString(bfhmq_sxstarttime,"yyyy/MM/dd"));
    ui->mod_sxendtime->setDateTime(QDateTime::fromString(bfhmq_sxendtime,"yyyy/MM/dd"));
    ui->mod_comments->setText(bfhmq_comments);
    ui->mod_process->setCurrentText(bfhmq_process);
    qDebug()<<bfhmq_projid;
}

//1-3 提交更改
void bfh_modify::on_pushButton_clicked()
{
    QString flag="correct";
    qDebug()<<flag;
    ui->pushButton->setEnabled(false);
    QString modct_name=ui->mod_name->text();
    QString modct_id=ui->mod_id->text();
    int modct_age=ui->mod_age->text().toInt();
    QString modct_cpnumber=ui->mod_cpnumber->text();
    QString modct_applycr=ui->mod_applycr->text();
    QString modct_applyproduct=ui->mod_applyproduct->currentText();
    QString modct_loopitem=ui->mod_loopitem->currentText();
    QDate modct_sxstarttime=ui->mod_sxstarttime->date();
    QDate modct_sxendtime=ui->mod_sxendtime->date();
    QString modct_process=ui->mod_process->currentText();
    QString modct_comments=ui->mod_comments->text();
    QString modct_projid=bfhmq_projid;
    //首先检查所有内容是否为空/原始数据
    if(modct_name=="" || modct_id=="" || modct_age==NULL || modct_cpnumber=="" || modct_applycr=="" || modct_applyproduct=="请选择"
            || modct_loopitem=="请选择" || modct_process=="请选择")
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::critical(NULL,"非空信息有留空","请检查是否应填信息有遗漏",QMessageBox::Ok);
    }
    //年龄问题
    if(modct_age<18)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::critical(NULL,"年龄不正确","申请人尚未成年！",QMessageBox::Ok);
    }
    else if(modct_age>60)
    {
        ui->pushButton->setEnabled(true);
        if(flag=="wrong")
        {
            flag="wrong";
        }
        QMessageBox::warning(NULL,"高龄申请人","申请人已超过60岁！请确认相关银行是否同意受理",QMessageBox::Ok);
    }
    //授信时间错误
    if(modct_sxstarttime>modct_sxendtime)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::critical(NULL,"授信时间错误","授信结束时间比授信开始时间早！请检查填写内容！如果授信时间不确定请保持两者一致",QMessageBox::Ok);
    }
    //检查身份证号码是否为18位
    if ((modct_id).toLocal8Bit().length()!=18)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::critical(NULL,"身份证号码/企业信用代码错误","您输入的身份证号码可能不是18位！请检查填写内容！",QMessageBox::Ok);
    }
    //检查手机号码
    if((modct_cpnumber).toLocal8Bit().length()==NULL)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::critical(NULL,"联系方式错误","联系方式不能为空！请检查填写内容！",QMessageBox::Ok);
    }
    else if((modct_cpnumber).toLocal8Bit().length()!=11 && (modct_cpnumber).toLocal8Bit().length()!=NULL)
    {
        ui->pushButton->setEnabled(true);
        if(flag=="wrong")
        {
            flag="wrong";
        }
        QMessageBox::information(NULL,"联系方式不是手机号码","您填写的联系方式可能不是手机号码，请仔细确认！",QMessageBox::Ok);
    }
    //数据验证无误后向服务器提交新增记录
    if(flag=="correct")
    {
        QSqlQuery sql;
        sql.prepare("UPDATE beforehand SET CustomerName=:CustomerName,ID=:ID,age=:age,cpnumber=:cpnumber,applycr=:applycr,applyproduct=:applyproduct"
                    ",loopitem=:loopitem,sxstarttime=:sxstarttime,sxendtime=:sxendtime,process=:process,comments=:comments WHERE projid=:projid");
        sql.bindValue(":CustomerName",modct_name);
        sql.bindValue(":ID",modct_id);
        sql.bindValue(":age",modct_age);
        sql.bindValue(":cpnumber",modct_cpnumber);
        sql.bindValue(":applycr",modct_applycr);
        sql.bindValue(":applyproduct",modct_applyproduct);
        sql.bindValue(":loopitem",modct_loopitem);
        sql.bindValue(":sxstarttime",modct_sxstarttime);
        sql.bindValue(":sxendtime",modct_sxendtime);
        sql.bindValue(":process",modct_process);
        sql.bindValue(":comments",modct_comments);
        sql.bindValue(":projid",bfhmq_projid);
        bool mqsql=sql.exec();
        if(mqsql)
        {
            emit selectquery2();
            qDebug()<<"send off";
            QMessageBox::information(NULL,"数据写入完成","已修改客户 "+modct_name+"("+modct_id+")"+" 的信息",QMessageBox::Ok);
            ui->pushButton->setEnabled(true);
            sql.clear();
            bfh_modify::close();
        }
        else
        {
            ui->pushButton->setEnabled(true);
            QMessageBox::critical(NULL,"连接数据库失败","写入数据到服务器失败！请检查网络",QMessageBox::Ok);
            sql.clear();
        }
    }
}

//自动填充年龄
void bfh_modify::on_mod_id_editingFinished()
{
    QString birthday=ui->mod_id->text().mid(6,8);
    QString birthtemp=birthday.mid(0,4);
    QString nowtime=QDateTime::currentDateTime().toString().right(4);
    int bdate=nowtime.toInt()-birthtemp.toInt();
    ui->mod_age->setText(QString::number(bdate));
}

//项目完结
void bfh_modify::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);
    QString modct_name=ui->mod_name->text();
    QString modct_id=ui->mod_id->text();
    int modct_age=ui->mod_age->text().toInt();
    QString modct_cpnumber=ui->mod_cpnumber->text();
    QString modct_applycr=ui->mod_applycr->text();
    QString modct_applyproduct=ui->mod_applyproduct->currentText();
    QString modct_loopitem=ui->mod_loopitem->currentText();
    QDate modct_sxstarttime=ui->mod_sxstarttime->date();
    QDate modct_sxendtime=ui->mod_sxendtime->date();
    //QString modct_process=ui->mod_process->currentText();
    QString modct_comments=ui->mod_comments->text();
    QString modct_projid=bfhmq_projid;
    int response=QMessageBox::question(this,"项目归入完结","您确定"+modct_name+"("+modct_id+")项目已确认放款并要完结该项目吗？",QMessageBox::Yes,QMessageBox::Cancel);
    if (response==QMessageBox::Yes)
    {
        QSqlQuery sql;
        sql.prepare("INSERT INTO afterwards (CustomerName,ID,age,cpnumber,applycr,applyproduct,loopitem,sxstarttime,sxendtime,process,comments,projid) "
                    "VALUE (:CustomerName,:ID,:age,:cpnumber,:applycr,:applyproduct,:loopitem,:sxstarttime,:sxendtime,:process,:comments,:projid)");
        sql.bindValue(":CustomerName",modct_name);
        sql.bindValue(":ID",modct_id);
        sql.bindValue(":age",modct_age);
        sql.bindValue(":cpnumber",modct_cpnumber);
        sql.bindValue(":applycr",modct_applycr);
        sql.bindValue(":applyproduct",modct_applyproduct);
        sql.bindValue(":loopitem",modct_loopitem);
        sql.bindValue(":sxstarttime",modct_sxstarttime);
        sql.bindValue(":sxendtime",modct_sxendtime);
        sql.bindValue(":process","B类资料归档");
        sql.bindValue(":comments",modct_comments);
        sql.bindValue(":projid",modct_projid);
        if (modct_projid=="")
        {
            QMessageBox::warning(NULL,"数据残缺警告","当前客户没有项目ID！请及时进行后台处理",QMessageBox::Ok);
            ui->pushButton_2->setEnabled(true);
        }
        else
        {
            bool aqsql=sql.exec();
            if(aqsql)
            {
                QMessageBox::information(NULL,"数据写入完成","客户 "+modct_name+"("+modct_id+")"+" 项目完结成功,相关数据已写入数据库",QMessageBox::Ok);
                QSqlQuery dsql;
                dsql.prepare("DELETE FROM beforehand WHERE projid=:projid");
                dsql.bindValue(":projid",modct_projid);
                bool dqsql=dsql.exec();
                if(dqsql)
                {
                    QMessageBox::information(NULL,"数据清理完毕","残留数据已清理",QMessageBox::Ok);
                }
                else
                {
                    QMessageBox::warning(NULL,"数据清理失败","清理指令发送不成功，选中的记录可能仍然在项目状态查询中，请手动删除！",QMessageBox::Ok);
                }
                emit selectquery2();
                ui->pushButton_2->setEnabled(true);
                bfh_modify::close();
            }
            else
            {
                ui->pushButton_2->setEnabled(true);
                QMessageBox::critical(NULL,"连接数据库失败","写入数据到服务器失败！请检查网络",QMessageBox::Ok);
            }
        }
    }
    ui->pushButton_2->setEnabled(true);
}

