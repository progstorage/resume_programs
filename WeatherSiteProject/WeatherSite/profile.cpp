#include "profile.h"
#include "ui_profile.h"
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include "information.h"
#include <QStringList>
#include <QMessageBox>
#include <QDebug>

static _user tmp_2(-1, "", "", "");

profile::profile(QWidget *parent) :
    QWidget(parent), ui(new Ui::profile)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Личный кабинет");

    connect(ui->exit, SIGNAL(clicked()), this, SLOT(on_exit_clicked()));

    QPalette Pal(palette());
    QImage background(":/resources/img/white.jpg");
    Pal.setBrush(QPalette::Background, background);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
}

profile::~profile()
{
    delete ui;
}

void profile::recieveData_4(QString str)
{
    tmp_2.email = str;

    ui->user_name->setText(str);
    ui->name_2->clear();
    ui->sname_2->clear();
    ui->tel_2->clear();
    ui->user_towns->clear();
    ui->new_town->clear();
    ui->name->clear();
    ui->sname->clear();
    ui->tel->clear();

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();

    QSqlQuery query;

    query.prepare("select id_user from Users where email = :email");
    query.bindValue(":email", tmp_2.email);
    query.exec();
    query.next();
    tmp_2.id = query.value(0).toInt();

    query.prepare("select Town from Cities C join Links L on C.id_t = L.id_t where L.id_user = (select id_user from Users where email = :email)");
    query.bindValue(":email", str);
    query.exec();

    while(query.next())
    {
        QString town = query.value(0).toString();
        ui->user_towns->insertPlainText(town + "\n");
    }

    query.prepare("select name, sname, tel from Info I join Users S on I.id_user = S.id_user where S.email = :email");
    query.bindValue(":email", str);
    query.exec();

    while(query.next())
    {
        QString name = query.value(0).toString();
        QString sname = query.value(1).toString();
        QString tel = query.value(2).toString();

        ui->name_2->setText(name);
        ui->sname_2->setText(sname);
        ui->tel_2->setText(tel);
    }

    db.close();
}

void profile::on_accept_2_clicked()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();

    QSqlQuery query;

    query.prepare("insert into Info (id_user, name, sname, tel) values (:id_user, :name, :sname, :tel)");
    query.bindValue(":id_user", tmp_2.id);
    query.bindValue(":name", ui->name->text());
    query.bindValue(":sname", ui->sname->text());
    query.bindValue(":tel", ui->tel->text());
    query.exec();
    //qDebug()<<"пользователь добавлен!";

    db.close();

    recieveData_4(tmp_2.email);
}

void profile::on_cancel_2_clicked()
{
     ui->name->clear();
     ui->sname->clear();
     ui->tel->clear();
}

void profile::on_cancel_clicked()
{
    ui->new_town->clear();
}

void profile::on_accept_clicked()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_linux);
    db.open();

    QSqlQuery query;

    query.prepare("select id_t from Cities where Town = :town");
    query.bindValue(":town", ui->new_town->text());
    query.exec();
    query.next();
    tmp_2.town = query.value(0).toString();

    query.prepare("insert into Links (id_user, id_t) values (:id_user, :id_t)");
    query.bindValue(":id_user", tmp_2.id);
    query.bindValue(":id_t", tmp_2.town);
    query.exec();
   // qDebug()<<"город добавлен!";

    db.close();

    recieveData_4(tmp_2.email);
}

void profile::on_exit_clicked()
{
    tmp_2.set_zero();
    ui->user_name->setText("");
    ui->name_2->clear();
    ui->sname_2->clear();
    ui->tel_2->clear();
    ui->user_towns->clear();
    ui->new_town->clear();
    ui->name->clear();
    ui->sname->clear();
    ui->tel->clear();

    emit sendSignalExit();

    this->hide();
}

void profile::on_close_clicked()
{
    this->close();
}
