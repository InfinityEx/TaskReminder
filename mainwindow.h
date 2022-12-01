#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bfh_addquery.h"
#include "bfh_modify.h"
#include "aft_addquery.h"
#include "aft_modify.h"
#include "updateinfo.h"
#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QDate>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QDateEdit>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QApplication>
#include <QStandardItemModel>
#include <QItemSelectionModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //项目事前控制全局变量
    QString bfh_name,bfh_id,bfh_age,bfh_cpnumber,bfh_applycr,bfh_applyproduct,bfh_loopitem,bfh_sxstarttime,bfh_sxendtime,bfh_process,bfh_comments,bfh_projid;
    //完结项目全局变量
    QString aft_name,aft_id,aft_age,aft_cpnumber,aft_applycr,aft_applyproduct,aft_loopitem,aft_sxstarttime,aft_sxendtime,aft_process,aft_comments,aft_projid;

private slots:
    void querynow(); //信号槽连接_立即查询
    void querynow2(); //信号槽连接_立即查询
    void linkinfo(); //连接信息-服务器配置
    void OpenDatabase(); //重新连接数据库并查询
    void on_actiond_triggered(); //主菜单_重新连接
    void on_sl_querybtn_clicked(); //查询功能
    void on_sl_querybtn_2_clicked(); //新增记录
    void on_sl_querybtn_3_clicked(); //修改记录
    void on_sl_querybtn_4_clicked();
    void on_sl_querybtn_5_clicked();

    void on_sl_querybtn_6_clicked();

    void on_sl_querybtn_7_clicked();

    void on_sl_querybtn_8_clicked();

    void on_tableView_1_clicked(const QModelIndex &index);

    void on_tableView_2_clicked(const QModelIndex &index);

    void on_actione_triggered();

    void on_action_2_triggered();

    void on_tabWidget_tabBarDoubleClicked(int index);

    void on_action_triggered();

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;  //MainWindow本体
    bfh_addquery *bfh_aq; //项目状态查询_新增客户信息窗体
    bfh_modify *bfh_mq; //项目状态查询_修改客户信息窗体
    aft_addquery *aft_aq;
    aft_modify *aft_mq;
    updateinfo *udi;
    QSqlDatabase db; //局部变量 定义数据库db

signals:
     void modifyinfolist(); //信号槽_传递数据

};

#endif // MAINWINDOW_H
