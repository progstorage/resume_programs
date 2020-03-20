#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QMainWindow>


namespace Ui {
class game;
}

class game : public QMainWindow
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    ~game();


private slots:
    void left_button();
    void right_button();

private:
    Ui::game *ui;
};

void previous(Ui::game*);

void Tick(Ui::game*);

bool isInside(float, float);

bool isShootLeft(float, float);

bool isShootRight(float, float);

void updateLeftHP(float, Ui::game*);

void updateRightHP(float, Ui::game*);

#endif // GAME_H
