#ifndef TIRO_H
#define TIRO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "circulo.h"

class Tiro:public Circulo{
    public:
        GLfloat angulo,anguloZ;
        GLfloat vel;
        Tiro(GLfloat cx, GLfloat cy,GLfloat cz,GLfloat h,GLfloat raio, GLfloat corR,
            GLfloat corG, GLfloat corB, GLint id, GLfloat angulo,GLfloat anguloz, GLfloat vel, GLuint tex):
                Circulo(cx,cy,cz,h,raio,corR,corG,corB,id,tex){
            this->angulo = angulo;
            this->vel = vel;
            this->anguloZ = anguloz;
        }
        void moveTiro(GLdouble timeDiference);
};
#endif /* TIRO_H */