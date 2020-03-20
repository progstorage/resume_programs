#ifndef AMMO_H
#define AMMO_H

#include <math.h>

#define g 9.81

class Gun {
public:
    float HP;
    float x, y, R;

    Gun(float _x, float _y, float _R = 12, float _HP = 10000) :
        x(_x), y(_y), R(_R), HP(_HP) {}

    void update(float damage){
        HP -= damage;
    }
};

class Kernel {
private:
    float m, v;

public:
    float x, y;
    float r;
    float damage;

    Kernel(float _r) : r(_r) {}

    void SetParam(float _m, float _v)
    {
        m = _m; v = _v;
        damage = m * v * v / 2;
    }

    void SetXtoLeft(float t, float _m, float _v, float _phi){
        float phi = _phi * 3.14 / 180;
        x = 1390 - _m * _v * cos(phi) * t;
    }

    void SetXtoRight(float t, float _m, float _v, float _phi){
        float phi = _phi * 3.14 / 180;
        x = 428 + _m * _v * cos(phi) * t;
    }

    void SetY(float t, float _m, float _v, float _phi){
        float phi = _phi * 3.14 / 180;
        y = 896 - _m *_v * sin(phi) * t + g * t * t / 2;
    }
};

#endif // AMMO_H
