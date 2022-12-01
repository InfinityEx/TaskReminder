#ifndef AFT_MODIFY_H
#define AFT_MODIFY_H

#include <QWidget>

namespace Ui {
class aft_modify;
}

class aft_modify : public QWidget
{
    Q_OBJECT

public:
    explicit aft_modify(QWidget *parent = nullptr);
    ~aft_modify();
    QString aftmq_name,aftmq_id,aftmq_age,aftmq_cpnumber,aftmq_applycr,aftmq_applyproduct,aftmq_loopitem,aftmq_sxstarttime,aftmq_sxendtime,aftmq_process,aftmq_comments,aftmq_projid;

private:
    Ui::aft_modify *ui;

private slots:
    void fromdata();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_mod_id_editingFinished();

    void on_pushButton_3_clicked();

signals:
    void selectquery2();
};

#endif // AFT_MODIFY_H
