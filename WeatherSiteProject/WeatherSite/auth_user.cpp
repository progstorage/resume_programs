#include "auth_user.h"
#include "ui_auth_user.h"

auth_user::auth_user(QWidget *parent) :
    QWidget(parent), ui(new Ui::auth_user)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Авторизация");

    connect(ui->login, SIGNAL(clicked()), this, SLOT(onButtonSend())); // подключаем клик по кнопке к определенному нами слоту

    QPalette Pal(palette());
    QImage background(":/resources/img/white.jpg");
    Pal.setBrush(QPalette::Background, background);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

}

auth_user::~auth_user()
{
    delete ui;
}

void auth_user::onButtonSend()
{
    emit sendData(ui->email_1->text()+" "+ui->pass_1->text());
    ui->email_1->clear();
    ui->pass_1->clear();
    this->hide();
}

void auth_user::on_cancel_clicked()
{
    this->close();
}
