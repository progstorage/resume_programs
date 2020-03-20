#include <menu.h>
#include <math.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    menu w;
    w.show();
    return app.exec();
}
