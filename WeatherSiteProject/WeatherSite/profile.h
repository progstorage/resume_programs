#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>

const QString path_linux = "../ProjectDB.db3";

namespace Ui {
class profile;
}

class profile : public QWidget
{
    Q_OBJECT

public:
    explicit profile(QWidget *parent = nullptr);
    ~profile();

private:
    Ui::profile *ui;

signals:
    void sendSignalExit();

public slots:
    void recieveData_4(QString str);

private slots:
    void on_accept_2_clicked();
    void on_cancel_2_clicked();
    void on_cancel_clicked();
    void on_accept_clicked();
    void on_exit_clicked();
    void on_close_clicked();
};

#endif // PROFILE_H
