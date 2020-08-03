#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget* parent): QMainWindow(parent),
    ui(new Ui::MainWindow), racket(new Racket(20, 200)) {
    // MainWindow's constructor
    Ball ball(150, 200);

    ui->setupUi(this);
    this->setWindowTitle("Racket Game");
    this->show();

    game_init(ball, *racket, ui);
    ball_to_right(ball, *racket, ui);

    /***************************
    QThread* th1 = new QThread;
    //QThread* th2 = new QThread;

    Ball* b = &ball;
    //Racket* c = &racket;

    //T_Cl* b = new T_Cl("first");
    //T_Cl* c = new T_Cl("second");

    b->moveToThread(th1);
    //c->moveToThread(th2);
    //connect(b, SIGNAL(errorString(QString)), this, SLOT(errorString(QString)));
    connect(th1, SIGNAL(started()), b, SLOT(process(Ui::MainWindow*)));
    connect(b, SIGNAL(finished()), th1, SLOT(quit()));
    connect(b, SIGNAL(finished()), b, SLOT(deleteLater()));
    connect(th1, SIGNAL(finished()), th1, SLOT(deleteLater()));


    connect(th2, SIGNAL(started()), c, SLOT(process()));
    connect(c, SIGNAL(finished()), th2, SLOT(quit()));
    connect(c, SIGNAL(finished()), c, SLOT(deleteLater()));
    connect(th2, SIGNAL(finished()), th2, SLOT(deleteLater()));

    //th1->start();

    //b->my_start(ui);
    //th2->start();

    //this->setFocusPolicy(Qt::ClickFocus);
    ***************************************/
}


void right_reflection(Ball& _ball) {
    // reflection from the right border
    _ball.phi = pi - _ball.phi;
    norm_angle(_ball);
}


void racket_reflection(Ball& _ball, Racket& _racket) {
    // reflection from the racket
    norm_angle(_ball);
    float _phi = to_deg(get_angle(_ball, _racket));

    if ((_phi >= -45 && _phi <= 45) // [-45, 45] спектр
            || (_phi >= 315 && _phi <= 405))
        _ball.phi = -to_rad(_phi);
    else
        _ball.phi = pi / 2 - to_rad(_phi);
}


void ball_to_right(Ball& _ball, Racket& _racket, Ui::MainWindow* ui) {
    // the ball flies to the right
    racket_reflection(_ball, _racket);
    while (true) {
        if (_ball.x >= borders["right"] && _ball.y >= borders["up"]
                && _ball.y <= borders["down"]) {
            // удар по стене
            find_and_heat_block(_ball, ui);
            ball_to_left(_ball, _racket, ui);
            break;
        }
        else if (_ball.y <= borders["up"]) {   // удар об верхнюю стенку
            top_reflection(_ball, 1);
        }
        else if (_ball.y >= borders["down"] || _ball.x <= borders["left"]) {
            // вылетели за пределы поля
            end_of_game(ui);     // end of game -> окошко с предложением перезапуска
        }

        _ball.move();
        Ball_Tick(_ball, ui);
    }
}


void ball_to_left(Ball& _ball, Racket& _racket, Ui::MainWindow* ui) {
    // the ball flies to the left
    right_reflection(_ball);
    while (true) {
        if (_ball.y >= borders["down"] || _ball.x <= borders["left"]) {
            // вылетели за пределы поля
            end_of_game(ui);
        }
        else if (_ball.y <= borders["up"]) {
            // удар об потолок
            top_reflection(_ball, -1);
        }

        if (is_racket_heat(_ball, _racket) == true) {
            // удар по ракетке
            ball_to_right(_ball, _racket, ui);
        }

        _ball.move();
        Ball_Tick(_ball, ui);
    }
}


void fly_to_end(Ball& _ball, Ui::MainWindow* ui) {
    // если встречен "пустой" кирпич, лететь до конца экрана
    while (_ball.x <= 625) {
        _ball.move();
        Ball_Tick(_ball, ui);
    }
    end_of_game(ui);
}


float to_rad(float _phi) {
    // converts degrees to radians
    return _phi * 3.14 / 180;
}


float to_deg(float _phi) {
    // converts radians to degrees
    return _phi / 3.14 * 180;
}


void top_reflection(Ball& _ball, int _flag) {
    // imitating of top reflecting
    if (_flag == -1) {   // летим справа
        _ball.phi *= -1;
    }
    else if (_flag == 1) { // летим слева
        _ball.phi = 2 * pi - _ball.phi;
    }
    norm_angle(_ball);
}


bool is_racket_heat(Ball& _ball, Racket& _racket) {
    // checking if the racket hits the ball
    float _x = _ball.center().first - _racket.center().first;
    float _y = _ball.center().second - _racket.center().second;
    float _r = _ball.r + 45.0;

    if (_x * _x + _y * _y <= _r * _r)
        return true;
    return false;
}


std::pair<float, float> Ball::center() {
    // return Ball's center
    return {x + r, y + r};
}


void MainWindow::keyPressEvent(QKeyEvent* event) {
    // reacts on key pressed
    int key = event->key();
    switch (key) {
        case Qt::Key_Left:
            racket->step_x = -10;
            racket->move();
            break;
        case Qt::Key_Right:
            racket->step_x = 10;
            racket->move();
            break;
        case Qt::Key_Up:
            racket->step_y = -10;
            racket->move();
            break;
        case Qt::Key_Down:
            racket->step_y = 10;
            racket->move();
            break;
        case Qt::Key_F1:
            this->close();
            exit(0);
        default:
            return;
    }
    Racket_Tick(*racket, ui);
}


void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    // reacts on key released
    int key = event->key();
    switch (key) {
        case Qt::Key_Left:
            racket->step_x = 0;
            //racket->move();
            break;
        case Qt::Key_Right:
            racket->step_x = 0;
            //racket->move();
            break;
        case Qt::Key_Up:
            racket->step_y = 0;
            //racket->move();
            break;
        case Qt::Key_Down:
            racket->step_y = 0;
            //racket->move();
            break;
    }
}


void Ball_Tick(Ball& _ball, Ui::MainWindow* ui) {
    // drawing the movement of the ball on the playing field
    ui->img_ball->hide();
    ui->img_ball->move(int(_ball.x), int(_ball.y));
    ui->img_ball->show();

    QApplication::processEvents();
    QThread::msleep(100);
}


void Racket_Tick(Racket& _racket, Ui::MainWindow* ui) {
    // drawing the movement of the racket on the playing field
    ui->img_racket->hide();
    ui->img_racket->move(int(_racket.x), int(_racket.y));
    ui->img_racket->show();
}


void Racket::move() {
    // moving racket
    x += step_x;
    y += step_y;
}


void game_init(Ball& _ball, Racket& _racket, Ui::MainWindow* ui) {
    // preparing to start the game

    // ball and racket:
    QPixmap pix_ball(":/resources/img/ball.jpg");
    QPixmap pix_racket(":/resources/img/racket.jpg");

    int _ball_w = ui->img_ball->width();
    int _ball_h = ui->img_ball->height();
    int r_w = ui->img_racket->width();
    int r_h = ui->img_racket->height();

    ui->img_ball->setPixmap(pix_ball.scaled(_ball_w, _ball_h, Qt::KeepAspectRatio));
    ui->img_racket->setPixmap(pix_racket.scaled(r_w, r_h, Qt::KeepAspectRatio));

    Racket_Tick(_racket, ui);
    Ball_Tick(_ball, ui);

    // bricks:
    build_wall(ui);
}


void build_wall(Ui::MainWindow* ui) {
    // building a wall of bricks
    bricks.push_back({560, 320});
    bricks.push_back({560, 240});
    bricks.push_back({560, 160});
    bricks.push_back({560, 80});
    bricks.push_back({560, 0});

    QPixmap pix_brick_1(":/resources/img/brick.jpg");
    QPixmap pix_brick_2(":/resources/img/brick.jpg");
    QPixmap pix_brick_3(":/resources/img/brick.jpg");
    QPixmap pix_brick_4(":/resources/img/brick.jpg");
    QPixmap pix_brick_5(":/resources/img/brick.jpg");

    int br_w = ui->img_brick_1->width();
    int br_h = ui->img_brick_1->height();

    ui->img_brick_1->setPixmap(pix_brick_1.scaled(br_w, br_h, Qt::KeepAspectRatio));
    ui->img_brick_2->setPixmap(pix_brick_2.scaled(br_w, br_h, Qt::KeepAspectRatio));
    ui->img_brick_3->setPixmap(pix_brick_3.scaled(br_w, br_h, Qt::KeepAspectRatio));
    ui->img_brick_4->setPixmap(pix_brick_4.scaled(br_w, br_h, Qt::KeepAspectRatio));
    ui->img_brick_5->setPixmap(pix_brick_5.scaled(br_w, br_h, Qt::KeepAspectRatio));

    ui->img_brick_1->show();
    ui->img_brick_2->show();
    ui->img_brick_3->show();
    ui->img_brick_4->show();
    ui->img_brick_5->show();
}


std::pair<float, float> Racket::center() {
    // return racket's center
    return {x + width/2, y + height/3};
}


float get_angle(Ball& _ball, Racket& _racket) {
    // return angle between racket and ball
    float x1 = _racket.center().first;
    float y1 = _racket.center().second;

    float x2 = _ball.center().first;
    float y2 = _ball.center().second;

    float tmp = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    int sgn;
    (x2 - x1 > 0) ? (sgn = -1) : (sgn = 1);
    return sgn * (acos((x2 - x1) / tmp));
}


void Ball::move() {
    // moving Ball's object
    x += 10 * cos(phi);
    y -= 10 * sin(phi);
}


[[noreturn]] void end_of_game(Ui::MainWindow* ui) {
    // временно
    qDebug() << "end of game";
    ui->centralWidget->close();
    exit(0);
}


void find_and_heat_block(Ball& _ball, Ui::MainWindow* ui) {
    // deals damage to the brick hit by the ball
    for (unsigned int i = 0; i < bricks.size(); i++) {
        if (_ball.y >= bricks[i].y && _ball.y <= bricks[i].y + bricks[i].height) {
            if (bricks[i].is_alive == true) {
                bricks[i].hp -= _ball.damage;
                if (bricks[i].hp <= 0) {
                    bricks[i].is_alive = false;

                    switch(i) {
                    case 0:
                        destroy_brick_1(ui);
                        break;
                    case 1:
                        destroy_brick_2(ui);
                        break;
                    case 2:
                        destroy_brick_3(ui);
                        break;
                    case 3:
                        destroy_brick_4(ui);
                        break;
                    case 4:
                        destroy_brick_5(ui);
                        break;
                    }
                }
                return;
            }
            else
                fly_to_end(_ball, ui);
        }
    }
}


void norm_angle(Ball& _ball) {
    // normalizes the angle
    float _phi = _ball.phi;

    while (_phi >= 2*pi)
        _phi -= 2*pi;
    while (_phi <= -2*pi)
        _phi += 2*pi;

    _ball.phi = _phi;
}


Racket::Racket(float _x, float _y) {
    // Racket's constructor
    x = _x;
    y = _y;
}


Ball::Ball(float _x, float _y): phi(0.0) {
    // Ball's constructor
    x = _x;
    y = _y;
    height = 50;
    width = 50;
}


MainWindow::~MainWindow() {
    // MainWindows's destructor
    delete ui;
}


// destroy bricks:
void destroy_brick_1(Ui::MainWindow* ui) {
    ui->img_brick_1->hide();
}

void destroy_brick_2(Ui::MainWindow* ui) {
    ui->img_brick_2->hide();
}

void destroy_brick_3(Ui::MainWindow* ui) {
    ui->img_brick_3->hide();
}

void destroy_brick_4(Ui::MainWindow* ui) {
    ui->img_brick_4->hide();
}

void destroy_brick_5(Ui::MainWindow* ui) {
    ui->img_brick_5->hide();
}
