#ifndef CHOOSETOWN_H
#define CHOOSETOWN_H

#include <QWidget>

namespace Ui {
class choosetown;
}

class choosetown : public QWidget
{
    Q_OBJECT

public:
    explicit choosetown(QWidget *parent = nullptr);
    ~choosetown();

private:
    Ui::choosetown *ui;

signals:
    void sendData(QString str);

private slots:
    void onButtonSend();
    void on_moscow_clicked();
    void on_vladimir_clicked();
    void on_voronej_clicked();
    void on_piter_clicked();
    void on_cancel_clicked();
    void on_arhang_clicked();
    void on_pskov_clicked();
    void on_krim_clicked();
    void on_rostov_clicked();
    void on_krasnodar_clicked();
    void on_volgograd_clicked();
};

#endif // CHOOSETOWN_H
