#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QMainWindow>
#include "choosetown.h"
#include "auth_user.h"
#include "reg_user.h"
#include "profile.h"
#include "about.h"


namespace Ui {
class mainpage;
};

class mainpage : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainpage(QWidget *parent = nullptr);
    ~mainpage();
    void universal(QString);

    QJsonDocument doc;
    QJsonObject w_tmp;
    QJsonParseError docEr;

    QNetworkAccessManager* netwMan;
    QNetworkAccessManager* netwMan_2;

    QNetworkRequest request;
    QNetworkRequest request_next;

private:
    Ui::mainpage *ui;
    choosetown *window;
    auth_user *window_2;
    reg_user *window_3;
    profile *window_4;
    about *window_5;

signals:
    void sendData(QString str);

public slots:
   void recieveData(QString str);
   void recieveData_2(QString str);
   void recieveData_3(QString str);
   void recieveData_exit_profile();

private slots:
   void on_yesterday_clicked();
   void on_clean_clicked();
   void on_today_clicked();
   void on_tomorrow_clicked();
   void onButtonSend_4();
   void on_predict_clicked();
   void on_refresh_clicked();

   void slot_netwMan(QNetworkReply*);
   void slot_netwMan_2(QNetworkReply*);


};

#endif // MAINPAGE_H
