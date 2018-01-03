#ifndef CIRCULO_H
#define CIRCULO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

class Circulo
{
public:
    GLuint texture;
    GLfloat cx,cy,cz,h,raio,corR,corG,corB;
    GLint id;
    Circulo(){
            cx = 0;
            cy = 0;
            cz = 0;
            h = 0;
            raio = 0;
            corR = 0;
            corG = 0;
            corB = 0;
            texture = 0;
    }
    Circulo(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat h, GLfloat raio, GLfloat corR, GLfloat corG, GLfloat corB, GLint id, GLuint texture){
            this->cx=cx;
            this->cy=cy;
            this->cz=cz;
            this->h=h;
            this->raio=raio;
            this->corR=corR;
            this->corG=corG;
            this->corB=corB;
            this->id=id;
            this->texture = texture;
    }
    void desenha();
    void desenhaCilindro(int interno, int sl, int st);
    void desenhaCirculo();
    void desenhaCircunferencia();
		void desenhaEsfera();
    int fora(Circulo* c);
    int dentro(Circulo* c);
    int colide(Circulo* c);
};

#endif
