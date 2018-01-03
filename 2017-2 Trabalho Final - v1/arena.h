#ifndef ARENA_H
#define ARENA_H
#include <iostream>
#include <string>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "tinyxml.h"
#include "tinystr.h"
#include "circulo.h"
#include "jogador.h"
using namespace std;

class Arena
{
private:
	void loadXml(const char* pFilename);
	void loadSvg(const char* pFilename);
public:
	vector<Circulo*>baixo;
        GLfloat obs_altura;
	Circulo arenaAzul,arenaBranco;
	GLfloat xa,ya;
	void desenhaArena();
        void desenhaCeu();
	int verificaColisao(Jogador jogador);
        void deletaTiros(Jogador jogador);
	int emcimaObs(Jogador jogador);
	int foraArena(Jogador jogador);
};

#endif
