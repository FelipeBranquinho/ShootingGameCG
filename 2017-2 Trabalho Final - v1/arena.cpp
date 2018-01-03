#include "arena.h"

using namespace std;


void Arena::desenhaCeu(){
    GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
    GLfloat materialColorA[] = { 1, 1, 1, 1};
    GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 100.0 };
 
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glBindTexture (GL_TEXTURE_2D, arenaAzul.texture);
    
    GLfloat raio = arenaAzul.raio;
    GLfloat h = arenaAzul.h;
    int interno = 0;
    float a = 0.0f;
    int numSteps = 100;
    float step = M_PI*2 / (float)numSteps;
    int numStepsZ = 40;
    float b = 0.0f;
    float stepZ = h/(float)numStepsZ;
    float norm;

    glBegin(GL_TRIANGLE_STRIP);
    for(int j = 0; j < numStepsZ; ++j){
        a = 0;
        for (int i = 0; i < numSteps + 1; ++i){           
            float x = cos(a) * raio;
            float y = sin(a) * raio;
            norm = sqrt(
                        x*x+
                        y*y+
                        0);
            glNormal3f(-x/norm,-y/norm,0);
            glTexCoord2f(a/(M_PI*2),b/h);
            glVertex3f(x,y,b);
            glNormal3f(-x/norm,-y/norm,0);
            glTexCoord2f(a/(M_PI*2),(b+stepZ)/h);
            glVertex3f(x,y,(b+stepZ));
            a += step;
        }
        b += stepZ;
    }
    glEnd();
    
    a = 0.0f;
    b = 0.0f;
    stepZ = (raio-interno)/(float)numStepsZ;

    glBegin(GL_TRIANGLE_STRIP);
    for(int j = 0; j < numStepsZ; ++j){
        a = 0;
        for (int i = 0; i < numSteps + 1; ++i)
        {
            float x = cos(a) * (raio-b);
            float y = sin(a) * (raio-b);
            glNormal3f(0,0,-1);
            glTexCoord2f(a/(M_PI*2),0.8+0.2*b/raio);
            glVertex3f(x,y,h);

            x = cos(a) * (raio-(b+stepZ));
            y = sin(a) * (raio-(b+stepZ));
            glNormal3f(0,0,-1);
            glTexCoord2f(a/(M_PI*2),0.8+0.2*(b+stepZ)/raio);
            glVertex3f(x,y, h);

            a += step;
        }
        b += stepZ;
    }
    glEnd();
}

void Arena::desenhaArena(){
        glPushMatrix();
        glTranslatef(arenaAzul.cx,arenaAzul.cy,-arenaAzul.h);
        arenaAzul.desenhaCilindro(arenaBranco.raio, 40, 40);
        glTranslatef(0,0,arenaAzul.h);
        desenhaCeu();
        glPopMatrix();
	for (std::vector<Circulo*>::iterator it = baixo.begin() ; it != baixo.end(); ++it){
            glPushMatrix();
            glTranslatef((*it)->cx,(*it)->cy,0);
            (*it)->desenhaCilindro(0,40,40);
            glPopMatrix();
	}
	return;
}

int Arena::verificaColisao(Jogador jogador){
	for (std::vector<Circulo*>::iterator it = baixo.begin() ; it != baixo.end(); ++it){
		if(jogador.colide((*it)) && !emcimaObs(jogador)) return 1;
	}
	return 0;
}

int Arena::emcimaObs(Jogador jogador){
	for (std::vector<Circulo*>::iterator it = baixo.begin() ; it != baixo.end(); ++it){
            if(jogador.colide((*it)) && (jogador.corpo.cz >= ((*it)->cz+0.9*(*it)->h))){
                return 1;
            }
	}
	return 0;
}
int Arena::foraArena(Jogador jogador){
	return (jogador.fora(&arenaAzul) || jogador.dentro(&arenaBranco));
}
