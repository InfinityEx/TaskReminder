#include "bfh_addquery.h"
#include "ui_bfh_addquery.h"
#include "mainwindow.h"
#include "signalist.h"


bfh_addquery::bfh_addquery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bfh_addquery)
{
    ui->setupUi(this);

    ui->add_sxstarttime->setDateTime(QDateTime::currentDateTime());
    ui->add_sxendtime->setDateTime(QDateTime::currentDateTime());
}

bfh_addquery::~bfh_addquery()
{
    delete ui;
}

bool bfh_addquery::isDigit(QString src)
{
    /*char *s=src.toUtf8().data();
    while(*s && *s>='0' && *s<='9')s++;
    return !bool(*s);*/
    bool ok;
    double val=src.toDouble(&ok);
    src=QString::number(val);
    val=NULL;
    return ok;
}

//新增客户信息
void bfh_addquery::on_pushButton_clicked()
{
    QString flag="correct";
    qDebug()<<flag;
    ui->pushButton->setEnabled(false);
    QString newct_name=ui->add_name->text();
    QString newct_id=ui->add_id->text();
    int newct_age=ui->add_age->text().toInt();
    QString newct_cpnumber=ui->add_cpnumber->text();
    QString newct_applycr=ui->add_applycr->text();
    QString newct_applyproduct=ui->add_applyproduct->currentText();
    QString newct_loopitem=ui->add_loopitem->currentText();
    QDate newct_sxstarttime=ui->add_sxstarttime->date();
    QDate newct_sxendtime=ui->add_sxendtime->date();
    QString newct_comments=ui->add_comments->text();
    QString newct_process=ui->add_process->currentText();
    //首先检查所有内容是否为空/原始数据
    if(newct_name=="" || newct_id=="" || newct_age==NULL || newct_cpnumber=="" || newct_applycr=="" || newct_applyproduct=="请选择"
            || newct_loopitem=="请选择" || newct_process=="请选择")
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::warning(NULL,"非空信息有留空","请检查是否应填信息有遗漏",QMessageBox::Ok);
    }
    //授信金额检查
    if(isDigit(ui->add_applycr->text())==false)
    {
        ui->pushButton->setEnabled(true);
        QMessageBox::critical(NULL,"填写不正确","您填写申请金额的不是数字！",QMessageBox::Ok);
        flag="wrong";
    }
    //年龄问题
    if(newct_age<18)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::warning(NULL,"年龄不正确","申请人尚未成年！",QMessageBox::Ok);
    }
    else if(newct_age>60)
    {
        ui->pushButton->setEnabled(true);
        if(flag=="wrong")
        {
            flag="wrong";
        }
        QMessageBox::warning(NULL,"高龄申请人","申请人已超过60岁！请确认相关银行是否同意受理",QMessageBox::Ok);
    }
    //授信时间错误
    if(newct_sxstarttime>newct_sxendtime)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::warning(NULL,"授信时间错误","授信结束时间比授信开始时间早！请检查填写内容！如果授信时间不确定请保持两者一致",QMessageBox::Ok);
    }
    //检查身份证号码是否为18位
    if ((newct_id).toLocal8Bit().length()!=18)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::warning(NULL,"身份证号码/企业信用代码错误","您输入的身份证号码可能不是18位！请检查填写内容！",QMessageBox::Ok);
    }
    //检查手机号码
    if((newct_cpnumber).toLocal8Bit().length()==NULL)
    {
        ui->pushButton->setEnabled(true);
        flag="wrong";
        QMessageBox::warning(NULL,"联系方式错误","联系方式不能为空！请检查填写内容！",QMessageBox::Ok);
    }
    else if((newct_cpnumber).toLocal8Bit().length()!=11 && (newct_cpnumber).toLocal8Bit().length()!=NULL)
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
        //生成时间戳
        QString nowtime1=QDateTime::currentDateTime().date().toString("yyyyMMdd").right(8);
        qsrand(QTime(0,0,0,0).secsTo(QTime::currentTime()));
        QString addr=QString::number(qrand()+1);
        QString pid=nowtime1+addr;

        QSqlQuery sql;
        sql.prepare("INSERT INTO beforehand (CustomerName,ID,age,cpnumber,applycr,applyproduct,loopitem,sxstarttime,sxendtime,process,comments,projid) "
                    "VALUE (:CustomerName,:ID,:age,:cpnumber,:applycr,:applyproduct,:loopitem,:sxstarttime,:sxendtime,:process,:comments,:projid)");
        sql.bindValue(":CustomerName",newct_name);
        sql.bindValue(":ID",newct_id);
        sql.bindValue(":age",newct_age);
        sql.bindValue(":cpnumber",newct_cpnumber);
        sql.bindValue(":applycr",newct_applycr);
        sql.bindValue(":applyproduct",newct_applyproduct);
        sql.bindValue(":loopitem",newct_loopitem);
        sql.bindValue(":sxstarttime",newct_sxstarttime);
        sql.bindValue(":sxendtime",newct_sxendtime);
        sql.bindValue(":process",newct_process);
        sql.bindValue(":comments",newct_comments);
        sql.bindValue(":projid",pid);
        bool aqsql=sql.exec();
        if(aqsql)
        {
            emit selectquery();
            qDebug()<<"send off";
            on_pushButton_2_clicked(); //成功后清空内容
            QMessageBox::information(NULL,"数据写入完成","已新增客户 "+newct_name+"("+newct_id+")"+" 的信息",QMessageBox::Ok);
            ui->pushButton->setEnabled(true);
            bfh_addquery::close();
        }
        else
        {
            ui->pushButton->setEnabled(true);
            QMessageBox::warning(NULL,"连接数据库失败","写入数据到服务器失败！请检查网络",QMessageBox::Ok);
        }
    }
}

//清空内容
void bfh_addquery::on_pushButton_2_clicked()
{
    ui->add_name->clear();
    ui->add_id->clear();
    ui->add_age->clear();
    ui->add_applycr->clear();
    ui->add_applyproduct->setCurrentText("请选择");
    ui->add_cpnumber->clear();
    ui->add_loopitem->setCurrentText("请选择");
    ui->add_sxstarttime->setDateTime(QDateTime::currentDateTime());
    ui->add_sxendtime->setDateTime(QDateTime::currentDateTime());
    ui->add_comments->clear();
    ui->add_process->setCurrentText("请选择");
}

//自动填充年龄
void bfh_addquery::on_add_id_editingFinished()
{
    QString birthday=ui->add_id->text().mid(6,8);
    //QString birthtemp=birthday.mid(0,4)+"/"+birthday.mid(4,2)+"/"+birthday.mid(6,2);
    QString birthtemp=birthday.mid(0,4);
    QString nowtime=QDateTime::currentDateTime().toString().right(4);
    //QDateTime birthdate=QDateTime::fromString(birthtemp);
    //QDateTime nowtime=QDateTime::currentDateTime();
    int bdate=nowtime.toInt()-birthtemp.toInt();
    ui->add_age->setText(QString::number(bdate));
    qDebug()<<QDateTime::currentDateTime()<<nowtime<<ui->add_age->text();
}

//申请金额填写检查
void bfh_addquery::on_add_applycr_editingFinished()
{
    bool ok;
    QString src=ui->add_applycr->text();
    double valu=src.toDouble(&ok);
    if(valu==false)
    {
        ui->add_applycr->setText("");
        QMessageBox::critical(NULL,"填写不正确","您填写申请金额的不是数字！",QMessageBox::Ok);
    }
}

//项目流程关注事项提醒
void bfh_addquery::on_add_process_currentIndexChanged(int index)
{
    switch (index) {
        case 0:
            break;
        case 1:
            QMessageBox::information(NULL,"预受理注意事项","    请注意询问客户资产、名下公司、家庭情况、经营规模、收益、流水、负债等情况\n    养殖类项目请重点关注客户规模，生猪养殖及其他较大规模养殖请与客户确认"
            "是否有动物防疫条件许可证，涉及食品加工的请确认是否有食品安全许可\n\n非新立项目请忽略该提示",QMessageBox::Ok);
            break;
        case 2:
            //QMessageBox::information(NULL,"调查走访注意事项","",QMessageBox::Ok);
            break;
        case 3:
            QMessageBox::information(NULL,"评审流程注意事项","    请重点关注项目必要材料(如身份证、户口本、结婚证/离婚证、夫妻双方征信报告、名下涉农公司企业征信、营业执照等经营许可要件)\n    客户名下有涉农的自然人投资"
            "/法人投资公司的，按照公司要求需要纳入反担保的，需及时与客户确认是否可出具股东会决议，合作社的需确认是否可出具合作社成员大会决议，由合作机构的还需有合作机构的推荐函\n\n非新立项目请忽略该提示",QMessageBox::Ok);
            break;
        case 4:
            QMessageBox::information(NULL,"合同签订注意事项","    申请人为微农贷项目的，合同签订均在合同管理菜单中无需二次生成；申请人为惠农担项目的，需单独生成纸质合同，涉及担保人需网签的，在项目评审通过后联系分公司"
            "胡堃慧提交OA申请，签订流程与微农贷网签步骤一致\n\n    银行方面合同需注意我司各信息是否正确，且保证合同上是否有双方合作条款，银行未打印合作条款的需单独贴条并双方签章后存档，各大银行个别注意事项如下：\n\n"
            "    华融湘江银行：需检查自助额度是否为0；保证合同其他条款是否按要求注明了主合同编号\n    长沙银行：需确认银行是否出具借款合同；保证合同是否为最高额保证合同",QMessageBox::Ok);
            break;
        default:
            break;
    }
}

