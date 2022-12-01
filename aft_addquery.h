#ifndef AFT_ADDQUERY_H
#define AFT_ADDQUERY_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

namespace Ui {
class aft_addquery;
}

class aft_addquery : public QWidget
{
    Q_OBJECT

public:
    explicit aft_addquery(QWidget *parent = nullptr);
    ~aft_addquery();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_add_id_editingFinished();

    void on_add_process_currentIndexChanged(int index);


private:
    Ui::aft_addquery *ui;

signals:
    void selectquery();
};

#endif // AFT_ADDQUERY_H
