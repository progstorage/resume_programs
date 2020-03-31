#ifndef AUTH_USER_H
#define AUTH_USER_H

#include <QWidget>

namespace Ui {
class auth_user;
}

class auth_user : public QWidget
{
    Q_OBJECT

public:
    explicit auth_user(QWidget *parent = nullptr);
    ~auth_user();

private:
    Ui::auth_user *ui;

signals:
    void sendData(QString str);

private slots:
    void onButtonSend();
    void on_cancel_clicked();
};

#endif // AUTH_USER_H
