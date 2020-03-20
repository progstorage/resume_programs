#include "menu.h"
#include "ui_menu.h"
#include "rules.h"
#include <QWidget>

menu::menu(QWidget *parent) :
    QDialog(parent), ui(new Ui::menu)
{
    ui->setupUi(this);
    window = new game();
}

menu::~menu()
{
    delete ui;
}

void menu::on_pushButton_clicked()
{
    this->close();
    window->show();
}

void menu::on_pushButton_2_clicked()
{
    QApplication::quit();
}

void menu::on_pushButton_3_clicked()
{
    Rules win;
    win.move(1000, 300);
    win.setWindowTitle("Rules");
    win.setModal(true);
    win.exec();
}
