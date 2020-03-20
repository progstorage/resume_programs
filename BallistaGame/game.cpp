#include "game.h"
#include "ui_game.h"
#include "ammo.h"
#include <QPixmap>
#include <QMessageBox>
#include <QThread>

Gun Left(364,960), Right(1516,960);

Kernel Ker(32);

game::game(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::game)
{
    ui->setupUi(this);

    this->setWindowTitle("BallistaGame");

    previous(ui);

    connect(ui->left_button, SIGNAL(clicked()), this, SLOT(left_button()));
    connect(ui->right_button, SIGNAL(clicked()), this, SLOT(right_button()));
}

void Tick(Ui::game *ui)
{
    ui->img_kernel->hide();
    ui->img_kernel->move(int(Ker.x), int(Ker.y));
    ui->img_kernel->show();
}

void previous(Ui::game* ui)
{

    QPixmap left(":/resources/img/left.png");
    QPixmap right(":/resources/img/right.png");

    int l_w = ui->img_left->width();
    int l_h = ui->img_left->height();
    int r_w = ui->img_right->width();
    int r_h = ui->img_right->height();

    ui->img_left->setPixmap(left.scaled(l_w, l_h, Qt::KeepAspectRatio));
    ui->img_right->setPixmap(right.scaled(r_w, r_h, Qt::KeepAspectRatio));

    updateLeftHP(Left.HP, ui);
    updateRightHP(Right.HP, ui);

    ui->left_button->show();
    ui->right_button->hide();
}

void game::left_button()
{
    float _m, _v, _phi;

    _m=ui->mass->text().toFloat();
    _v=ui->rate->text().toFloat();
    _phi=ui->angle->text().toFloat();

    QPixmap kernel(":/resources/img/kernel.png");

    int k_w = ui->img_kernel->width();
    int k_h = ui->img_kernel->height();

    ui->img_kernel->setPixmap(kernel.scaled(k_w, k_h, Qt::KeepAspectRatio));

    if(_m > 26 || _v > 26 || _phi > 90 || _m < 0 || _v < 0 || _phi < 0)
    {
        ui->img_kernel->hide();
        ui->left_button->hide();
        QMessageBox::warning(this,"","Неправильные параметры!");
        ui->right_button->show();
        return;
    }

    for(float t=0.2; t<50.0; t+=0.2)
        {
           Ker.SetXtoRight(t, _m, _v, _phi);
           Ker.SetY(t, _m, _v, _phi);

           Tick(ui);

           if(isInside(Ker.x, Ker.y)!=true) //вылетел за границы поля
             {
               //QMessageBox::information(this,"","Вылетел!");
               break;
             }

           if(isShootRight(Ker.x, Ker.y)==true)    //попал в цель
            {
                Ker.SetParam(_m, _v);
                Right.update(Ker.damage);
                updateRightHP(Right.HP, ui);
                //QMessageBox::information(this,"","Попал!");
                if(Right.HP<=0)
                {
                    QMessageBox::information(this, "GameOver", "Левый Игрок победил!");
                    this->close();
                }
                break;
            }

           QApplication::processEvents();
           QThread::msleep(100);
        }

    ui->img_kernel->hide();
    ui->left_button->hide();
    ui->right_button->show();
}

void game::right_button()
{
    float _m, _v, _phi;

    _m=ui->mass_2->text().toFloat();
    _v=ui->rate_2->text().toFloat();
    _phi=ui->angle_2->text().toFloat();

    QPixmap kernel(":/resources/img/kernel.png");

    int k_w = ui->img_kernel->width();
    int k_h = ui->img_kernel->height();

    ui->img_kernel->setPixmap(kernel.scaled(k_w, k_h, Qt::KeepAspectRatio));

    if(_m > 26 || _v > 26 || _phi > 90 || _m < 0 || _v < 0 || _phi < 0)
    {
        ui->img_kernel->hide();
        ui->right_button->hide();
        QMessageBox::warning(this,"","Неправильные параметры!");
        ui->left_button->show();
        return;
    }

    for(float t=0.2; t<50.0; t+=0.2)
        {
           Ker.SetXtoLeft(t, _m, _v, _phi);
           Ker.SetY(t, _m, _v, _phi);

           Tick(ui);

           if(isInside(Ker.x, Ker.y)!=true) //вылетел за границы поля
             {
               //QMessageBox::information(this,"","Вылетел!");
               break;
             }

           if(isShootLeft(Ker.x,Ker.y)==true)    //попал в цель
            {
                Ker.SetParam(_m, _v);
                Left.update(Ker.damage);
                updateLeftHP(Left.HP, ui);
                //QMessageBox::information(this,"","Попал!");
                if(Left.HP<=0)
                {
                    QMessageBox::information(this, "GameOver", "Правый Игрок победил!");
                    this->close();
                }
                break;
            }

           QApplication::processEvents();
           QThread::msleep(100);
        }

    ui->img_kernel->hide();
    ui->right_button->hide();
    ui->left_button->show();
}

void updateLeftHP(float value, Ui::game *ui)
{
    ui->left_hp->setText(QString::number(value) + "/10000");
}

void updateRightHP(float value, Ui::game *ui)
{
    ui->right_hp->setText(QString::number(value) + "/10000");
}

bool isInside(float _x, float _y)
{
    if(_x>332 && _x<1548 && _y<992 && _y>32)
        return true;
    return false;
}

bool isShootLeft(float _x, float _y)
{
    if(sqrt((_x-364)*(_x-364)+(_y-960)*(_y-960))<=Ker.r+Left.R)
        return true;
    return false;
}

bool isShootRight(float _x, float _y)
{
    if(sqrt((_x-1516)*(_x-1516)+(_y-960)*(_y-960))<=Ker.r+Right.R)
        return true;
    return false;
}

game::~game()
{
    delete ui;
}

