/* 
 * File:   jogador.h
 * Author: Gabriel Giorisatto De Angelo
 *
 * Created on 30 de Outubro de 2017, 15:35
 */

#ifndef JOGADOR_H
#define JOGADOR_H
#include <iostream>
#include <string>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "circulo.h"
#include "tiro.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <valarray>
#include "imageloader.h"
#include "objloader.h"

using namespace std;
class Arena;

class Jogador{
public:
    vector<mesh*> m;
    mesh arma;
    GLint frame;
    GLfloat vx,vy;          //Direcionamento do jogador (vetor direcao)
    GLfloat theta_braco,theta_braco_z;    //Angulo do braco
    GLfloat desloc_perna;   //Deslocamento das pernas
    GLfloat velocidade;     //Velocidade do jogador
    GLfloat vel_tiro;
    void desenhaPernas();
    void desenhaCorpo();
    void desenhaCabeca();
    void desenhaOmbros();
    void desenhaArma();
    void DesenhaRect(GLfloat height, GLfloat width,
                    GLfloat R, GLfloat G, GLfloat B);
    vector<Tiro*>vtiro;
    Circulo corpo;          //Posicao do jogador (centro do corpo)
    GLfloat tamanho;        //Tamanho do jogador
    GLfloat altura;
    GLfloat caindo,flag_caindo;
    GLint pulando,emcima;
    GLfloat raio_original;
    struct timeval start, end;
    long mtime, seconds, useconds;
    struct timeval tstart, tend;
    long tmtime, tseconds, tuseconds;
    GLuint textiro;
    Jogador(GLfloat cx, GLfloat cy,GLfloat cz,GLfloat h,GLfloat raio, GLfloat corR, GLfloat corG,
            GLfloat corB, GLint id, GLfloat vel, GLfloat velt, GLuint tex, GLuint textiro,vector<mesh*> m, mesh arma){
		corpo = Circulo(cx,cy,cz,h,raio,corR,corG,corB,id,tex);
                vx = 0;
                vy = 1;
                theta_braco = 0;
                theta_braco_z = 0;
                desloc_perna = 0;
                tamanho = 1;
                velocidade = vel;
                vel_tiro = velt;
                pulando = 0;
                emcima = 0;
                raio_original = raio;
                altura = 1;
                caindo = 0;
                frame = 0;
                this->textiro = textiro;
                this->m = m;
                this->arma = arma;
	}
    Jogador(){
		corpo = Circulo(0,0,0,0,0,0,0,0,0,0);
	}
    void desenhaJogador();
    void desenhaTiro();
    void moveJogador(GLint direcao, GLdouble timeDiference);
    int moveNaArena(GLint direcao, GLdouble timeDiference, Arena arena, vector<Jogador*> inimigos, Jogador jogador, GLint id);
    void giraJogador(GLint sentido, GLdouble timeDiference);
    int moveBraco(GLint sentido);
    int moveBracoZ(GLint sentido);
    int colide(Circulo* c);
    int colidePersonagens(Jogador jogador, vector<Jogador*> inimigos, GLint id);
    int fora(Circulo* c);
    int dentro(Circulo* c);
    int atira();
    void moveTiros(GLdouble timeDiference);
    void deletaTiros(Arena arena);
    void atualizaTiros(GLdouble timeDiference, Arena arena);
    int acertou(Jogador inimigo);
    void verificaPulo(Arena arena, float MAX_ALTURA);
};

#include "arena.h"
#endif /* JOGADOR_H */
