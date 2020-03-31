#include "mainpage.h"
#include "ui_mainpage.h"
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include "information.h"
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsView>
#include <QPalette>

static _user tmp(-1, "", "", "");
const QString cur_date = QDate::currentDate().toString("yyyy.MM.dd");

mainpage::mainpage(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::mainpage)
{
    ui->setupUi(this);
    ui->profile->hide();
    ui->label_2->hide();
    ui->user_name->hide();
    ui->predict->hide();
    ui->refresh->hide();
    ui->date_today->setText(cur_date);

    this->setWindowTitle("Arcobaleno - weather site");

    QPalette Pal(palette());
    QImage background(":/resources/img/white.jpg");
    Pal.setBrush(QPalette::Background, background);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    window = new choosetown();
    connect(ui->select_town, SIGNAL(clicked()), window, SLOT(show()));
    connect(window, SIGNAL(sendData(QString)), this, SLOT(recieveData(QString)));

    window_2 = new auth_user();
    connect(ui->enter, SIGNAL(clicked()), window_2, SLOT(show()));
    connect(window_2, SIGNAL(sendData(QString)), this, SLOT(recieveData_2(QString)));

    window_3 = new reg_user();
    connect(ui->register_2, SIGNAL(clicked()), window_3, SLOT(show()));
    connect(window_3, SIGNAL(sendData(QString)), this, SLOT(recieveData_3(QString)));

    window_4 = new profile();
    connect(ui->profile, SIGNAL(clicked()), window_4, SLOT(show()));
    connect(ui->profile, SIGNAL(clicked()), this, SLOT(onButtonSend_4()));
    connect(this, SIGNAL(sendData(QString)), window_4, SLOT(recieveData_4(QString)));
    connect(window_4, SIGNAL(sendSignalExit()), this, SLOT(recieveData_exit_profile()));

    window_5 = new about();
    connect(ui->about, SIGNAL(clicked()), window_5, SLOT(show()));


    netwMan = new QNetworkAccessManager(this);
    connect(netwMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_netwMan(QNetworkReply*)));

    netwMan_2 = new QNetworkAccessManager(this);
    connect(netwMan_2, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_netwMan_2(QNetworkReply*)));
}

void mainpage::onButtonSend_4()
{
    emit sendData(tmp.email);
}

void mainpage::recieveData_exit_profile()
{
    ui->user_name->clear();
    ui->profile->hide();
    ui->enter->show();
    ui->register_2->show();
    ui->town_status->setText("NONE");
    ui->predict->hide();
    ui->refresh->hide();
}

void mainpage::recieveData(QString str)
{
    ui->town_status->setText(str);
}

void mainpage::recieveData_3(QString str)
{
    QStringList lst = str.split(" ");

    if (lst.size() != 3){
        qDebug()<<lst.size();
        QMessageBox::warning(this,"Warning","Проверьте введенные данные!");
        return;
    }

    tmp.email = lst.at(0);
    tmp.pass = lst.at(1);
    tmp.town = lst.at(2);

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();

    QSqlQuery query;

    int count = 0;

    query.prepare("select count(*) from Cities where town = :town group by town");
    query.bindValue(":town", tmp.town);
    query.exec();
    while (query.next())
        count++;

    if(count == 0){
        qDebug()<<count;
        QMessageBox::warning(this, "Warning","Проверьте введенные данные!");
        return;
    }

    query.exec("UPDATE sqlite_sequence SET seq=1 WHERE name='Users'");
    query.prepare("insert into Users (email, pass) values (:email, :pass)");
    query.bindValue(":email", tmp.email);
    query.bindValue(":pass", tmp.pass);
    query.exec();

    query.prepare("insert into Links (id_user, id_t) select id_user, id_t from Users, Cities where email = :email and Town = :town");
    query.bindValue(":email", tmp.email);
    query.bindValue(":town", tmp.town);
    query.exec();

    db.close();

}

bool check_is_in_db(_user _tmp)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();

    QSqlQuery query;

    int count = 0;

    query.prepare("select count(*) from Users where email = :email group by email");
    query.bindValue(":email", _tmp.email);
    query.exec();
    while (query.next())
        count++;

    query.prepare("select pass from Users where email = :email group by pass");
    query.bindValue(":email", _tmp.email);
    query.exec();
    query.next();

    if (count > 0 && query.value(0).toString() == _tmp.pass)
    {
        db.close();
        return true;
    }

    db.close();
    return false;
}

void mainpage::recieveData_2(QString str)
{
    QStringList lst = str.split(" ");

    if (lst.size() > 1) {
       // qDebug() << "2 параметра";
        tmp.email = lst.at(0);
        tmp.pass = lst.at(1);
        if (check_is_in_db(tmp) == true) {
            //qDebug()<<"проверка пройдена";
            ui->enter->hide();
            ui->register_2->hide();
            ui->label_2->show();
            ui->user_name->show();
            ui->user_name->setText(tmp.email);

            ui->profile->show();

            if (tmp.email=="admin") {
                ui->predict->show();
                ui->refresh->show();
            }

            QSqlDatabase db;
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(path_linux);
            db.open();

            QSqlQuery query;

            query.prepare("select Town from Cities C join Links L on C.id_t = L.id_t where L.id_user = (select id_user from Users where email = :email)");
            query.bindValue(":email", tmp.email);
            query.exec();
            query.next();
            tmp.town = query.value(0).toString();

            ui->town_status->setText(tmp.town);

            db.close();
        }
        else {
            QMessageBox::warning(this,"Warning","Проверьте введенные данные, либо зарегистрируйтесь!");
        }

    }
    else {
        qDebug()<<"1 пар-р";
    }
}

mainpage::~mainpage()
{
    delete ui;
    delete netwMan;
    delete netwMan_2;
}

void mainpage::universal(QString str)
{
    QString cur_town = ui->town_status->text();

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();

    QSqlQuery query;

    query.prepare(str);

    query.bindValue(":date", cur_date);
    query.bindValue(":town", cur_town);

    query.exec();

    while (query.next())
    {
        QString row = query.value(0).toString().left(5);
        QString id_t = query.value(1).toString().left(5);
        QString date = query.value(2).toString();
        QString temp = query.value(3).toString().left(5);
        QString temp_min = query.value(4).toString().left(5);
        QString temp_max = query.value(5).toString().left(5);
        QString avg_day = query.value(6).toString().left(5);
        QString sky = query.value(7).toString().left(7);
        QString wind_power = query.value(8).toString().left(5);
        QString pressure = query.value(9).toString().left(7);

        ui->textBrowser->insertPlainText(" "+date+"   |\t"+temp+"\t|\t"+temp_min+"\t|\t"+temp_max+"\t|\t"+avg_day+"\t|\t"+sky+"\t|\t"+wind_power+"\t|     "+pressure+"\n");
    }

    db.close();

}

void mainpage::on_yesterday_clicked()
{
    QString str = "select* from Report R join Cities C on C.id_t = R.id_t where (R.date < :date and C.Town = :town)";
    universal(str);

}

void mainpage::on_today_clicked()
{
    QString str = "select* from Report R join Cities C on C.id_t = R.id_t where (R.date = :date and C.Town = :town)";
    universal(str);
}

void mainpage::on_tomorrow_clicked()
{
    QString str = "select* from Report R join Cities C on C.id_t = R.id_t where (R.date > :date and C.Town = :town)";
    universal(str);
}

void mainpage::on_clean_clicked()
{
    ui->textBrowser->clear();
}

void mainpage::on_predict_clicked()
{
    request_next.setUrl(QUrl("http://api.openweathermap.org/data/2.5/forecast?q=" + ui->town_status->text() + "&APPID=78612a0e6d78c7a3e2eca8182a7d83a9"));
    netwMan_2->get(request_next);
}

void mainpage::on_refresh_clicked()
{
    request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/weather?q=" + ui->town_status->text() + "&APPID=78612a0e6d78c7a3e2eca8182a7d83a9"));
    netwMan->get(request);
}

void mainpage::slot_netwMan(QNetworkReply *rep)
{
    rep->waitForReadyRead(1000);

    doc = QJsonDocument::fromJson(rep->readAll(), &docEr);

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();
    QSqlQuery query;

    query.prepare("select id_t from Cities where Town = :Town");
    query.bindValue(":Town", ui->town_status->text());
    query.exec();
    query.next();
    int _id_t = query.value(0).toInt();

    query.prepare("delete from Report where (date = :date and id_t = :id_t)");
    query.bindValue(":date", cur_date);
    query.bindValue(":id_t", _id_t);
    query.exec();

    double temp, temp_min, temp_max, avg_day, sky, wind_power, pressure;

    query.exec("UPDATE sqlite_sequence SET seq = 1 WHERE name = 'Report'");


    w_tmp = doc.object().value("main").toObject();
    temp = w_tmp.value("temp").toDouble()-273;
    temp_min = w_tmp.value("temp_min").toDouble()-273;
    temp_max = w_tmp.value("temp_max").toDouble()-273;
    avg_day = (temp + temp_min + temp_max) / 3;
    pressure = w_tmp.value("pressure").toDouble();
    sky = doc.object().value("clouds").toObject().value("all").toDouble();
    wind_power = doc.object().value("wind").toObject().value("speed").toDouble();

    query.prepare("insert into Report (id_t, date, temp, temp_min, temp_max, avg_day, sky, wind_power, pressure) values (:id_t, :date, :temp, :temp_min, :temp_max, :avg_day, :sky, :wind_power, :pressure)");
    query.bindValue(":id_t", _id_t);
    query.bindValue(":date", cur_date);
    query.bindValue(":temp", temp);
    query.bindValue(":temp_min", temp_min);
    query.bindValue(":temp_max", temp_max);
    query.bindValue(":avg_day", avg_day);
    query.bindValue(":sky", sky);
    query.bindValue(":wind_power", wind_power);
    query.bindValue(":pressure", pressure);
    query.exec();


    db.close();

    rep->deleteLater();

}

void mainpage::slot_netwMan_2(QNetworkReply *rep)
{
    rep->waitForReadyRead(1000);

    doc = QJsonDocument::fromJson(rep->readAll(), &docEr);

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();
    QSqlQuery query;

    query.prepare("select id_t from Cities where Town = :Town");
    query.bindValue(":Town", ui->town_status->text());
    query.exec();
    query.next();
    int _id_t = query.value(0).toInt();

    query.prepare("delete from Report where (date > :date and id_t = :id_t)");
    query.bindValue(":date", cur_date);
    query.bindValue(":id_t", _id_t);
    query.exec();

    double temp[3], temp_min[3], temp_max[3], avg_day[3], sky[3], wind_power[3], pressure[3];

    QString _date[3];
    for (int k = 0; k < 3; k++)
        _date[k] = QDate::currentDate().addDays(k+1).toString("yyyy.MM.dd");

    query.exec("UPDATE sqlite_sequence SET seq = 1 WHERE name = 'Report'");



    for (int i = 0; i < 3; i++) {
        w_tmp = doc.object().value("list")[(i+1)*6].toObject().value("main").toObject();
        temp[i] = w_tmp.value("temp").toDouble()-273;
        temp_min[i] = w_tmp.value("temp_min").toDouble()-273;
        temp_max[i] = w_tmp.value("temp_max").toDouble()-273;
        avg_day[i] = (temp[i] + temp_min[i] + temp_max[i]) / 3;
        pressure[i] = w_tmp.value("pressure").toDouble();
        sky[i] = doc.object().value("list")[(i+1)*6].toObject().value("clouds").toObject().value("all").toDouble();
        wind_power[i] = doc.object().value("list")[(i+1)*6].toObject().value("wind").toObject().value("speed").toDouble();

        query.prepare("insert into Report (id_t, date, temp, temp_min, temp_max, avg_day, sky, wind_power, pressure) values (:id_t, :date, :temp, :temp_min, :temp_max, :avg_day, :sky, :wind_power, :pressure)");
        query.bindValue(":id_t", _id_t);
        query.bindValue(":date", _date[i]);
        query.bindValue(":temp", temp[i]);
        query.bindValue(":temp_min", temp_min[i]);
        query.bindValue(":temp_max", temp_max[i]);
        query.bindValue(":avg_day", avg_day[i]);
        query.bindValue(":sky", sky[i]);
        query.bindValue(":wind_power", wind_power[i]);
        query.bindValue(":pressure", pressure[i]);
        query.exec();
    }

    db.close();

    rep->deleteLater();
}


















