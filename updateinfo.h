#ifndef UPDATEINFO_H
#define UPDATEINFO_H

#include <QWidget>

namespace Ui {
class updateinfo;
}

class updateinfo : public QWidget
{
    Q_OBJECT

public:
    explicit updateinfo(QWidget *parent = nullptr);
    ~updateinfo();

private:
    Ui::updateinfo *ui;
};

#endif // UPDATEINFO_H
