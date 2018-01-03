#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <math.h>
#include <cstring>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <valarray>
#include "arena.h"
#include "circulo.h"
#include "imageloader.h"
#include "ImageRaw.h"
#include "objloader.h"

using namespace std;

//-----------Gabriel Giorisatto De Angelo - 2014100628------------------------//
//------------------TC3 - Computacao Grafica----------------------------------//
//------------------------UFES - 2017/2---------------------------------------//

//Variaveis globais
vector<mesh*> m;
mesh arma;
Arena arena;
Jogador jogador;
vector<Jogador*>inimigos;

//Key status
int keyStatus[256];

int ax = 0;
int ay = 0;
float velo_tiro = 0, velo_joga = 0;
float velo_tiro_ini = 0, velo_ini = 0, freq_tiro = 0;
#define MAX_ALTURA 40
int aux_tiro = 1;
int aux_braco = 0;

int score = 0;
int perdeu = 0;

int zoom = 150;

//Camera controls
int cameraToggle = 2;
double camDist=50;
double camXYAngle=0;
double camXZAngle=65;
int lastX = 0;
int lastY = 0;
int buttonDown=0;
int contador = 0;

// Text variable
static char str[2000];
void * font = GLUT_BITMAP_9_BY_15;

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b)
{
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(r,g,b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
}

void PrintText(GLfloat x, GLfloat y, const char * text, double r, double g, double b)
{
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, 1, 0, 1, -1, 1);
        RasterChars(x, y, 1, text, r, g, b);    
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

void init ()
{
    //Limpa a cor com Branco
    glClearColor (1.0, 1.0, 1.0, 0.0);
    
    //Initialize keyStatus
    for(int i = 0; i < 256; i++)
        keyStatus[i] = 0;
    
    glShadeModel (GL_SMOOTH);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    m.push_back(new mesh);
    //m.at(0)->loadMesh("Blender/untitledc.obj");
    //m.at(0)->loadMeshTex(LoadTextureRAW("parede.bmp"));
    for(int i = 1; i < 16; i++){
        m.push_back(new mesh);
        sprintf(str, "Blender/untitledc%d.obj", i );
        m.at(i-1)->loadMesh(str);
        m.at(i-1)->loadMeshTex(LoadTextureRAW("earth.bmp"));
    }
    arma.loadMesh("Blender/armanova.obj");
    arma.loadMeshTex(LoadTextureRAW("earth.bmp"));
}

void verificaMorte(){
    for(int i = 0; i< (int)inimigos.size(); i++){
        if(jogador.acertou((*inimigos.at(i)))){
            inimigos.erase(inimigos.begin()+i);
            score++;
        }
        else if((*inimigos.at(i)).acertou(jogador)){
            perdeu = 1;
        }
    }
}

void PrintScore(GLfloat x, GLfloat y)
{
    sprintf(str, "Score: %d", score );
    PrintText(x, y, "Score", 1, 0, 0);
}

void PrintDerrota(){
    sprintf(str, "DERROTA");
    PrintText(0.5, 0.5, str, 1, 0, 0);
}
void PrintVitoria(){
    sprintf(str, "VITORIA");
    //Define the position to start printing
    PrintText(0.5, 0.5, str, 0, 1, 0);

}
void DrawObj(double size)
{
	GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0 };
	GLfloat materialColor[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glColor3f(1,0,0);
        glPushMatrix();
	glutSolidCube(size);
        glPopMatrix();
}
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      gluPerspective (45, (GLfloat)h/(GLfloat)w, 1, 1000);
   else
      gluPerspective (45, (GLfloat)w/(GLfloat)h, 1, 1000);
   glMatrixMode(GL_MODELVIEW);
}

void lanterna(){
    glPushMatrix();
    glTranslatef(jogador.corpo.cx,jogador.corpo.cy,jogador.corpo.cz);
    glRotatef((atan2(jogador.vy,jogador.vx)*180/M_PI)-90,0,0,1);
    glTranslatef(0.5*jogador.corpo.raio,jogador.corpo.raio,jogador.corpo.h*3/4);
    glRotatef(jogador.theta_braco,0,0,1);
    glRotatef(jogador.theta_braco_z,1,0,0);
    glTranslatef(0,0.75*jogador.corpo.raio,0);
    GLfloat light_ambient[] = { .4, .4, .4, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position2[] = { 0.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT1,GL_POSITION,light_position2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);
    GLfloat spot_direction[] = { 0.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    glPopMatrix();
}

void displayOlho(){
    glViewport(0,500,500,200);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(  jogador.corpo.cx+jogador.corpo.raio*jogador.vx,
                jogador.corpo.cy+jogador.corpo.raio*jogador.vy,
                jogador.corpo.cz+jogador.corpo.h,
                jogador.corpo.cx+jogador.corpo.raio*jogador.vx+jogador.vx,
                jogador.corpo.cy+jogador.corpo.raio*jogador.vy+jogador.vy,
                jogador.corpo.cz+jogador.corpo.h,
                0,0,1);
    glPushMatrix();
    glTranslatef(arena.arenaAzul.cx,arena.arenaAzul.cy,80);
    GLfloat light_position[] = {0, 0, 0, 1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glPopMatrix();
    lanterna();
    arena.desenhaArena();
    if(!perdeu){
              jogador.desenhaJogador();
              jogador.desenhaTiro();
    }
    for (std::vector<Jogador*>::iterator it = inimigos.begin() ; it != inimigos.end(); ++it){
              (*it)->desenhaJogador();
              (*it)->desenhaTiro();
    }
    glPopMatrix();
}

void displayTerceiraPessoa(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(  jogador.corpo.cx - zoom*sin(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)+atan2(jogador.vy,jogador.vx)),
                jogador.corpo.cy - zoom*sin(camXZAngle*M_PI/180)*sin((camXYAngle*M_PI/180)+atan2(jogador.vy,jogador.vx)),
                jogador.corpo.cz + zoom*cos(camXZAngle*M_PI/180),
                jogador.corpo.cx,
                jogador.corpo.cy,
                jogador.corpo.cz,
                0,0,1);
    glPushMatrix();
    glTranslatef(arena.arenaAzul.cx,arena.arenaAzul.cy,80);
    GLfloat light_position[] = {0, 0, 0, 1};
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glPopMatrix();
    lanterna();
    arena.desenhaArena();
    if(!perdeu){
              jogador.desenhaJogador();
              jogador.desenhaTiro();
    }
    for (std::vector<Jogador*>::iterator it = inimigos.begin() ; it != inimigos.end(); ++it){
              (*it)->desenhaJogador();
              (*it)->desenhaTiro();
    }
    verificaMorte();
    glPopMatrix();
}

void displayPrimeiraPessoa(){
    PrintText(0.5, 0.4, "+", 0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Sistema de coordenadas da arma (centralizado na 'mao' do personagem)
    float xcxi = 0;
    float xcxli = 0;
    
    float ycyi = 0;
    float ycyli = 1;
    
    float zczi = jogador.corpo.raio*0.5;
    float zczli = jogador.corpo.raio*0.5;
    
    //Rotacao de theta (rad) em torno de X
    float thetar = jogador.theta_braco_z*M_PI/180;
    
    float xcxr = xcxi;
    float ycyr = ycyi*cos(thetar)-zczi*sin(thetar);
    float zczr = ycyi*sin(thetar)+zczi*cos(thetar);
    
    float xcxlr = xcxli;
    float ycylr = ycyli*cos(thetar)-zczli*sin(thetar);
    float zczlr = ycyli*sin(thetar)+zczli*cos(thetar);
    
    //Rotacao de theta (rad) em torno de Z
    thetar = (jogador.theta_braco*M_PI/180) + ((atan2(jogador.vy,jogador.vx)*180/M_PI)-90)*M_PI/180;
    
    float xcx = xcxr*cos(thetar)-ycyr*sin(thetar);
    float ycy = xcxr*sin(thetar)+ycyr*cos(thetar);
    float zcz = zczr;
    
    float xcxl = xcxlr*cos(thetar)-ycylr*sin(thetar);
    float ycyl = xcxlr*sin(thetar)+ycylr*cos(thetar);
    float zczl = zczlr;
    
    // Vetor Look
    xcxl -= xcx;
    ycyl -= ycy;
    zczl -= zcz;
    
    // Sistema de coordenadas do mundo
    xcx += jogador.corpo.cx +
            1.12*jogador.corpo.raio*cos(((atan2(jogador.vy,jogador.vx)*180/M_PI)-26.565)*M_PI/180);
    ycy += jogador.corpo.cy +
            1.12*jogador.corpo.raio*sin(((atan2(jogador.vy,jogador.vx)*180/M_PI)-26.565)*M_PI/180);
    zcz += jogador.corpo.cz + jogador.corpo.h*3/4;
    
    float norm = sqrt( xcxl*xcxl+ycyl*ycyl+zczl*zczl);
    xcxl /= norm;
    ycyl /= norm;
    zczl /= norm;
    gluLookAt(  xcx, ycy, zcz,
                xcx+xcxl, ycy+ycyl, zcz+zczl,
                0, 0, 1 );
    glPushMatrix();
    glTranslatef(arena.arenaAzul.cx,arena.arenaAzul.cy,80);
    GLfloat light_position[] = {0, 0, 0, 1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glPopMatrix();
    lanterna();
    arena.desenhaArena();
    if(!perdeu){
              jogador.desenhaJogador();
              jogador.desenhaTiro();
    }
    for (std::vector<Jogador*>::iterator it = inimigos.begin() ; it != inimigos.end(); ++it){
              (*it)->desenhaJogador();
              (*it)->desenhaTiro();
    }
    verificaMorte();
    glPopMatrix();
}

void displayMinimap(){
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho(arena.arenaAzul.cx-arena.arenaAzul.raio,
            arena.arenaAzul.cx+arena.arenaAzul.raio,
            arena.arenaAzul.cy-arena.arenaAzul.raio,
            arena.arenaAzul.cy+arena.arenaAzul.raio,
            -1.0, 1.0);
        //Push to recover original attributes
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        
        glTranslatef((arena.arenaAzul.cx+arena.arenaAzul.raio)*3/4,0,1);
        glScalef(0.25,0.25,1);
        
        glPushMatrix();
        glTranslatef(arena.arenaAzul.cx,arena.arenaAzul.cy,0);
        arena.arenaAzul.desenhaCircunferencia();
        arena.arenaBranco.desenhaCircunferencia();
        glPopMatrix();
        
        if(!perdeu){
            glPushMatrix();
            glTranslatef(jogador.corpo.cx,jogador.corpo.cy,0);
            jogador.corpo.desenhaCirculo();
            glRotatef((atan2(jogador.vy,jogador.vx)*180/M_PI)-90,0,0,1);
            glPushMatrix();
            glRotatef(-45,0,0,1);
            glBegin(GL_LINES);
            glColor3f(1,1,1);
            glVertex2f(0,0);
            glVertex2f(0,jogador.corpo.raio*4);
            glEnd();
            glPopMatrix();
            glPushMatrix();
            glRotatef(45,0,0,1);
            glBegin(GL_LINES);
            glColor3f(1,1,1);
            glVertex2f(0,0);
            glVertex2f(0,jogador.corpo.raio*4);
            glEnd();
            glPopMatrix();
            glPopMatrix();
            for (std::vector<Tiro*>::iterator it = jogador.vtiro.begin() ; it != jogador.vtiro.end(); ++it){
                  glPushMatrix();
                  glTranslatef((*it)->cx,(*it)->cy,0);
                  (*it)->desenhaCirculo();
                  glPopMatrix();
            }
        }
        
        for (std::vector<Jogador*>::iterator it = inimigos.begin() ; it != inimigos.end(); ++it){
            glPushMatrix();
            glTranslatef((*it)->corpo.cx,(*it)->corpo.cy,0);
            (*it)->corpo.desenhaCirculo();
            glPopMatrix();
            for (std::vector<Tiro*>::iterator it2 = (*it)->vtiro.begin() ; it2 != (*it)->vtiro.end(); ++it2){
                  glPushMatrix();
                  glTranslatef((*it2)->cx,(*it2)->cy,0);
                  (*it2)->desenhaCirculo();
                  glPopMatrix();
            }
        }
        
        for (std::vector<Circulo*>::iterator it = arena.baixo.begin() ; it != arena.baixo.end(); ++it){
            glPushMatrix();
            glTranslatef((*it)->cx,(*it)->cy,0);
            (*it)->desenhaCirculo();
            glPopMatrix();
	}
        
    glPopAttrib();
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

void display(void)
{
	/* Limpar todos os pixels */
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        displayOlho();
        glViewport(0,0,500,500);
        sprintf(str, "Score: %d", score );
        PrintText(0.8, 0.9, str, 0,1,0);
        if(perdeu)
            PrintText(0.4, 0.5, "DERROTA!", 1,0,0);
	else if(inimigos.size()==0)
            PrintText(0.4, 0.5, "VITORIA!", 0,1,0);
        displayMinimap();
        if (cameraToggle == 1){
            displayPrimeiraPessoa();
        }
        else if (cameraToggle == 2){
            displayTerceiraPessoa();
        }
	glFlush ();
}
void keyPress(unsigned char key, int x, int y)
{
	switch(key){
        case 'w':
        case 'W':
             keyStatus[(int)('w')] = 1;
             break;
        case 's':
        case 'S':
             keyStatus[(int)('s')] = 1;
             break;
        case 'a':
        case 'A':
             keyStatus[(int)('a')] = 1;
             break;
        case 'd':
        case 'D':
             keyStatus[(int)('d')] = 1;
             break;
        case 'p':
        case 'P':
             keyStatus[(int)('p')] = 1;
             break;
        case 27 :
             exit(0);
	}
}

void keyRelease(unsigned char key, int x, int y)
{
	switch(key){
            case 'w':
            case 'W':
                 keyStatus[(int)('w')] = 0;
                 break;
            case 's':
            case 'S':
                 keyStatus[(int)('s')] = 0;
                 break;
            case 'a':
            case 'A':
                 keyStatus[(int)('a')] = 0;
                 break;
            case 'd':
            case 'D':
                 keyStatus[(int)('d')] = 0;
                 break;
            case 'p':
            case 'P':
                 keyStatus[(int)('p')] = 0;
                 break;
            case 'n':
            case 'N':
                 if(!keyStatus[(int)('n')]){
                     keyStatus[(int)('n')] = 1;
                     glDisable(GL_LIGHT0);
                     glEnable(GL_LIGHT1);
                 }
                 else{
                     keyStatus[(int)('n')] = 0;
                     glEnable(GL_LIGHT0);
                     glDisable(GL_LIGHT1);
                 }
                 break;
            case '1':
                cameraToggle = 1;
                break;
            case '2':
                cameraToggle = 2;
                break;
            default:
                 break;
	}
}

void motion(int x, int y){
    if(ax < x)
        jogador.moveBraco(-1);
    else if(ax > x)
        jogador.moveBraco(1);
    ax = x;
    if(ay < y)
        jogador.moveBracoZ(-1);
    else if(ay > y)
        jogador.moveBracoZ(1);
    ay = y;
}

void mouse(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        jogador.atira();
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        lastX = x;
        lastY = y;
        buttonDown = 1;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        buttonDown = 0;
        camXYAngle = 0;
        camXZAngle = 65;
    }
    if (button == 3 && state){
        zoom--;
    }
    if (button == 4 && state){
        zoom++;
    }
    glutPostRedisplay();
}

void mouse_motion(int x, int y)
{
    if (!buttonDown)
        return;
    
    camXYAngle -= x - lastX;
    camXZAngle -= y - lastY;
    if(camXYAngle >= 180)
        camXYAngle = 180;
    else if(camXYAngle <= -180)
        camXYAngle = -180;
    if(camXZAngle >= 179)
        camXZAngle = 179;
    else if(camXZAngle <= 1)
        camXZAngle = 1;
    
    //camXYAngle = (int)camXYAngle % 360;
    //camXZAngle = (int)camXZAngle % 360;
    
    lastX = x;
    lastY = y;
    glutPostRedisplay();
}


void atualizaJogador(GLdouble timeDiference){
    //MoveJogador
    if(keyStatus[(int)('a')])
        jogador.giraJogador(1,timeDiference);
    if(keyStatus[(int)('d')])
        jogador.giraJogador(-1,timeDiference);
    if(keyStatus[(int)('s')])
        jogador.moveNaArena(-1,timeDiference,arena,inimigos,jogador,-1);
    if(keyStatus[(int)('w')])
        jogador.moveNaArena(1,timeDiference,arena,inimigos,jogador,-1);
    //Pulo
    if(keyStatus[(int)('p')] && (!jogador.pulando)){
        //Comeca pulo
        gettimeofday(&jogador.start, NULL);
        jogador.pulando = 1;
    }
    jogador.verificaPulo(arena, MAX_ALTURA);
}

void atualizaInimigo(GLdouble timeDiference, Jogador* inimigo,GLint id){
    if(aux_tiro){
        aux_tiro = 0;
        gettimeofday(&inimigo->tstart, NULL);
    }
    gettimeofday(&inimigo->tend, NULL);
    inimigo->tseconds  = inimigo->tend.tv_sec  - inimigo->tstart.tv_sec;
    inimigo->tuseconds = inimigo->tend.tv_usec - inimigo->tstart.tv_usec;
    inimigo->tmtime = ((inimigo->tseconds) * 1000 +inimigo->tuseconds/1000.0) + 0.5;
    if(inimigo->tmtime > (1/freq_tiro)*1000){
        if(inimigo->atira())
            gettimeofday(&inimigo->tstart, NULL);
    }
    if(aux_braco){
        if(!inimigo->moveBraco(1))
            aux_braco = 0;
    }
    else{
        if(!inimigo->moveBraco(-1))
            aux_braco = 1;
    }
    //MoveJogador
    int pula = 0;
    if(!inimigo->pulando && !inimigo->emcima){
        inimigo->moveJogador(1, timeDiference);
        if(arena.verificaColisao(*inimigo))
                pula = 1;
        inimigo->moveJogador(-1, timeDiference);
    }
    int aux_ia = inimigo->moveNaArena(1,timeDiference,arena,inimigos,jogador,id);
    if(!aux_ia){
        inimigo->moveJogador(1, timeDiference);
        if(!inimigo->pulando || arena.foraArena(*inimigo)){
            inimigo->moveNaArena(-1,timeDiference,arena,inimigos,jogador,id);
            inimigo->giraJogador(1,timeDiference);
            inimigo->moveNaArena(1,timeDiference,arena,inimigos,jogador,id);
        }
        else
            inimigo->moveNaArena(-1,timeDiference,arena,inimigos,jogador,id);
    }
    //Pulo
    if(pula && (!inimigo->pulando || arena.emcimaObs(*inimigo))){
        //Comeca pulo
        gettimeofday(&inimigo->start, NULL);
        inimigo->pulando = 1;
    }
    inimigo->verificaPulo(arena, MAX_ALTURA);
    
}
void idleFunc(void)
{
    static GLdouble previousTime = 0;
    GLdouble currentTime;
    GLdouble timeDiference;
    
    // Elapsed time from the initiation of the game.
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeDiference = currentTime - previousTime; // Elapsed time from the previous frame.
    //timeDiference = 1; // Elapsed time from the previous frame.
    previousTime = currentTime; //Update previous time
    if(!perdeu){
    atualizaJogador(timeDiference);
    jogador.atualizaTiros(timeDiference,arena);
    }
    for(int i = 0; i< (int)inimigos.size(); i++){
        atualizaInimigo(timeDiference,(inimigos.at(i)),i);
        inimigos.at(i)->atualizaTiros(timeDiference,arena);
    }
	
    glutPostRedisplay();
}
/*
GLuint LoadTextureRAW( const char * filename )
{

    GLuint texture;
    
    Image* image = loadBMP(filename);

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             image->width, image->height,  //Width and height
                             0,                            //The border of the image
                             GL_RGB, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image->pixels);               //The actual pixel data
    delete image;

    return texture;
}*/

void loadSvg(const char* pFilename)
{
	printf("%s",pFilename);
	TiXmlDocument doc(pFilename);
	if (!doc.LoadFile()){
		printf("Falha na leitura do arquivo svg\n");	
		return;
	}
	TiXmlNode* pRoot = doc.FirstChildElement("svg");
	TiXmlElement* objectTree = pRoot->FirstChildElement("circle");
	for(TiXmlElement*obj = objectTree; obj != NULL; obj = obj-> NextSiblingElement()) {
		float cx=0, cy=0, r=0;
		string fill, name;
		int id;

		obj->QueryFloatAttribute("cx", &cx);
		obj->QueryFloatAttribute("cy", &cy);
		obj->QueryFloatAttribute("r", &r);
		fill = obj->Attribute("fill");
		id = atoi(obj->Attribute("id"));

		if (!fill.compare("blue")){
		    arena.arenaAzul = Circulo(cx,cy,0,0,r,0,0,1,id,LoadTextureRAW( "parede.bmp" ));
				arena.ya = cy+r;
		}
		else if (!fill.compare("white")){
		    arena.arenaBranco = Circulo(cx,cy,0,0,r,0,0,1,id,0);
		}
		else if (!fill.compare("red")){
				inimigos.push_back(new Jogador(cx,cy,0,2*r,r,1,0,0,id,velo_ini,velo_tiro_ini,0,LoadTextureRAW( "sun1.bmp" ),m,arma));
		}
		else if (!fill.compare("green")){
				jogador = Jogador(cx,cy,0,2*r,r,0,1,0,id,velo_joga,velo_tiro,0,LoadTextureRAW( "sun1.bmp" ),m,arma);
		}
		else if (!fill.compare("black")){
		    arena.baixo.push_back(new Circulo(cx,cy,0,arena.obs_altura*40,r,0,0,0,id,LoadTextureRAW( "stone2.bmp" )));

		}
	}
        arena.arenaAzul.h = jogador.corpo.h*4;
        arena.arenaAzul.cz = -arena.arenaAzul.h;
	jogador.corpo.cy = arena.ya - jogador.corpo.cy;
	arena.arenaAzul.cy = arena.ya - arena.arenaAzul.cy;
	arena.arenaBranco.cy = arena.ya - arena.arenaBranco.cy;
	for (std::vector<Circulo*>::iterator it = arena.baixo.begin() ; it != arena.baixo.end(); ++it){
		(*it)->cy = arena.ya - (*it)->cy;
	}
	for (std::vector<Jogador*>::iterator it = inimigos.begin() ; it != inimigos.end(); ++it){
		(*it)->corpo.cy = arena.ya - (*it)->corpo.cy;
	}
	return;
}

void loadXml(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	if (!doc.LoadFile()){
		printf("Falha na leitura do arquivo xml\n");	
		return;
	}
	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pEle = pRoot->FirstChildElement("arquivoDaArena");
	string arqSvg = string(pEle->Attribute("caminho")) + string(pEle->Attribute("nome")) + "." + string(pEle->Attribute("tipo"));
        pEle = pRoot->FirstChildElement("jogador");
        pEle->QueryFloatAttribute("velTiro",&velo_tiro);
        pEle->QueryFloatAttribute("vel",&velo_joga);
        pEle = pRoot->FirstChildElement("inimigo");
        pEle->QueryFloatAttribute("freqTiro",&freq_tiro);
        pEle->QueryFloatAttribute("vel",&velo_ini);
        pEle->QueryFloatAttribute("velTiro",&velo_tiro_ini);
        pEle = pRoot->FirstChildElement("obstaculo");
        pEle->QueryFloatAttribute("altura",&arena.obs_altura);
        arena.obs_altura = arena.obs_altura/100;
	loadSvg(arqSvg.c_str());
	return;
}

void load(const char* pFilename)
{
	loadXml(pFilename);
	return;
}

int main(int argc, char** argv)
{
	if(argc != 2){
		printf("Informe a localizacao do arquivo xml\n");
		return 0;
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500,700);
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("janela");
	init();
        load(strcat(argv[1],"config.xml")); //Leitura da arena
	glutIdleFunc(idleFunc);
	glutDisplayFunc(display);
        glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutPassiveMotionFunc(motion);
        glutMotionFunc(mouse_motion);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
