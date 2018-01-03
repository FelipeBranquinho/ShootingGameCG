#include "circulo.h"

void Circulo::desenha(){
    	/*GLfloat materialEmission[] = { 0, 0, 0, 1.0 };
	GLfloat materialColor[] = { corR, corG, corB, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glutSolidSphere(raio,100,100);
	int i;
	GLfloat x,y;
	glBegin(GL_POLYGON);
		for ( i=0;i<360;i+=5)
		{
			x = (raio*cos(M_PI*i/180));
			y = (raio*sin(M_PI*i/180));
                        glNormal3f(0,1,0);
			glVertex3f(x,y,0);
		}
	glEnd();*/
    desenhaCilindro(0, 40, 40);
}

void Circulo::desenhaCilindro(int interno, int sl, int st){
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
    glBindTexture (GL_TEXTURE_2D, texture);
    
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
            glNormal3f(x/norm,y/norm,0);
            glTexCoord2f(a/(M_PI*2),b/h);
            glVertex3f(x,y,b);
            glNormal3f(x/norm,y/norm,0);
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
            glNormal3f(0,0,1);
            glTexCoord2f(a/(M_PI*2),b/raio);
            glVertex3f(x,y,h);

            x = cos(a) * (raio-(b+stepZ));
            y = sin(a) * (raio-(b+stepZ));
            glNormal3f(0,0,1);
            glTexCoord2f(a/(M_PI*2),(b+stepZ)/raio);
            glVertex3f(x,y, h);

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
            glTexCoord2f(a/(M_PI*2),b/raio);
            glVertex3f(x,y,0);

            x = cos(a) * (raio-(b+stepZ));
            y = sin(a) * (raio-(b+stepZ));
            glNormal3f(0,0,-1);
            glTexCoord2f(a/(M_PI*2),(b+stepZ)/raio);
            glVertex3f(x,y, 0);

            a += step;
        }
        b += stepZ;
    }
    glEnd();
    
    if(interno != 0){
        a = 0.0f;
        step = M_PI*2 / (float)numSteps;
        b = 0.0f;
        stepZ = h/(float)numStepsZ;
        glBegin(GL_TRIANGLE_STRIP);
        for(int j = 0; j < numStepsZ; ++j){
            a = 0;
            for (int i = 0; i < numSteps + 1; ++i){           
                float x = cos(a) * interno;
                float y = sin(a) * interno;
                norm = sqrt(
                            x*x+
                            y*y+
                            0);
                glNormal3f(-x/norm,-y/norm,0);
                glTexCoord2f(a/(M_PI*2),0.1*b/h);
                glVertex3f(x,y,b);
                glNormal3f(-x/norm,-y/norm,0);
                glTexCoord2f(a/(M_PI*2),0.1*(b+stepZ)/h);
                glVertex3f(x,y,(b+stepZ));
                a += step;
            }
            b += stepZ;
        }
        glEnd();
    }
}

void Circulo::desenhaCirculo(){
    glColor3f(corR,corG,corB);
    glBegin(GL_POLYGON);
            for (int i=0;i<360;i+=5)
            {
                GLfloat x = (raio*cos(M_PI*i/180));
                GLfloat y = (raio*sin(M_PI*i/180));
                glVertex2f(x,y);
            }
    glEnd();
}

void Circulo::desenhaCircunferencia(){
    glColor3f(corR,corG,corB);
    glBegin(GL_LINES);
            for (int i=0;i<360;i+=5)
            {
                GLfloat x = (raio*cos(M_PI*i/180));
                GLfloat y = (raio*sin(M_PI*i/180));
                glVertex2f(x,y);
            }
    glEnd();
}

void Circulo::desenhaEsfera(){
    GLfloat materialEmission[] = { 0, 0, 0, 1};
    GLfloat materialColorA[] = { 0.6, 0.6, 0.6, 1};
    GLfloat materialColorD[] = { 0.6, 0.6, 0.6, 1};
    GLfloat mat_specular[] = { 0.6, 0.6, 0.6, 1};
    GLfloat mat_shininess[] = { 100.0 };
 
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glColor3f(1,0,0);
    glPushMatrix();
		glutSolidSphere(raio,20,20);
    glPopMatrix();
}

float distancia(float x0,float y0,float x,float y){
	return (sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y)));
}

int Circulo::fora(Circulo* c){
  return (distancia(this->cx,this->cy, c->cx,c->cy) > (c->raio - this->raio));
}

int Circulo::dentro(Circulo* c){
  return (distancia(this->cx,this->cy, c->cx,c->cy) <= (c->raio + this->raio));
}

int Circulo::colide(Circulo* c){
	return (distancia(this->cx,this->cy, c->cx,c->cy) <= (c->raio + this->raio) && (
                (this->cz >= c->cz && this->cz <= (c->cz+c->h)) ||
                (c->cz >= this->cz && (c->cz <= (this->cz+this->h)))
                ));
}
