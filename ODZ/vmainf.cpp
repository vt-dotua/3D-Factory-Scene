#include <windows.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glaux.h>
#include <iostream>
#include <math.h>

#define M_PI 3.14159265358979323846
// кут повороту камери
float angle = 0.0;

GLfloat front_color[] = {1,1,1,1};
GLfloat back_color[] = {1,1,1,1};

// координати вектора напрямку руху камери
float lx = 0.0f, lz = -1.0f;
// XZ позиція камери
float x = 0.0f, z = 70.0f;//70
// Ключі статусу камери. Змінні ініціалізуються нульовими значеннями
// коли клавіші не натиснуті
float deltaAngle = 0.0f;
float deltaMove = 0;
float alpha=110, beta=0;
float delta=0.001;
int vA = 0;
int vB = 0;
float scale = 1;
int vS = 0;

float pos[4] = {10, 10, 15,1};
float pos2[4] = {-10, 10, 15,1};

//Глоб. зм
unsigned int sky_tex;
AUX_RGBImageRec* sky_image;

unsigned int yachik_tex;
AUX_RGBImageRec* yachik_image;


void wall(float k){
	glBegin(GL_QUADS);
    glColor3d (1,0,0); // малюємо трикутник
    glVertex3d(-2*k,  0, -1.5*k);//D
	glVertex3d( 2*k,  0, -1.5*k); //K
	glVertex3d( 2*k,  0,  1.5*k);//C
	glVertex3d(-2*k,  0,  1.5*k);//E

	glColor3d (1,0,0); // малюємо трикутник
    glVertex3d(-2*k,  2, -1.5*k);//D
	glVertex3d( 2*k,  2, -1.5*k); //K
	glVertex3d( 2*k,  2,  1.5*k);//C
	glVertex3d(-2*k,  2,  1.5*k);//E
}

void changeSize(int w, int h) {
	float ratio;
// запобігання поділу на нуль
	if (h == 0)
		h = 1;
	ratio = w * 1.0 / h;
// використовуємо матрицю проекції
	glMatrixMode(GL_PROJECTION);
// Обнуляємо матрицю
	glLoadIdentity();
// встановити параметри вьюпорту
	glViewport(0, 0, w, h);
// встановити коректну перспективу
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
// повернутися до матриці проекції
	glMatrixMode(GL_MODELVIEW);
}


void computePos(float deltaMove)
{
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle)
{
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void computeAlpha(){
	alpha += vA * 0.1;
}

void computeBate(){
	beta += vB * 0.1;
}

void lightScale(void);
void computeScale(){

	if(scale > 0.08)
		scale += vS * 0.002;
	else
		scale += 0.08;
}

void pressKey(int key, int xx, int yy) {
	float nor[4];
	int mod = glutGetModifiers();
	if( key == GLUT_KEY_LEFT){
		if(mod == GLUT_ACTIVE_ALT){
			vA = - 1;	
		}
		else if(mod != GLUT_ACTIVE_ALT && mod != GLUT_ACTIVE_CTRL)		
			deltaAngle = -0.004f;
			
	}else if ( key == GLUT_KEY_RIGHT){
		if(mod == GLUT_ACTIVE_ALT){
			vA =+ 1;
		}
		else if(mod != GLUT_ACTIVE_ALT && mod != GLUT_ACTIVE_CTRL)
			deltaAngle = 0.004f;
	}
	else if ( key == GLUT_KEY_UP ){
		if(mod == GLUT_ACTIVE_ALT)
		{
			vB = 1;
		}
		else if(mod == GLUT_ACTIVE_CTRL){
			vS = 1;
		}
		else
		deltaMove = 0.3f;
	}
	else if ( key == GLUT_KEY_DOWN){
		if(mod == GLUT_ACTIVE_ALT)
		{
			vB = - 1;
		}
		else if(mod == GLUT_ACTIVE_CTRL){
			vS = -1;
		}
		else
		deltaMove = -0.3f;
	}	
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT:
			deltaAngle = 0.0f;
			vA = 0;
		break;
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
			deltaMove = 0;
			vB = 0;
			vS = 0;
			//scale = 1;
		break;
	}
}

void LookAt()
{
	// обнулити трансформацію
	glLoadIdentity();
	// встановити камеру
	gluLookAt( x, 1.0f, z, x+lx, 1.0f, z+lz, 0.0f, 1.0f, 0.0f );
}

void roomArea(){
	glColor3d(.69,.69,.69);
	glPushMatrix();
	auxSolidBox(20, 0.5, 35);
	glPopMatrix();
	
	glColor3d(.82,.82,.82);
	glPushMatrix();
	glTranslated(0, 5.3, 17.5);
	glRotated(90,1,0,0 );
	auxSolidBox(20, 0.5, 11);
	glPopMatrix();

	glColor3d(.82,.82,.82);
	glPushMatrix();
	glTranslated(10, 5.3, 0);
	glRotated(90,1,0,0 );
	glRotated(90,0,0,1 );
	auxSolidBox(35, 0.5 , 11);
	glPopMatrix();

}


void CranePart1(void){

	glPushMatrix();
	glTranslated( 0, 0,-2.7);
	glRotated(20,1,0,0 );
	auxSolidCylinder(0.1, 11);	 
	glPopMatrix();

	glPushMatrix();
	glTranslated( 0,0,-2);
	glRotated(-20,1,0,0);
	auxSolidCylinder(0.1, 11);	 
	glPopMatrix();

}

void CranePart2(void){
	glPushMatrix();
	glTranslated( -2,2,0);
	glRotated(6,0,0,1);
	CranePart1();
	glPopMatrix();

	glPushMatrix();
	glTranslated( -1, -6.8, 0.3);
	glRotated(90,1,0,0);
	auxSolidCylinder(0.1, 7.2);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -1.49, -2.8, -1.2);
	glRotated(90,1,0,0);
	auxSolidCylinder(0.1, 4.3);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.5, -2.8, -1.2);
	glRotated(90,1,0,0);
	auxSolidCylinder(0.1, 4.3);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.5, -2.8, -4.47);
	glRotated(90,1,0,0);
	glRotated(90,0,0,1);
	auxSolidCylinder(0.1, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.5, -2.8, -0.24);
	glRotated(90,1,0,0);
	glRotated(90,0,0,1);
	auxSolidCylinder(0.1, 1);
	glPopMatrix();
//------
	glPushMatrix();
	glTranslated( -1.8, 0, -2.3);
	glRotated(90,1,0,0);
	auxSolidCylinder(0.1, 2.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.21, 0, -2.3);
	glRotated(90,1,0,0);
	auxSolidCylinder(0.1, 2.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.7, 0, -3.41);
	glRotated(90,1,0,0);
	glRotated(90,0,0,1);
	auxSolidCylinder(0.1, 1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.7, 0, -1.3);
	glRotated(90,1,0,0);
	glRotated(90,0,0,1);
	auxSolidCylinder(0.1, 1.5);
	glPopMatrix();
//------

	glPushMatrix();
	glTranslated( 0,2,0);
	glRotated(-6,0,0,1);
	CranePart1();
	glPopMatrix();

	
}

void CranePart3(void){
	glPushMatrix();
	glTranslated( -1.1,3.5,-2.3);
	glRotated(0,0,0,1);
	auxSolidBox(20, 0.5, 0.1); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( -1.1,3.7,-2.3);
	glRotated(0,0,0,1);
	auxSolidBox(20, 0.1, 0.5); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( -1.1,3.3,-2.3);
	glRotated(0,0,0,1);
	auxSolidBox(20, 0.1, 0.5); 
	glPopMatrix();

}

void CranePart5(void){

	//glPolygonMode(GL_FILL); //
    glBegin(GL_QUADS);
    glVertex3d(-0.2,  0, 0);//D
	glVertex3d( 0.2,  0, 0); //K
	
	glVertex3d( 0.5,  0.7, 0 );//C
	glVertex3d(-0.5,  0.7, 0);//E
    glEnd();

	glBegin(GL_QUADS);
    glVertex3d(-0.2,  0, 0.3);
	glVertex3d( 0.2,  0, 0.3);
	
	glVertex3d( 0.5,  0.7, 0.3);
	glVertex3d(-0.5,  0.7, 0.3);
    glEnd();


	glBegin(GL_QUADS);
	glVertex3d(-0.2,  0, 0);
	glVertex3d(-0.2,  0, 0.3);

    glVertex3d(-0.5,  0.7, 0.3);
	glVertex3d(-0.5,  0.7, 0);
    glEnd();

	glBegin(GL_QUADS);
	glVertex3d(0.2,  0, 0);
	glVertex3d(0.2,  0, 0.3);

    glVertex3d(0.5,  0.7, 0.3);
	glVertex3d(0.5,  0.7, 0);
    glEnd();

	glBegin(GL_QUADS);
	glVertex3d(-0.2,  0, 0.3);
	glVertex3d(-0.2,  0, 0);
	glVertex3d( 0.2,  0, 0);
	glVertex3d( 0.2,  0, 0.3);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3d(-0.5,  0.7, 0.3);
	glVertex3d(-0.5,  0.7, 0);
	glVertex3d( 0.5,  0.7, 0);
	glVertex3d( 0.5,  0.7, 0.3);
	glEnd();


}

void CranePart6(void){
	
	glPushMatrix();
	glTranslated( 0,0,0);
	glRotated(0,0,0,1);
	auxSolidBox(0.1, 0.5, 0.1); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( 0,-0.2,-0.1);
	glRotated(0,0,0,1);
	auxSolidBox(0.1, 0.1, 0.2); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( 0,-0.15,-0.2);
	auxSolidBox(0.1, 0.2, 0.13); 
	glPopMatrix();
}

void CranePart4(void){
	
	glPushMatrix();
	glTranslated( 0, 2, -2.3);
	auxSolidCylinder(0.05, 3.5);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.4, -1.2, -2.4);
	CranePart5();
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.4, -1.4, -2.25);
	glRotated(90,0,1,0);
	CranePart6();
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.8, 2, -2.3);
	auxSolidCylinder(0.05, 3.5);
	glPopMatrix();

}


void crane(){

	glColor3f(.21,.39,.80);
	glPushMatrix();
	glTranslated( 8, 0, 0);
	CranePart2();
	glPopMatrix();

	glColor3f(.21,.39,.80);
	glPushMatrix();
	CranePart4();
	glPopMatrix();

	glColor3f(.21,.39,.80);
	glPushMatrix();
	glTranslated( -0.5, 2.9,-2.3);
	auxSolidBox(2, 0.5, 0.5); 
	glPopMatrix();

	glColor3f(.21,.39,.80);
	glPushMatrix();
	glTranslated( 0, -0.35, 0);
	CranePart3();
	glPopMatrix();

	glColor3f(.21,.39,.80);
	glPushMatrix();
	glTranslated( -8, 0, 0);
	CranePart2();
	glPopMatrix();


}

void windows(){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,  sky_tex);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);
	glVertex3d( 6.5, 0,  0);
	glTexCoord2d(0,1);
	
	glVertex3d(-6.5, 0,  0);
	glTexCoord2d(1,1);
	
	glVertex3d(-6.5, -5,  0);
	glTexCoord2d(1,0);
	
	glVertex3d( 6.5, -5,  0);	
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	auxSolidBox(13, 0.12, 0.13); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( 0, -5, 0);
	auxSolidBox(13, 0.12, 0.13); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( 0, -2.5, 0);
	auxSolidBox(13, 0.12, 0.13); 
	glPopMatrix();


	glPushMatrix();
	glTranslated( 0, -2.5, 0);
	auxSolidBox(0.12, 5, 0.13); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( -6.45, -2.5, 0);
	auxSolidBox(0.12, 5, 0.13); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( -3.25, -2.5, 0);
	auxSolidBox(0.12, 5, 0.13); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( 6.45, -2.5, 0);
	auxSolidBox(0.12, 5, 0.13); 
	glPopMatrix();

	glPushMatrix();
	glTranslated( 3.25, -2.5, 0);
	auxSolidBox(0.12, 5, 0.13); 
	glPopMatrix();

}

void stairs(){
	glColor3f(.83,.51,0);
	glPushMatrix();
	glTranslated( 0, 10, 0);
	auxSolidCylinder(0.07, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated( -0.8, 10, 0);
	auxSolidCylinder(0.07, 10);
	glPopMatrix();

	for(int i = 0, k=0; i < 9; i++, k +=1)
	
	{
		glPushMatrix();
		glTranslated( 0.2, 2 + k, 0);
		glRotated(90,1,0,0);
		glRotated(90,0,0,1);
		auxSolidCylinder(0.07, 0.8);
		glPopMatrix();
	}

}

void pipe( GLUquadricObj *quadObj )
{
	glColor3f(.73,.57,.38);
	glPushMatrix();
		glTranslated( 0, 0.75, 4);
		glRotated(90,0,0,1);
		glRotated(90,1,0,0);
		auxSolidCylinder(0.5, 14);
	glPopMatrix();
	
	glPushMatrix();
		glTranslated( 0, 0.75, 4);
		glRotated(90,0,0,1);
		glRotated(90,1,0,0);
		auxSolidCylinder(0.4, 14);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0.75, 5);
		gluQuadricDrawStyle (quadObj, GLU_FILL);
		gluDisk(quadObj,0.35, 0.5,30,30);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0.75, -9);
		gluQuadricDrawStyle (quadObj, GLU_FILL);
		gluDisk(quadObj,0.35, 0.5,30,30);
	glPopMatrix();
}

void pipes(GLUquadricObj *quadObj){
	
	glPushMatrix();
		glTranslated( 0, 0, 0);
		pipe(quadObj);
	glPopMatrix();

	glPushMatrix();
		glTranslated( -1, 0, 0);
		pipe(quadObj);
	glPopMatrix();

	glPushMatrix();
		glTranslated( -0.5, 0.87, 0);
		pipe(quadObj);
	glPopMatrix();
	
}

void beidge_part1(){
	
	glPushMatrix();
		glTranslated(5.3, 1.7, -11.38);
		glRotated(-55,0,0,1);
		auxSolidBox(4.5, 0.5, 0.25);
	glPopMatrix();

	
	glPushMatrix();
		glTranslated(5.3, 1.7, -8.65);
		glRotated(-55,0,0,1);
		auxSolidBox(4.5, 0.5, 0.25);
	glPopMatrix();

	for(float i = 0, h = 0, h2 = 0; i < 6; i++, h += 0.5, h2 += 0.35){
		glPushMatrix();
			glTranslated(6.14 - h2 , 0.5 + h , -10);
			auxSolidBox(0.5, 0.15, 2.8);
		glPopMatrix();
	
	}

	glPushMatrix();
	glTranslated(5.3, 4.5, -8.65);
	glRotated(36,0,0,1);
	auxSolidCylinder(0.08, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslated(5.3, 4.5, -11.38);
	glRotated(36,0,0,1);
	auxSolidCylinder(0.08, 4);
	glPopMatrix();

	for(float i = 0,h=0, h1=0; i < 3; i++, h += 0.7, h1+=0.48){
		glPushMatrix();
		glTranslated(6.15 - h, 2.3+(h1*2), -11.38);
		auxSolidCylinder(0.08, 3);
		glPopMatrix();
	}

	for(float i = 0,h=0, h1=0; i < 3; i++, h += 0.7, h1+=0.48){
		glPushMatrix();
		glTranslated(6.15 - h, 2.3+(h1*2), -8.65);
		auxSolidCylinder(0.08, 3);
		glPopMatrix();
	}

}

void beidge_part2(){
	
	glPushMatrix();
		glTranslated(-5.3, 1.7, -11.38);
		glRotated(55,0,0,1);
		auxSolidBox(4.5, 0.5, 0.25);
	glPopMatrix();

	
	glPushMatrix();
		glTranslated(-5.3, 1.7, -8.65);
		glRotated(55,0,0,1);
		auxSolidBox(4.5, 0.5, 0.25);
	glPopMatrix();

	
	for(float i = 0, h = 0, h2 = 0; i < 6; i++, h += 0.5, h2 += 0.35){
		glPushMatrix();
			glTranslated(-6.14 + h2 , 0.5 + h , -10);
			auxSolidBox(0.5, 0.15, 2.8);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslated(-5.3, 4.5, -8.65);
	glRotated(-36,0,0,1);
	auxSolidCylinder(0.08, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5.3, 4.5, -11.38);
	glRotated(-36,0,0,1);
	auxSolidCylinder(0.08, 4);
	glPopMatrix();

	for(float i = 0,h=0, h1=0; i < 3; i++, h += 0.7, h1+=0.48){
		glPushMatrix();
		glTranslated(-6.15 + h, 2.3+(h1*2), -11.38);
		auxSolidCylinder(0.08, 3);
		glPopMatrix();
	}

	for(float i = 0,h=0, h1=0; i < 3; i++, h += 0.7, h1+=0.48){
		glPushMatrix();
		glTranslated(-6.15 + h, 2.3+(h1*2), -8.65);
		auxSolidCylinder(0.08, 3);
		glPopMatrix();
	}
}

void beidge_part3(){

	
	glPushMatrix();
		glTranslated(-3.75, 5.27,-8.65);
		glRotated(90,0,0,1);
		auxSolidCylinder(0.08, 9.54);
	glPopMatrix();

	for(float i = 0,h=0; i < 13; i++, h += 0.7){
		glPushMatrix();
		glTranslated(4.15 - h, 4.2, -8.65);
		auxSolidCylinder(0.08, 1.7);
		glPopMatrix();
	}

	glPushMatrix();
		glTranslated(-3.75, 5.27, -11.38);
		glRotated(90,0,0,1);
		auxSolidCylinder(0.08, 9.54);
	glPopMatrix();

	for(float i = 0,h=0; i < 13; i++, h += 0.7){
		glPushMatrix();
		glTranslated(4.15 - h, 4.2, -11.38);
		auxSolidCylinder(0.08, 1.7);
		glPopMatrix();
	}
}

void bridge(){
	glColor3f(.55,.54,.58);
	glPushMatrix();
		glTranslated(0,3.56,-10);
		auxSolidBox(8.5, 0.25, 3);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 0);
		beidge_part1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 0);
		beidge_part2();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 0);
		beidge_part3();
	glPopMatrix();

}


void part_box(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,  yachik_tex);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);
	glVertex3d( 2, 0,  0);
	glTexCoord2d(0,1);
	
	glVertex3d(-2, 0,  0);
	glTexCoord2d(1,1);
	
	glVertex3d(-2, -4,  0);
	glTexCoord2d(1,0);
	
	glVertex3d( 2, -4,  0);	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void box()
{
	glPushMatrix();
	glTranslated(0,0,0);
		part_box();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,0,-4);
		part_box();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2,0,-2);
	glRotated(90, 0,1,0);
		part_box();
	glPopMatrix();

	glPushMatrix();
	glTranslated(2,0,-2);
	glRotated(90, 0,1,0);
		part_box();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,0,0);
	glRotated(90, 1,0,0);
		part_box();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,-4,0);
	glRotated(90, 1,0,0);
		part_box();
	glPopMatrix();



}

void table_part(){
	glPushMatrix();
		auxSolidBox(0.16, 2, 0.16);
	glPopMatrix();
}

void table(){
	glColor3f(.57,.39,.32);
	glPushMatrix();
	glTranslated(0,0,0);
	//glRotated(90, 1,0,0);
		auxSolidBox(3, 0.15, 1.5);
	glPopMatrix();

	glPushMatrix();
		glTranslated(1.4,-1, 0.5);
		table_part();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-1.4,-1,0.5);
		table_part();
	glPopMatrix();

	glPushMatrix();
		glTranslated(1.4,-1,-0.5);
		table_part();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-1.4,-1,-0.5);
		table_part();
	glPopMatrix();

}

void stool_part(){
	glPushMatrix();
		auxSolidBox(0.16, 1.4, 0.16);
	glPopMatrix();
}

void stool(){
	glColor3f(.57,.39,.32);
	glPushMatrix();
	glTranslated(0,-0.3,0);
	//glRotated(90, 1,0,0);
		auxSolidBox(1.2, 0.15, 1.2);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0.5,-1, 0.5);
		stool_part();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-0.5,-1,0.5);
		stool_part();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0.5,-1,-0.5);
		stool_part();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-0.5,-1,-0.5);
		stool_part();
	glPopMatrix();
}

/*
void Lamp(){
	 float nor[4];
	 glGetLightfv(GL_LIGHT0, GL_POSITION, nor);
	 glColor3d(1,1,1);
	 glPushMatrix();
 	 glTranslated( nor[0], nor[1], nor[2]);
	 auxSolidSphere(1); 
	 glPopMatrix();
}

void Lamp2(){
	 float nor[4];
	 glGetLightfv(GL_LIGHT1, GL_POSITION, nor);
	 glColor3d(1,0,0);
	 glPushMatrix();
 	 glTranslated( nor[0], nor[1], nor[2]);
	 auxSolidSphere(1); 
	 glPopMatrix();
}
*/

void renderScene(void)
{
	int i,j;
	if (deltaMove)
		computePos(deltaMove);

	if (deltaAngle)
		computeDir(deltaAngle);
	
	if(vA)
		computeAlpha();

	if(vB)
		computeBate();

	if(vS)
		computeScale();

	GLUquadricObj *quadObj = gluNewQuadric ();

	// очистити буфер кольору і глибини
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	LookAt();
	//Lamp(); Удалить
	//Lamp2(); Удалить
	glPushMatrix();
	glRotated(alpha, 0,1,0); // обертаємося навколо осей Х і Y
	glRotated(beta, -1,0,0);
	glScalef(scale, scale, scale);
		
		glPushMatrix();
		glTranslated(7.3,3.5,14);
		glScalef(0.8, 0.8, 0.8);
			box();
		glPopMatrix();

		glPushMatrix();
		glTranslated( 9.7, 9, 8);
		glRotated(90, 0,1,0);
			windows();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated( 9.7, 9, -8);
		glRotated(90, 0,1,0);
			windows();
		glPopMatrix();
	 
		glPushMatrix();
		glTranslated( 0, 7.2, 0);
			crane();
		glPopMatrix();
		
		glPushMatrix();
	    glTranslated(8, -1.2, 15);
		glRotated(90, 0,1,0);
		glRotated(9, 1,0,0);
			stairs();
		glPopMatrix();

		
		glPushMatrix();
		glTranslated(0, 0, 0);
		pipes(quadObj);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0,0,0);
			bridge();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 2.2, 15);
			table();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-5, 2.2, 15);
			table();
		glPopMatrix();

		
		glPushMatrix();
		glTranslated(0, 1.9, 13);
			stool();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-5, 1.9, 13);
			stool();
		glPopMatrix();

		roomArea();

	glPopMatrix();

	
	gluDeleteQuadric (quadObj);
	glutSwapBuffers();
}



int main(int argc, char **argv) {

	
	float sp[4] = {1,1,1,1};
	float dir[3] = {-1,-1,-1};
	float color[4] = {1,0,0,1};
    GLfloat mat_specular[] = {1,1,1,1};

	//float sp2[4] = {0,0,0,0};
	float dir2[3] = {-1,-1,-1};
    float color2[4] = { 0,1,1,1};

	// ініціалізація GLUT і створення вікна
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Tatarinov Vadim KB-61");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_SPECULAR, sp);

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);

	glLightfv(GL_LIGHT1, GL_SPECULAR, sp);

	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color2);


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
	// реєстрація викликів
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	
	glutSpecialFunc(pressKey);
	// Нові функції для реєстрації
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);

	//Текстуры
	sky_image = auxDIBImageLoad("D:\\OPenGL\\OPenGL\\ODZ\\ODZ\\Debug\\sky.bmp");
	glGenTextures(1, &sky_tex);
	glBindTexture(GL_TEXTURE_2D, sky_tex);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
	sky_image->sizeX,
	sky_image->sizeY,
	GL_RGB, GL_UNSIGNED_BYTE,
	sky_image->data);

	yachik_image = auxDIBImageLoad("D:\\OPenGL\\OPenGL\\ODZ\\ODZ\\Debug\\yachik.bmp");
	glGenTextures(1, &yachik_tex);
	glBindTexture(GL_TEXTURE_2D, yachik_tex);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
	yachik_image->sizeX,
	yachik_image->sizeY,
	GL_RGB, GL_UNSIGNED_BYTE,
	yachik_image->data);

	
	// головний цикл
	glutMainLoop();
	return 1;

}

