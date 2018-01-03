/* 
 * File:   jogador.cpp
 * Author: Gabriel Giorisatto De Angelo
 * 
 * Created on 30 de Outubro de 2017, 15:35
 */
#include "jogador.h"

void Jogador::DesenhaRect(GLfloat height, GLfloat width,
			GLfloat R, GLfloat G, GLfloat B)
{
	glColor3f(R,G,B);

	glBegin(GL_QUADS);
		glVertex2f( -width/2,0);
		glVertex2f( -width/2,height);
                glVertex2f( width/2,height);
		glVertex2f( width/2,0);
	glEnd();
}

void Jogador::desenhaPernas(){
    GLfloat materialEmission[] = { 0, 0, 0, 1.0 };
    GLfloat materialColor[] = { corpo.corR, corpo.corG, corpo.corB, 1.0 };
    GLfloat mat_specular[] = { 0, 0, 0, 0 };
    GLfloat mat_shininess[] = { 50 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glPushMatrix();
    //Perna direita
    glTranslatef(0.5*corpo.raio,0,corpo.h/3);
    glRotatef(180,1,0,0);
    glRotatef(45*sin(M_PI*desloc_perna/180),1,0,0);
    glTranslatef(0,0,corpo.h/12);
    glPushMatrix();
    glScalef(0.6,0.8,1);
    glutSolidCube(corpo.h/6);
    glPopMatrix();
    glTranslatef(0,0,corpo.h/12);
    glRotatef((45*sin(M_PI*desloc_perna/180)-45)/2,1,0,0);
    glTranslatef(0,0,corpo.h/12);
    glScalef(0.6,0.8,1);
    glutSolidCube(corpo.h/6);
    glPopMatrix();
    //Perna esquerda
    glPushMatrix();
    glTranslatef(-0.5*corpo.raio,0,corpo.h/3);
    glRotatef(180,1,0,0);
    glRotatef(-45*sin(M_PI*desloc_perna/180),1,0,0);
    glTranslatef(0,0,corpo.h/12);
    glPushMatrix();
    glScalef(0.6,0.8,1);
    glutSolidCube(corpo.h/6);
    glPopMatrix();
    glTranslatef(0,0,corpo.h/12);
    glRotatef((-45*sin(M_PI*desloc_perna/180)-45)/2,1,0,0);
    glTranslatef(0,0,corpo.h/12);
    glScalef(0.6,0.8,1);
    glutSolidCube(corpo.h/6);
    glPopMatrix();
    
}
void Jogador::desenhaCorpo(){
    glPushMatrix();
    glTranslatef(0,0,corpo.h/3);
    glScalef(1,1,0.33);
    corpo.desenha();
    glPopMatrix();
}
void Jogador::desenhaCabeca(){
    GLfloat materialEmission[] = { 0, 0, 0, 1.0 };
    GLfloat materialColor[] = { corpo.corR, corpo.corG, corpo.corB, 1.0 };
    GLfloat mat_specular[] = { 0, 0, 0, 0 };
    GLfloat mat_shininess[] = { 50 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glPushMatrix();
    glTranslatef(0,0,corpo.h*5/6);
    glScalef(3,3,1);
    glutSolidCube(corpo.h*1/6);
    glPopMatrix();
}

void Jogador::desenhaOmbros(){
    int i;
    GLfloat x,y;
    glColor3f(corpo.corR,corpo.corG,corpo.corB);
    glBegin(GL_POLYGON);
            for (i=0;i<360;i+=5)
            {
                    x = (corpo.raio*2*cos(M_PI*i/180));
                    y = (corpo.raio*0.5*sin(M_PI*i/180));
                    glVertex2f(x,y);
            }
    glEnd();
}
void Jogador::desenhaArma(){
    glPushMatrix();
    glTranslatef(corpo.raio*0.5,corpo.raio,corpo.h*3/4);
    glRotatef(theta_braco,0,0,1);
    glRotatef(theta_braco_z,1,0,0);
    arma.draw();
    glPopMatrix();
}
void Jogador::desenhaJogador(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(corpo.cx,corpo.cy,corpo.cz);
    glRotatef((atan2(vy,vx)*180/M_PI)-90,0,0,1);
    glPushMatrix();
		glScalef(corpo.raio/20,corpo.raio/20,corpo.raio/40);
		glRotatef(90,1,0,0);
		glRotatef(180,0,1,0);
    m.at(frame)->draw();
    glPopMatrix();
    //desenhaCorpo();
    //desenhaCabeca();
    //desenhaPernas();
    desenhaArma();
    //desenhaOmbros();
    glPopMatrix();
}
void Jogador::moveJogador(GLint direcao, GLdouble timeDiference){
    GLdouble vat = velocidade*timeDiference;
    corpo.cx += direcao*vx*vat;
    corpo.cy += direcao*vy*vat;
    //desloc_perna = fmod((desloc_perna+direcao*vat*2),360);
}
int Jogador::moveNaArena(GLint direcao, GLdouble timeDiference, Arena arena, vector<Jogador*> inimigos, Jogador jogador, GLint id){
    moveJogador(direcao, timeDiference);
    if(arena.verificaColisao(*this) || arena.foraArena(*this) || colidePersonagens(jogador,inimigos,id)){
        moveJogador(-direcao, timeDiference);
        return 0;
    }
    frame = fmod(frame+1+velocidade*timeDiference*0.1,15);
    return 1;
}
int Jogador::colidePersonagens(Jogador jogador, vector<Jogador*> inimigos, GLint id){
    for(int i = 0; i< (int)inimigos.size(); i++){
        if(i != id && colide(&inimigos.at(i)->corpo))
                return 1;
    }
    if(id != -1){
        if(colide(&jogador.corpo))
            return 1;
    }
    return 0;
}
void Jogador::giraJogador(GLint sentido, GLdouble timeDiference){
    GLfloat a_vx=vx,a_vy=vy, fat = 60;
    GLdouble vat = velocidade*timeDiference;
    vx = a_vx*cos(vat*M_PI/fat) - sentido*a_vy*sin(vat*M_PI/fat);
    vy = sentido*a_vx*sin(vat*M_PI/fat) + a_vy*cos(vat*M_PI/fat);
}
int Jogador::moveBraco(GLint sentido){
    if(theta_braco < 45 && sentido == 1){
        theta_braco+=1;
        return 1;
    }
    else if(theta_braco > -45 && sentido == -1){
        theta_braco-=1;
        return 1;
    }
    else{
        return 0;
    }
}
int Jogador::moveBracoZ(GLint sentido){
    if(theta_braco_z < 45 && sentido == 1){
        theta_braco_z+=1;
        return 1;
    }
    else if(theta_braco_z > -45 && sentido == -1){
        theta_braco_z-=1;
        return 1;
    }
    else{
        return 0;
    }
}
int Jogador::colide(Circulo* c){
    return corpo.colide(c);
}
int Jogador::fora(Circulo* c){
    return corpo.fora(c);
}
int Jogador::dentro(Circulo* c){
    return corpo.dentro(c);
}
int Jogador::atira(){
    if(pulando || caindo)
        return 0;
    GLfloat x,y,z,raio = 0.1*corpo.raio,id = vtiro.size(),angulo;
    GLfloat angulo_aux = (atan2(vy,vx)*180/M_PI);
    x = corpo.cx + 1.12*corpo.raio*cos((angulo_aux-26.565)*M_PI/180) +1.5*corpo.raio*cos((theta_braco_z)*M_PI/180)*cos((theta_braco+angulo_aux)*M_PI/180);
    y = corpo.cy + 1.12*corpo.raio*sin((angulo_aux-26.565)*M_PI/180) + 1.5*corpo.raio*cos((theta_braco_z)*M_PI/180)*sin((theta_braco+angulo_aux)*M_PI/180);
    z = corpo.cz + corpo.h*3/4 + 1.5*corpo.raio*sin((theta_braco_z)*M_PI/180);
    angulo = angulo_aux + theta_braco;
    vtiro.push_back(new Tiro(x,y,z,raio,raio,0,0,0,id,angulo,theta_braco_z,vel_tiro,textiro));
    return 1;
}
void Jogador::desenhaTiro(){
    glMatrixMode(GL_MODELVIEW);
    for (std::vector<Tiro*>::iterator it = vtiro.begin() ; it != vtiro.end(); ++it){
        glPushMatrix();
        glTranslatef((*it)->cx,(*it)->cy,(*it)->cz);
        (*it)->desenhaEsfera();
        glPopMatrix();
    }
}
void Jogador::moveTiros(GLdouble timeDiference){
    for(int i = 0; i< (int)vtiro.size(); i++){
	vtiro.at(i)->moveTiro(timeDiference);
    }
}
void Jogador::deletaTiros(Arena arena){
    for(int i = 0; i< (int)vtiro.size(); i++){
        if( vtiro.at(i)->cx > arena.arenaAzul.cx + arena.arenaAzul.raio ||
            vtiro.at(i)->cy > arena.arenaAzul.cy + arena.arenaAzul.raio ||
            vtiro.at(i)->cx < arena.arenaAzul.cx - arena.arenaAzul.raio ||
            vtiro.at(i)->cy < arena.arenaAzul.cy - arena.arenaAzul.raio){
            vtiro.erase(vtiro.begin()+i);
            continue;
        }
            
        for (std::vector<Circulo*>::iterator it = arena.baixo.begin() ; it != arena.baixo.end(); ++it){
            if(vtiro.at(i)->colide((*it))){
                vtiro.erase(vtiro.begin()+i);
                break;
            }
	}
    }
}

void Jogador::atualizaTiros(GLdouble timeDiference, Arena arena){
    moveTiros(timeDiference);
    deletaTiros(arena);
    //printf("%d\n",jogador.vtiro.size());
}

int Jogador::acertou(Jogador inimigo){
    for(int i = 0; i< (int)vtiro.size(); i++){
        if(vtiro.at(i)->colide(&inimigo.corpo)){
            vtiro.erase(vtiro.begin()+i);
            return 1;
        }
    }
    return 0;
}

void Jogador::verificaPulo(Arena arena, float MAX_ALTURA){
    if(caindo && flag_caindo){
        gettimeofday(&start, NULL);
        flag_caindo = 0;
    }
    if(corpo.cz != 0 || pulando){
        gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
        if(caindo)
            mtime += 1000 + 1000*(2*(altura-MAX_ALTURA)+MAX_ALTURA)/MAX_ALTURA;
        if(mtime <= 1000) //Subindo
            corpo.cz = altura + (MAX_ALTURA*(float)mtime/1000);
        else if(mtime > 1000 && mtime <= 2000 && !arena.emcimaObs((*this))){ //Descendo
            corpo.cz = 2*(altura+MAX_ALTURA) -(((float)mtime/1000)*(altura+MAX_ALTURA));
            for (std::vector<Circulo*>::iterator it = arena.baixo.begin() ; it != arena.baixo.end(); ++it){
                if(this->colide((*it))){
                    corpo.cz = (*it)->cz + (*it)->h;
                    caindo = 0;
                    pulando = 0;
                    flag_caindo =0;
                    altura = corpo.cz;
                    break;
                }
            }
        }
        else if(!arena.emcimaObs((*this))){ //Fim do pulo
            if(corpo.cz <= (arena.baixo.at(0)->cz+arena.baixo.at(0)->h)*0.5){
                corpo.cz = 0;
                pulando = 0;
                caindo = 0;
            }
            else{
                flag_caindo = 1;
                caindo = 1;
            }
            for (std::vector<Circulo*>::iterator it = arena.baixo.begin() ; it != arena.baixo.end(); ++it){
                if(this->colide((*it))){
                    corpo.cz = (*it)->cz + (*it)->h;
                    caindo = 0;
                    flag_caindo = 0;
                    pulando = 0;
                    break;
                }
            }
            altura = corpo.cz;
        }
        else if(arena.emcimaObs((*this))){
            caindo = 0;
            corpo.cz = arena.baixo.at(0)->cz+arena.baixo.at(0)->h;
            altura = corpo.cz;
            pulando = 0;
        }
    }
}
