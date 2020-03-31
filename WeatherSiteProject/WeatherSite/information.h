#ifndef INFORMATION_H
#define INFORMATION_H

//#include <iostream>
//#include <string>
#include <QString>
//using namespace std;

class _user{
public:
    int id;
    QString email;
    QString pass;
    QString town;

    _user(int _id = 0, QString _email = "", QString _pass = "", QString _town = "Москва") :
        id(_id), email(_email), pass(_pass), town(_town) {}

    void set_zero()
    {
        id = 0;
        email = pass = town = "";
    }

};


#endif // INFORMATION_H




