#ifndef BFH_ADDQUERY_H
#define BFH_ADDQUERY_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

namespace Ui {
class bfh_addquery;
}

class bfh_addquery : public QWidget
{
    Q_OBJECT

public:
    explicit bfh_addquery(QWidget *parent = nullptr);
    ~bfh_addquery();

private slots:
    bool isDigit(QString src);
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_add_id_editingFinished();

    void on_add_applycr_editingFinished();

    void on_add_process_currentIndexChanged(int index);

private:
    Ui::bfh_addquery *ui;


signals:
    void selectquery();

};
#endif // BFH_ADDQUERY_H
