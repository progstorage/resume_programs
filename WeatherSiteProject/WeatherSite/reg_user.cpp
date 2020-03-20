#include "reg_user.h"
#include "ui_reg_user.h"

reg_user::reg_user(QWidget *parent) :
    QWidget(parent), ui(new Ui::reg_user)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Регистрация");

    connect(ui->accept, SIGNAL(clicked()), this, SLOT(onButtonSend()));

    QPalette Pal(palette());
    QImage background(":/resources/img/white.jpg");
    Pal.setBrush(QPalette::Background, background);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
}

reg_user::~reg_user()
{
    delete ui;
}

void reg_user::onButtonSend()
{
    emit sendData(ui->email->text()+" "+ui->pass->text()+" "+ui->town->text());
    ui->email->clear();
    ui->pass->clear();
    ui->town->clear();
    this->hide();
}

void reg_user::on_cancel_clicked()
{
    this->close();
}
