#ifndef BFH_MODIFY_H
#define BFH_MODIFY_H

#include <QWidget>

namespace Ui {
class bfh_modify;
}

class bfh_modify : public QWidget
{
    Q_OBJECT

public:
    explicit bfh_modify(QWidget *parent = nullptr);
    ~bfh_modify();
    QString bfhmq_name,bfhmq_id,bfhmq_age,bfhmq_cpnumber,bfhmq_applycr,bfhmq_applyproduct,bfhmq_loopitem,bfhmq_sxstarttime,bfhmq_sxendtime,bfhmq_process,bfhmq_comments,bfhmq_projid;

private slots:
    void fromdata();
    void on_pushButton_clicked();

    void on_mod_id_editingFinished();

    void on_pushButton_2_clicked();

private:
    Ui::bfh_modify *ui;

signals:
    void selectquery2();
};

#endif // BFH_MODIFY_H
