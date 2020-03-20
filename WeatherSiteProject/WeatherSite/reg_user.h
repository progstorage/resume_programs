#ifndef REG_USER_H
#define REG_USER_H

#include <QWidget>

namespace Ui {
class reg_user;
}

class reg_user : public QWidget
{
    Q_OBJECT

public:
    explicit reg_user(QWidget *parent = nullptr);
    ~reg_user();

private:
    Ui::reg_user *ui;

signals:
    void sendData(QString str);

private slots:
    void onButtonSend();
    void on_cancel_clicked();
};

#endif // REG_USER_H
