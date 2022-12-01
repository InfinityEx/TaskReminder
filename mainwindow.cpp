#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bfh_addquery.h"

void OpenDatabase();
void ExecQuery();
void linkinfo();
QString serverip,dbname,dbacc,dbpw;
int serverport;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    linkinfo(); //读取MySQL服务器配置
    ui->uip_ip->setText(serverip);
    ui->uip_port->setText(QString::number(serverport));
    ui->uip_dbname->setText(dbname);
    ui->uip_dbacc->setText(dbacc);
    ui->uip_pw->setText(dbpw);

    //1-2 新增记录通信&信号槽
    MainWindow::bfh_aq=new bfh_addquery;
    connect(bfh_aq,SIGNAL(selectquery()),this,SLOT(querynow()));
    //1-3 修改记录通信&信号槽
    MainWindow::bfh_mq=new bfh_modify;
    connect(this,SIGNAL(modifyinfolist()),bfh_mq,SLOT(fromdata()));
    connect(bfh_mq,SIGNAL(selectquery2()),this,SLOT(querynow()));
    //2-2 完结项目新增记录通信&信号槽
    MainWindow::aft_aq=new aft_addquery;
    connect(aft_aq,SIGNAL(selectquery()),this,SLOT(querynow2()));
    //2-3 修改记录通信&信号槽
    MainWindow::aft_mq=new aft_modify;
    connect(this,SIGNAL(modifyinfolist()),aft_mq,SLOT(fromdata()));
    connect(aft_mq,SIGNAL(selectquery2()),this,SLOT(querynow2()));
    //关于
    MainWindow::udi=new updateinfo;

    //启动MainWindow时连接MySQL服务器
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(serverip);
    db.setPort(serverport);
    db.setDatabaseName(dbname);
    db.setUserName(dbacc);
    db.setPassword(dbpw);

    //判断是否打开了数据库
    if (!db.open())
    {
        qDebug() << "Error: " << db.lastError().text();
        ui->status->setText("当前状态：数据库连接失败！请检查网络及服务器状态");
        QMessageBox::warning(NULL,"数据库连接失败","数据库连接失败！请检查网络及服务器状态",QMessageBox::Ok);
    }
    else
    {
        qDebug()<<"connect sql server successfully!";
        ui->status->setText("当前状态：数据库连接成功");
    }
}

//窗口注销
MainWindow::~MainWindow()
{
    db.close();
    qDebug()<<"Link is take off";
    delete ui;
}

//读取MySQL配置
void MainWindow::linkinfo()
{
    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    QFileInfo file(QCoreApplication::applicationDirPath()+"/option.ini");
    if (file.exists()==false)
    {
        QSettings settings(QCoreApplication::applicationDirPath()+"/option.ini",QSettings::IniFormat);
        settings.beginGroup("MySQL_Option");
        settings.setValue("serverip","192.168.1.1");
        settings.setValue("port",3306);
        settings.setValue("dbname","database");
        settings.setValue("dbacc","sa");
        settings.setValue("dbpw","123456");
        settings.endGroup();
        ui->status->setText("当前状态：MySQL配置已重置");
        QSettings settingsread(QCoreApplication::applicationDirPath()+"/option.ini",QSettings::IniFormat);
        serverip=settingsread.value("MySQL_Option/serverip").toString();
        serverport=settingsread.value("MySQL_Option/port").toInt();
        dbname=settingsread.value("MySQL_Option/dbname").toString();
        dbacc=settingsread.value("MySQL_Option/dbacc").toString();
        dbpw=settingsread.value("MySQL_Option/dbpw").toString();
        ui->status->setText("当前状态：已读取本地MySQL配置");
    }
    else
    {
        QSettings settingsread(QCoreApplication::applicationDirPath()+"/option.ini",QSettings::IniFormat);
        serverip=settingsread.value("MySQL_Option/serverip").toString();
        serverport=settingsread.value("MySQL_Option/port").toInt();
        dbname=settingsread.value("MySQL_Option/dbname").toString();
        dbacc=settingsread.value("MySQL_Option/dbacc").toString();
        dbpw=settingsread.value("MySQL_Option/dbpw").toString();
        ui->status->setText("当前状态：已读取本地MySQL配置");
    }
}

//函数_打开数据库
void MainWindow::OpenDatabase()
{
    serverip=ui->uip_ip->text();
    serverport=ui->uip_port->text().toInt();
    dbname=ui->uip_dbname->text();
    dbacc=ui->uip_dbacc->text();
    dbpw=ui->uip_pw->text();
    qDebug()<<serverip<<serverport<<dbname<<dbacc<<dbpw;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(serverip);
    db.setPort(serverport);
    db.setDatabaseName(dbname);
    db.setUserName(dbacc);
    db.setPassword(dbpw);

    //判断是否打开了数据库
    if (!db.open())
    {
        qDebug() << "Error: " << db.lastError().text();
        ui->status->setText("当前状态：数据库连接失败！请检查网络及服务器状态");
        QMessageBox::warning(NULL,"数据库连接失败","数据库连接失败！请检查网络及服务器状态",QMessageBox::Ok);
    }
    else
    {
        qDebug()<<"connect sql server successfully!";
        ui->status->setText("当前状态：数据库连接成功");
    }
}

//debug_查询
void ExecQuery()
{
    QSqlQuery query;
    QString str;
    qDebug()<<str<<query.exec("select * from yushouli");
    //qDebug()<<str;

    if (query.exec())
    {
        while(query.next())
        {
            //获取每条记录中属性（即列）的个数
                        int columnCount = query.record().count();
                        //获取name属性所在列的编号，列从左向右编号，最左边的编号为0
                        int nameIndex = query.record().indexOf("CustomerName");
                        int idnIndex =query.record().indexOf("ID");
                        //获取id属性的值，并转换为int型
                        int id = query.value(0).toInt();
                        //获取name属性的值
                        QString name = query.value(nameIndex).toString();
                        QString idn = query.value(idnIndex).toString();

                        qDebug() << " id is : " << id
                                 << " name is : " << name
                                 << " ID Card is" << idn
                                 << " columnCount is : " << columnCount;
                        //qDebug()<<str;
        }
    }
}


//0 菜单栏_重新连接服务器
void MainWindow::on_actiond_triggered()
{
    OpenDatabase(); //服务器重连
}

//0 菜单栏_退出
void MainWindow::on_actione_triggered()
{
    MainWindow::close();
}

//0 菜单栏_关于_更新信息
void MainWindow::on_action_triggered()
{
    udi->setWindowModality(Qt::ApplicationModal); //阻塞窗口，避免操作主窗体
    udi->show();
}

//0 菜单栏_版权信息
void MainWindow::on_action_2_triggered()
{
    QMessageBox::information(NULL,"版权信息","author:InfinityEx\nGithub:https://github.com/InfinityEx\nDev in Qt 5.15.2(LGPL)\nMySQL Community Server 5.7.36(GPL)\n程序基于Qt 5.15.2(LGPL协议)开发\n"
                                  "数据库基于MySQL社区版服务器5.7.36(GPL协议)架设",QMessageBox::Ok);
}

//1-1 项目流程状态_查询按钮
QStandardItemModel* model =new QStandardItemModel();
void MainWindow::on_sl_querybtn_clicked()
{
    ui->tableView_1->clearSpans(); //每次刷新都清空后再执行
    //QStandardItemModel* model =new QStandardItemModel();
    QSqlQuery query;
    QString sqlselect;
    QString na="%"+ui->cname->text().trimmed()+"%";
    QString pc=ui->add_process->currentText();
    int status1=0;
    if(ui->cname->text().trimmed()!=NULL){status1=status1+1;}
    if(ui->add_process->currentText()!="请选择"){status1=status1+2;}
    switch (status1)
    {
        case 0:
            sqlselect=query.exec("select * from beforehand;");
            break;
        case 1:
            sqlselect=QString("select * from beforehand where CustomerName like '%1';").arg(na);
            //query.bindValue(":namestring",na);
            //QString jkl=ui->cname->text().trimmed();
            qDebug()<<na<<sqlselect<<ui->cname->text().trimmed();
            query.exec(sqlselect);
            break;
        case 2:
            sqlselect=QString("select * from beforehand where process like '%1';").arg(pc);
            query.exec(sqlselect);
            break;
        case 3:
        sqlselect=QString("select * from beforehand where CustomerName like '%1' and process like '%2';").arg(na).arg(pc);
        query.exec(sqlselect);
        break;
    }

    if(query.exec())
    {
        int row=0,column=0; //初始化行列数量
        int columnCount = query.record().count(); //获得数据库最大列
        int rowCount=query.size(); //获得数据库最大行
        //定义model组件
        //QStandardItemModel* model =new QStandardItemModel();
        //传递行列数量
        model->setColumnCount(columnCount);
        model->setRowCount(rowCount);
        //填充表头各列实际名称
        QStringList bfh_item={"姓名","身份证号码","年龄","联系方式","申报金额(万)","申报产品","循环项目","授信开始时间","授信结束时间","当前流程","备注","项目ID"};
        for(int i=0;i<bfh_item.count();i++)
        {
            //渲染各列标题名称
            model->setHeaderData(i,Qt::Horizontal,bfh_item[i]);
        }
        //渲染model组件
        ui->tableView_1->setModel(model);
        while(query.next())
        {
            //获取name属性所在列的编号，列从左向右编号，最左边的编号为0
            int nameIndex = query.record().indexOf("CustomerName");
            int idnIndex =query.record().indexOf("ID");
            int ageIndex =query.record().indexOf("age");
            int cpnumberIndex =query.record().indexOf("cpnumber");
            int applycrIndex =query.record().indexOf("applycr");
            int applyproductIndex =query.record().indexOf("applyproduct");
            int loopitemIndex =query.record().indexOf("loopitem");
            int sxstarttimeIndex =query.record().indexOf("sxstarttime");
            int sxendtimeIndex =query.record().indexOf("sxendtime");
            int processIndex =query.record().indexOf("process");
            int commentsIndex =query.record().indexOf("comments");
            int projidIndex = query.record().indexOf("projid");
            //获取值
            QString name = query.value(nameIndex).toString();
            QString idn = query.value(idnIndex).toString();
            QString age = query.value(ageIndex).toString();
            QString cpnumber=query.value(cpnumberIndex).toString();
            QString applycr=query.value(applycrIndex).toString();
            QString applyproduct=query.value(applyproductIndex).toString();
            QString loopitem=query.value(loopitemIndex).toString();
            QString sxstarttime=query.value(sxstarttimeIndex).toString();
            QString sxendtime=query.value(sxendtimeIndex).toString();
            QString process=query.value(processIndex).toString();
            QString comments=query.value(commentsIndex).toString();
            QString projid=query.value(projidIndex).toString();
            qDebug()<<"name"<<name<<"id"<<idn<<"age"<<age<<"row"<<rowCount<<"column"<<columnCount;
            QStringList infolist={name,idn,age,cpnumber,applycr,applyproduct,loopitem,sxstarttime,sxendtime,process,comments,projid};
            for(int j=0;j<infolist.count();j++)
            {
                model->setItem(row,column+j,new QStandardItem(infolist[j]));
                model->item(row,column+j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
            //model->setItem(row,column,new QStandardItem(name));
            //model->setItem(row,column+1,new QStandardItem(idn));

            row++;
        }
        ui->tableView_1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_1->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView_1->show();
        qDebug()<<query.exec("select * from beforehand");
        query.clear();
    }
    else
    {
        QMessageBox::warning(NULL,"数据写入失败","写入数据过程异常，请检查服务器连接状态",QMessageBox::Ok);
    }
}


//1-2 预受理流程新增记录UI
void MainWindow::on_sl_querybtn_2_clicked()
{
    bfh_aq->setWindowModality(Qt::ApplicationModal); //阻塞窗口，避免操作主窗体
    bfh_aq->show(); //显示新增记录UI
}

//1-2 信号槽1 预受理_新增记录后立刻刷新
void MainWindow::querynow()
{
    on_sl_querybtn_clicked();
    qDebug()<<"it's over";
}

//1-3 修改选中记录
void MainWindow::on_sl_querybtn_3_clicked()
{
    qDebug()<<ui->tableView_1->currentIndex();  //没有选中的情形，x,y结果均为-1,选中的情形，第一行和第一列的行号、列号均为0
    if(ui->tableView_1->currentIndex().column()==-1)
    {
        qDebug()<<"Select Nothing";
        QMessageBox::critical(NULL,"读取数据失败","当前未选中任何数据！",QMessageBox::Ok);
    }
    //if(ui->tableView_1->currentIndex().column()!=-1)
    else
    {
        int currow=ui->tableView_1->currentIndex().row();
        QStandardItemModel *modela=qobject_cast<QStandardItemModel*>(ui->tableView_1->model());
        QStringList bfh_addlist={bfh_name,bfh_id,bfh_age,bfh_cpnumber,bfh_applycr,bfh_applyproduct,bfh_loopitem,bfh_sxstarttime,bfh_sxendtime,bfh_process};
        qDebug()<<modela->item(currow,7)->text().replace("-","/");
        qDebug()<<modela->item(currow,8)->text().replace("-","/");
        bfh_mq->bfhmq_name=modela->item(currow,0)->text();
        bfh_mq->bfhmq_id=modela->item(currow,1)->text();
        bfh_mq->bfhmq_age=modela->item(currow,2)->text();
        bfh_mq->bfhmq_cpnumber=modela->item(currow,3)->text();
        bfh_mq->bfhmq_applycr=modela->item(currow,4)->text();
        bfh_mq->bfhmq_applyproduct=modela->item(currow,5)->text();
        bfh_mq->bfhmq_loopitem=modela->item(currow,6)->text();
        bfh_mq->bfhmq_sxstarttime=modela->item(currow,7)->text().replace("-","/");
        bfh_mq->bfhmq_sxendtime=modela->item(currow,8)->text().replace("-","/");
        bfh_mq->bfhmq_process=modela->item(currow,9)->text();
        bfh_mq->bfhmq_comments=modela->item(currow,10)->text();
        bfh_mq->bfhmq_projid=modela->item(currow,11)->text();
        bfh_mq->setWindowModality(Qt::ApplicationModal); //阻塞窗口，避免操作主窗体
        bfh_mq->show(); //显示修改记录UI
        emit modifyinfolist();
    }
}

//1-3&2-3 信号1 传递值
void modifyinfolist();

//1-4 项目状态查询_删除当前记录
void MainWindow::on_sl_querybtn_4_clicked()
{
    if(ui->tableView_1->currentIndex().column()!=-1)
    {
        int response=QMessageBox::warning(this,"项目删除警告","您确定要删除此项目吗？\n\n此操作一旦执行将不可撤回！",QMessageBox::Yes,QMessageBox::Cancel);
        if (response==QMessageBox::Yes)
        {
            QStandardItemModel *modela=qobject_cast<QStandardItemModel*>(ui->tableView_1->model());
            int currow=ui->tableView_1->currentIndex().row();
            QString del_pid=modela->item(currow,11)->text();
            QSqlQuery dsql;
            dsql.prepare("DELETE FROM beforehand WHERE projid=:projid");
            dsql.bindValue(":projid",del_pid);
            bool dqsql=dsql.exec();
            if (dqsql)
            {
                QMessageBox::information(NULL,"删除成功","数据已删除",QMessageBox::Ok);
                querynow();
            }
        }
    }
}

//2-1 已完结项目_查询记录
QStandardItemModel* model2 =new QStandardItemModel();
void MainWindow::on_sl_querybtn_5_clicked()
{
    ui->tableView_2->clearSpans(); //每次刷新都清空后再执行
    QSqlQuery query;
    QString sqlselect;
    QString na="%"+ui->cname_2->text().trimmed()+"%";
    QString pc=ui->add_process_2->currentText();
    int status2=0;
    if(ui->cname_2->text().trimmed()!=NULL){status2=status2+1;}
    if(ui->add_process_2->currentText()!="请选择"){status2=status2+2;}
    switch (status2)
    {
        case 0:
            sqlselect=query.exec("select * from afterwards;");
            break;
        case 1:
            sqlselect=QString("select * from afterwards where CustomerName like '%1';").arg(na);
            //query.bindValue(":namestring",na);
            //QString jkl=ui->cname->text().trimmed();
            qDebug()<<na<<sqlselect<<ui->cname->text().trimmed();
            query.exec(sqlselect);
            break;
        case 2:
            sqlselect=QString("select * from afterwards where process like '%1';").arg(pc);
            query.exec(sqlselect);
            break;
        case 3:
            sqlselect=QString("select * from afterwards where CustomerName like '%1' and process like '%2';").arg(na).arg(pc);
            query.exec(sqlselect);
            break;
    }

    if(query.exec())
    {
        int row=0,column=0; //初始化行列数量
        int columnCount = query.record().count(); //获得数据库最大列
        int rowCount=query.size(); //获得数据库最大行
        //定义model组件
        //QStandardItemModel* model =new QStandardItemModel();
        //传递行列数量
        model2->setColumnCount(columnCount);
        model2->setRowCount(rowCount);
        //填充表头各列实际名称
        QStringList bfh_item={"姓名","身份证号码","年龄","联系方式","授信金额(万)","申报产品","循环项目","授信开始时间","授信结束时间","当前流程","备注","项目ID"};
        for(int i=0;i<bfh_item.count();i++)
        {
            //渲染各列标题名称
            model2->setHeaderData(i,Qt::Horizontal,bfh_item[i]);
        }
        //渲染model组件
        ui->tableView_2->setModel(model2);
        while(query.next())
        {
            //获取name属性所在列的编号，列从左向右编号，最左边的编号为0
            int nameIndex = query.record().indexOf("CustomerName");
            int idnIndex =query.record().indexOf("ID");
            int ageIndex =query.record().indexOf("age");
            int cpnumberIndex =query.record().indexOf("cpnumber");
            int applycrIndex =query.record().indexOf("applycr");
            int applyproductIndex =query.record().indexOf("applyproduct");
            int loopitemIndex =query.record().indexOf("loopitem");
            int sxstarttimeIndex =query.record().indexOf("sxstarttime");
            int sxendtimeIndex =query.record().indexOf("sxendtime");
            int processIndex =query.record().indexOf("process");
            int commentsIndex =query.record().indexOf("comments");
            int projidIndex = query.record().indexOf("projid");
            //获取值
            QString name = query.value(nameIndex).toString();
            QString idn = query.value(idnIndex).toString();
            QString age = query.value(ageIndex).toString();
            QString cpnumber=query.value(cpnumberIndex).toString();
            QString applycr=query.value(applycrIndex).toString();
            QString applyproduct=query.value(applyproductIndex).toString();
            QString loopitem=query.value(loopitemIndex).toString();
            QString sxstarttime=query.value(sxstarttimeIndex).toString();
            QString sxendtime=query.value(sxendtimeIndex).toString();
            QString process=query.value(processIndex).toString();
            QString comments=query.value(commentsIndex).toString();
            QString projid=query.value(projidIndex).toString();
            qDebug()<<"name"<<name<<"id"<<idn<<"age"<<age<<"row"<<rowCount<<"column"<<columnCount;
            QStringList infolist={name,idn,age,cpnumber,applycr,applyproduct,loopitem,sxstarttime,sxendtime,process,comments,projid};
            for(int j=0;j<infolist.count();j++)
            {
                model2->setItem(row,column+j,new QStandardItem(infolist[j]));
                model2->item(row,column+j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
            //model->setItem(row,column,new QStandardItem(name));
            //model->setItem(row,column+1,new QStandardItem(idn));

            row++;
        }
        ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView_2->show();
        query.clear();
    }
    else
    {
        QMessageBox::critical(NULL,"数据写入失败","写入数据过程异常，请检查服务器连接状态",QMessageBox::Ok);
    }
}

//2-2 完结项目新增记录UI
void MainWindow::on_sl_querybtn_6_clicked()
{
    aft_aq->setWindowModality(Qt::ApplicationModal); //阻塞窗口，避免操作主窗体
    aft_aq->show(); //显示新增记录UI
}

//2-2 完结项目刷新数据
void MainWindow::querynow2()
{
    on_sl_querybtn_5_clicked();
}

//2-3 完结项目修改记录UI
void MainWindow::on_sl_querybtn_7_clicked()
{
    if(ui->tableView_2->currentIndex().column()==-1)
    {
        QMessageBox::critical(NULL,"读取数据失败","当前未选中任何数据！",QMessageBox::Ok);
    }
    else
    {
        int currow=ui->tableView_2->currentIndex().row();
        QStandardItemModel *modela=qobject_cast<QStandardItemModel*>(ui->tableView_2->model());
        QStringList bfh_addlist={bfh_name,bfh_id,bfh_age,bfh_cpnumber,bfh_applycr,bfh_applyproduct,bfh_loopitem,bfh_sxstarttime,bfh_sxendtime,bfh_process};
        qDebug()<<modela->item(currow,7)->text().replace("-","/");
        qDebug()<<modela->item(currow,8)->text().replace("-","/");
        aft_mq->aftmq_name=modela->item(currow,0)->text();
        aft_mq->aftmq_id=modela->item(currow,1)->text();
        aft_mq->aftmq_age=modela->item(currow,2)->text();
        aft_mq->aftmq_cpnumber=modela->item(currow,3)->text();
        aft_mq->aftmq_applycr=modela->item(currow,4)->text();
        aft_mq->aftmq_applyproduct=modela->item(currow,5)->text();
        aft_mq->aftmq_loopitem=modela->item(currow,6)->text();
        aft_mq->aftmq_sxstarttime=modela->item(currow,7)->text().replace("-","/");
        aft_mq->aftmq_sxendtime=modela->item(currow,8)->text().replace("-","/");
        aft_mq->aftmq_process=modela->item(currow,9)->text();
        aft_mq->aftmq_comments=modela->item(currow,10)->text();
        aft_mq->aftmq_projid=modela->item(currow,11)->text();
        if(modela->item(currow,9)->text()=="项目终结")
        {
            QMessageBox::critical(NULL,"不允许修改的项目","此项目已终结！不允许再修改",QMessageBox::Ok);
        }
        else
        {
            aft_mq->setWindowModality(Qt::ApplicationModal); //阻塞窗口，避免操作主窗体
            aft_mq->show(); //显示修改记录UI
            emit modifyinfolist();
        }
    }
}

//2-4 完结项目删除
void MainWindow::on_sl_querybtn_8_clicked()
{
    if(ui->tableView_2->currentIndex().column()!=-1)
    {
        int response=QMessageBox::warning(this,"项目删除警告","您确定要删除此项目吗？\n\n此操作一旦执行将不可撤回！",QMessageBox::Yes,QMessageBox::Cancel);
        if (response==QMessageBox::Yes)
        {
            QStandardItemModel *modela=qobject_cast<QStandardItemModel*>(ui->tableView_2->model());
            int currow=ui->tableView_2->currentIndex().row();
            QString del_pid=modela->item(currow,11)->text();
            QSqlQuery dsql;
            dsql.prepare("DELETE FROM afterwards WHERE projid=:projid");
            dsql.bindValue(":projid",del_pid);
            bool dqsql=dsql.exec();
            if (dqsql)
            {
                QMessageBox::information(NULL,"删除成功","数据已删除",QMessageBox::Ok);
                querynow2();
            }
        }
    }
}

//显示选中数据_项目状态查询
void MainWindow::on_tableView_1_clicked(const QModelIndex &index)
{
    //qDebug()<<index.row();
    ui->ncuinfo->clear();
    int tbrow=index.row();
    QStandardItemModel *modela=qobject_cast<QStandardItemModel*>(ui->tableView_1->model());
    QString cun=modela->item(tbrow,0)->text();
    QString cid=modela->item(tbrow,1)->text();
    QString cage=modela->item(tbrow,2)->text();
    QString cnumb=modela->item(tbrow,3)->text();
    QString ccr=modela->item(tbrow,4)->text();
    QString cprd=modela->item(tbrow,5)->text();
    QString clop=modela->item(tbrow,6)->text();
    QString cdate1=modela->item(tbrow,7)->text();
    QString cdate2=modela->item(tbrow,8)->text();
    QString cpoc=modela->item(tbrow,9)->text();
    QString ccomt=modela->item(tbrow,10)->text();
    ui->ncuinfo->setPlainText("姓名："+cun+"    "+"身份证号："+cid+"    "+"年龄："+cage+"    "+"申报金额："+ccr+"万元"+"\n\n"+"联系方式："+cnumb+"    "+"申报产品："+cprd+"    "+
                              "项目是否循环："+clop+"    "+"授信时间："+cdate1+" 至 "+cdate2+"\n\n"+"项目当前流程："+cpoc+"    "+"备注："+ccomt);
}


//显示选中数据_已完结项目查询
void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    ui->ccuinfo->clear();
    int tbrow=index.row();
    QStandardItemModel *model2=qobject_cast<QStandardItemModel*>(ui->tableView_2->model());
    QString cun=model2->item(tbrow,0)->text();
    QString cid=model2->item(tbrow,1)->text();
    QString cage=model2->item(tbrow,2)->text();
    QString cnumb=model2->item(tbrow,3)->text();
    QString ccr=model2->item(tbrow,4)->text();
    QString cprd=model2->item(tbrow,5)->text();
    QString clop=model2->item(tbrow,6)->text();
    QString cdate1=model2->item(tbrow,7)->text();
    QString cdate2=model2->item(tbrow,8)->text();
    QString cpoc=model2->item(tbrow,9)->text();
    QString ccomt=model2->item(tbrow,10)->text();
    ui->ccuinfo->setPlainText("姓名："+cun+"    "+"身份证号："+cid+"    "+"年龄："+cage+"    "+"授信金额："+ccr+"万元"+"\n\n"+"联系方式："+cnumb+"    "+"申报产品："+cprd+"    "+
                              "项目是否循环："+clop+"    "+"授信时间："+cdate1+" 至 "+cdate2+"\n\n"+"项目当前流程："+cpoc+"    "+"备注："+ccomt);
}

//标签隐藏命令_清空搜索条件
void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    if(index==1)
    {
        ui->cname->clear();
        ui->add_process->setCurrentIndex(0);
        querynow();
    }
    if(index==2)
    {
        ui->cname_2->clear();
        ui->add_process_2->setCurrentIndex(0);
        querynow2();
    }
}

//统计数据
QStandardItemModel *modelx=new QStandardItemModel();
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index==3)
    {
        ui->tableView_X->clearSpans();
        QSqlQuery xquery;
        QString xlselect;
        xlselect=QString("SELECT sum(applycr) from afterwards WHERE process!='项目终结'");
        bool exc1=xquery.exec(xlselect);
        if(exc1==true)
        {
            xquery.next();
            //qDebug()<<xquery.value("allcr").toInt();
            ui->zbcr->setText(xquery.value(0).toString()+"万");
        }
        xlselect=QString("SELECT count(age) from afterwards WHERE process!='项目终结'");
        bool exc2=xquery.exec(xlselect);
        if(exc2==true)
        {
            xquery.next();
            ui->zbcc->setText(xquery.value(0).toString());
        }
        modelx->setColumnCount(7);
        modelx->setRowCount(2);
        ui->tableView_X->horizontalHeader()->setVisible(false);
        ui->tableView_X->verticalHeader()->setVisible(false);
        QStringList rel_prod={"惠农担-粮食贷","惠农担-生猪贷","惠农担-油茶贷","惠农担-茶叶贷","惠农担-农机贴息贷","惠农担-特色贷"};
        QStringList rel_filter={"客户数","当前在保额(万)"};
        for(int x=0;x<rel_prod.count();x++)
        {
            modelx->setItem(0,x+1,new QStandardItem(rel_prod[x]));
        }
        for(int y=0;y<rel_filter.count();y++)
        {
            modelx->setItem(y+1,0,new QStandardItem(rel_filter[y]));
        }
        QStringList rslx={"rsla","rslb","rslc","rsld","rsle","rslf"};
        QStringList rsly={"rsl1","rsl2","rsl3","rsl4","rsl5","rsl6"};
        xlselect=QString("SELECT "
                         "(SELECT COUNT(age) FROM afterwards WHERE applyproduct='惠农担-粮食贷' AND process!='项目终结') AS rsla,"
                         "(SELECT COUNT(age) FROM afterwards WHERE applyproduct='惠农担-生猪贷' AND process!='项目终结') AS rslb,"
                         "(SELECT COUNT(age) FROM afterwards WHERE applyproduct='惠农担-油茶贷' AND process!='项目终结') AS rslc,"
                         "(SELECT COUNT(age) FROM afterwards WHERE applyproduct='惠农担-茶叶贷' AND process!='项目终结') AS rsld,"
                         "(SELECT COUNT(age) FROM afterwards WHERE applyproduct='惠农担-农机贴息贷' AND process!='项目终结') AS rsle,"
                         "(SELECT COUNT(age) FROM afterwards WHERE applyproduct='惠农担-特色贷' AND process!='项目终结') AS rslf FROM afterwards");
        bool exc3=xquery.exec(xlselect);
        if(exc3==true)
        {
            xquery.next();
            for(int c=0;c<rslx.count();c++)
            {
                modelx->setItem(1,c+1,new QStandardItem(xquery.value(c).toString()));
            }
        }
        xlselect=QString("SELECT "
                         "(SELECT SUM(applycr) FROM afterwards WHERE applyproduct='惠农担-粮食贷' AND process!='项目终结') AS rsl1,"
                         "(SELECT SUM(applycr) FROM afterwards WHERE applyproduct='惠农担-生猪贷' AND process!='项目终结') AS rsl2,"
                         "(SELECT SUM(applycr) FROM afterwards WHERE applyproduct='惠农担-油茶贷' AND process!='项目终结') AS rsl3,"
                         "(SELECT SUM(applycr) FROM afterwards WHERE applyproduct='惠农担-茶叶贷' AND process!='项目终结') AS rsl4,"
                         "(SELECT SUM(applycr) FROM afterwards WHERE applyproduct='惠农担-农机贴息贷' AND process!='项目终结') AS rsl5,"
                         "(SELECT SUM(applycr) FROM afterwards WHERE applyproduct='惠农担-特色贷' AND process!='项目终结') AS rsl6 FROM afterwards");
        bool exc4=xquery.exec(xlselect);
        if(exc4==true)
        {
            xquery.next();
            for(int d=0;d<rsly.count();d++)
            {
                modelx->setItem(2,d+1,new QStandardItem(xquery.value(d).toString()));
            }
        }
        for(int e=0;e<7;e++)
        {
                if(e!=0)
                {
                    modelx->item(0,e)->setTextAlignment(Qt::AlignCenter);
                }
                modelx->item(1,e)->setTextAlignment(Qt::AlignCenter);
                modelx->item(2,e)->setTextAlignment(Qt::AlignCenter);
                qDebug()<<"OK W6";
        }
        ui->tableView_X->setModel(modelx);
        ui->tableView_X->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_X->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView_X->setEditTriggers(QAbstractItemView::NoEditTriggers);
        xquery.clear();
    }
}

