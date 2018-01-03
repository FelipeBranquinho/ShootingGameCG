#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#include "ImageRaw.h"
#include "imageloader.h"
#include "objloader.h"

mesh m;

using namespace std;


void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  /*
  int i;
  for(i = -50;i < 50;i = i + 10){
  glPushMatrix();
  glTranslatef(i,0,0);
  glutSolidCube(5);
  glPopMatrix();
  }
  */
  m.draw();
  glutSwapBuffers();
  
}


void init(void){
  GLfloat matEmission[] = {0.00,0.00,0.00,1};
  GLfloat matColor[] = {1.0,0.0,0.0,1};
  GLfloat matShininess[] = {0.0,1.0,1.0,1};
  GLfloat matSpecular[] = {50.0};
  GLfloat lightPos[] = {-1.0,-1.0,-1.0,0.0};
  
  glClearColor(1.0,1.0,1.0,0.0);
  
  glShadeModel(GL_FLAT);
  m.loadMesh("Cadeira.obj");
  m.loadMeshTex(LoadTextureRAW("Posto.bmp"));
  
  glMaterialfv(GL_FRONT,GL_EMISSION,matEmission);
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,matColor);
  glMaterialfv(GL_FRONT,GL_SPECULAR,matSpecular);
  glMaterialfv(GL_FRONT,GL_SHININESS,matShininess);
  glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH);
  
  glMatrixMode(GL_PROJECTION);
  gluPerspective(60,1,1,98);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(10,10,10,0,0,0,0,1,0);
}


int main(int argc,char** argv){
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutCreateWindow("object");
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;
}
