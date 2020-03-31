#include "choosetown.h"
#include "ui_choosetown.h"

choosetown::choosetown(QWidget *parent) :
    QWidget(parent), ui(new Ui::choosetown)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("Выбор города");

    connect(ui->accept, SIGNAL(clicked()), this, SLOT(onButtonSend())); // подключаем клик по кнопке к определенному нами слоту

    QPalette Pal(palette());
    QImage background(":/resources/img/white.jpg");
    Pal.setBrush(QPalette::Background, background);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

}

choosetown::~choosetown()
{
    delete ui;
}

void choosetown::onButtonSend()
{
    emit sendData(ui->town->text());

    this->hide();
}

void choosetown::on_moscow_clicked()
{
    ui->town->setText("Moscow");
}

void choosetown::on_vladimir_clicked()
{
    ui->town->setText("Vladimir");
}

void choosetown::on_voronej_clicked()
{
    ui->town->setText("Voronezh");
}

void choosetown::on_piter_clicked()
{
    ui->town->setText("Saint Petersburg");
}

void choosetown::on_cancel_clicked()
{
    ui->town->clear();
    this->hide();
}

void choosetown::on_arhang_clicked()
{
    ui->town->setText("Arkhangelsk");
}

void choosetown::on_pskov_clicked()
{
    ui->town->setText("Pskov");
}

void choosetown::on_krim_clicked()
{
    ui->town->setText("Crimea");
}

void choosetown::on_rostov_clicked()
{
    ui->town->setText("Rostov");
}

void choosetown::on_krasnodar_clicked()
{
    ui->town->setText("Krasnodar");
}

void choosetown::on_volgograd_clicked()
{
    ui->town->setText("Volgograd");
}
