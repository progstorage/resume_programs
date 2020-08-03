#pragma once

#include <cmath>

#include <QDebug>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPixmap>
#include <QThread>

/***********************************************
TODO:
1) 2й поток (для ракетки)

2) указывать текущий счет
+ завести переменную score

3) после окончания игры предложить начать заново
(сброс bricks, координат мяча и ракетки)

3') новое окно: 2 кнопки - restart, exit

4) нарисовать красную точку (мб) в центре ракетки и мяча

5) фон
***********************************************/

class MainWindow;
class Game2D;
class Racket;
class Ball;
class Brick;

static const float pi = 3.14;

static std::vector<Brick> bricks;

static std::map<std::string, const int> borders = {
    {"left", -50},
    {"right", 510},
    {"up", 0},
    {"down", 425}
};


namespace Ui {
	class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* = nullptr);
    ~MainWindow();

protected:
   void keyPressEvent(QKeyEvent*);
   void keyReleaseEvent(QKeyEvent*);

private:
    Ui::MainWindow* ui;
    Racket*         racket;
};


class Game2D: public QObject {
    Q_OBJECT

public:
    Game2D()    = default;
    ~Game2D()   = default;

protected:
    float   x, y;
    int     height, width;

    // чисто виртуальные ф-ии
    virtual inline void                    move(void)      = 0;
    virtual inline std::pair<float, float> center(void)    = 0;
};


class Racket: virtual public Game2D {
    friend class MainWindow;
    friend class Ball;

public:
    explicit Racket(float = 0, float = 0);

    ~Racket() = default;

    void                    move();
    std::pair<float, float> center(void);

    friend void Racket_Tick(Racket&, Ui::MainWindow*);

private:
    static const int height = 80;
    static const int width  = 120;

    float step_x, step_y;
};


class Ball: virtual public Game2D {
public:
    explicit Ball(float = 0, float = 0);

    ~Ball() = default;

    void                    move();
    std::pair<float, float> center();

    friend void norm_angle(Ball&);
    friend void right_reflection(Ball&);
    friend void top_reflection(Ball&, int);
    friend void Ball_Tick(Ball&, Ui::MainWindow*);
    friend void fly_to_end(Ball&, Ui::MainWindow*);
    friend void find_and_heat_block(Ball&, Ui::MainWindow*);
    friend void racket_reflection(Ball&, Racket&);
    friend bool is_racket_heat(Ball&, Racket&);
    friend float get_angle(Ball&, Racket&);
    friend void ball_to_right(Ball&, Racket&, Ui::MainWindow*);
    friend void ball_to_left(Ball&, Racket&, Ui::MainWindow*);
    friend void game_init(Ball&, Racket&, Ui::MainWindow*);

private:
    static const int r      = 25;
    static const int damage = 10;

    float phi;

/************************
public slots:
    void process(Ui::MainWindow*);

signals:
    void finished();
    //void my_start(Ui::MainWindow*);
    //void error(QString err);

************************/
};


class Brick {
public:
    static const int height = 40;
    static const int width  = 80;

    int     x, y, hp;
    bool    is_alive;

    Brick(int _x = 0, int _y = 0):
        x(_x), y(_y), hp(20), is_alive(true) {}

    ~Brick() = default;
};


void build_wall(Ui::MainWindow*);

inline void destroy_brick_1(Ui::MainWindow*);
inline void destroy_brick_2(Ui::MainWindow*);
inline void destroy_brick_3(Ui::MainWindow*);
inline void destroy_brick_4(Ui::MainWindow*);
inline void destroy_brick_5(Ui::MainWindow*);

inline float to_deg(float);
inline float to_rad(float);

[[noreturn]] void end_of_game(Ui::MainWindow*);


/***************************
class T_Cl: public QObject {
    Q_OBJECT
public:
    T_Cl(QString _name): name(_name) {}
    ~T_Cl() = default;

    void run_fun(int);
    void run();

public slots:
    void process();


signals:
    void finished();
    void errorString(QString);

private:
    QString name;
};

void T_Cl::process() {
    qDebug() << "Hi";
    for (int i = 0; i < 100; i++)
        this->run_fun(i);
    emit finished();
}

void T_Cl::run_fun(int num) {
    qDebug() << this->name << "\tnum: " << num;
    //this->thread()->msleep(10);
}
******************************/
