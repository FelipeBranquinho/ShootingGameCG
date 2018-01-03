#include "tiro.h"

void Tiro::moveTiro(GLdouble timeDiference){
    cx += vel*timeDiference*cos(angulo*M_PI/180);
    cy += vel*timeDiference*sin(angulo*M_PI/180);
    cz += vel*timeDiference*sin(anguloZ*M_PI/180);
}