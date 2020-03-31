#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QWidget(parent), ui(new Ui::about)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("About");
    QPalette Pal(palette());
    QImage background(":/resources/img/white.jpg");
    Pal.setBrush(QPalette::Background, background);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
}

about::~about()
{
    delete ui;
}

void about::on_exit_clicked()
{
    this->close();
}
