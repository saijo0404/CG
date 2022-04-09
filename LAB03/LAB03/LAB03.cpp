/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
using namespace std;

/***freeglut***/
#include "C:\freeglut\include\GL\freeglut.h"
#include <cmath>

#define PI 3.14159265

void SetupRC();
void ChangeSize(int w, int h);
void RenderScence();
void myKeyboard(unsigned char key, int x, int y);
void mySpecialKey(int key, int x, int y);
GLfloat mycos(int theta);
GLfloat mysin(int theta);

int tx = 0, ty = 0, tz = 0;
int thetaX = 0, thetaY = 0, thetaZ = 0;
float s = 1;

int main(int argc, char** argv)
{
<<<<<<< HEAD
	tx = 0, ty = 0, tz = 0;
	thetaX = 0, thetaY = 0, thetaZ = 0;
	s = 1;

=======
>>>>>>> aa31787a6eb46884dbb641193cbc2381f6dedc09
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKey);
	glutDisplayFunc(RenderScence);
	glutMainLoop();
	return 0;
}

void SetupRC() {
	GLfloat whiteLight[] = { 0.45f,0.45f,0.45f,1.0f };
	GLfloat sourceLight[] = { 0.25f,0.25f,0.25f,1.0f };
	GLfloat lightPos[] = { 0.f,25.0f,20.0f,0.0f };

	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
}

void ChangeSize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderScence() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.f, 1.f, 10.f, 0, 0, 0, 0, 1, 0);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-100, 0, 0);
	glVertex3f(100, 0, 0);
	glEnd();
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(0, -100, 0);
	glVertex3f(0, 100, 0);
	glEnd();
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(50, 50, -100);
	glVertex3f(-50, -50, 100);
	glEnd();

	/*glRotatef(thetaX, 1, 0, 0);
	glRotatef(thetaY, 0, 1, 0);
	glRotatef(thetaZ, 0, 0, 1);*/
<<<<<<< HEAD
	/*GLfloat rotAryX[] = { 1,0,0,0,
=======
	GLfloat rotAryX[] = { 1,0,0,0,
>>>>>>> aa31787a6eb46884dbb641193cbc2381f6dedc09
				   0,cos(thetaX*PI / 180.0),sin(thetaX*PI / 180.0),0,
				   0,-sin(thetaX*PI / 180.0),cos(thetaX*PI / 180.0),0,
				   0,0,0,1 };
	GLfloat rotAryY[] = { cos(thetaY*PI / 180.0),0,-sin(thetaY*PI / 180.0),0,
					   0,1,0,0,
					   sin(thetaY*PI / 180.0),0,cos(thetaY*PI / 180.0),0,
					   0,0,0,1 };
	GLfloat rotAryZ[] = { cos(thetaZ*PI / 180.0),sin(thetaZ*PI / 180.0),0,0,
				   -sin(thetaZ*PI / 180.0),cos(thetaZ*PI / 180.0),0,0,
				   0,0,1,0,
				   0,0,0,1 };
	glMultMatrixf(rotAryX);
	glMultMatrixf(rotAryY);
<<<<<<< HEAD
	glMultMatrixf(rotAryZ);*/
	GLfloat rotMatrix[] = {
		mycos(thetaY)+mycos(thetaZ)-1,mysin(thetaZ),-mysin(thetaY),0,
		-mysin(thetaZ),mycos(thetaX)+mycos(thetaZ)-1,mysin(thetaX),0,
		mysin(thetaY),-mysin(thetaX),mycos(thetaX)+mycos(thetaY)-1,0,
		0,0,0,1 };
	glMultMatrixf(rotMatrix);

=======
	glMultMatrixf(rotAryZ);
	/*GLfloat rotAry[] = { (thetaY != 0) ? cos(thetaY*PI / 180.0) : cos(thetaZ*PI / 180.0),sin(thetaZ*PI / 180.0),-sin(thetaY*PI / 180.0),0,
						 -sin(thetaZ*PI / 180.0),(thetaX != 0) ? cos(thetaX*PI / 180.0) : cos(thetaZ*PI / 180.0),sin(thetaX*PI / 180.0),0,
						 sin(thetaY*PI / 180.0),-sin(thetaX*PI / 180.0),(thetaX != 0) ? cos(thetaX*PI / 180.0) : cos(thetaY*PI / 180.0),0,
						0,0,0,1	};
	
	glMultMatrixf(rotAry);
	cout << "x: " << thetaX << " y: " << thetaY << " z: " << thetaZ << " s: " << endl;*/
>>>>>>> aa31787a6eb46884dbb641193cbc2381f6dedc09
	//glTranslatef(tx, ty, tz);
	GLfloat translateMatrix[] = { 1,0,0,0,
				   0,1,0,0,
				   0,0,1,0,
				   tx,ty,tz,1 };
	glMultMatrixf(translateMatrix);

	GLfloat scale[] = { s,0,0,0,
				   0,s,0,0,
				   0,0,s,0,
				   0,0,0,1 };
	glMultMatrixf(scale);

	glColor3f(1, 1, 0);
	glutSolidCube(6);
	glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		thetaX = 0, thetaY = 0, thetaZ = 0;
		//tx = 0, ty = 0, tz = 0;
		break;
	case 'a':
		thetaX += 10;
		//thetaY = 0, thetaZ = 0;
		//glRotatef(thetaX, 1, 0, 0);
		break;
	case 'd':
		thetaX -= 10;
		//thetaY = 0, thetaZ = 0;
		//glRotatef(thetaX, 1, 0, 0);
		break;
	case 'w':
		thetaY += 10;
		//thetaX = 0, thetaZ = 0;
		//glRotatef(thetaY, 0, 1, 0);
		break;
	case 's':
		thetaY -= 10;
		//thetaX = 0, thetaZ = 0;
		//glRotatef(thetaY, 0, 1, 0);
		break;
	case 'z':
		thetaZ += 10;
<<<<<<< HEAD
		//glRotatef(thetaZ, 0, 0, 1);
=======
		//thetaX = 0, thetaY = 0;
		//glRotatef(thetaX, 0, 0, 1);
>>>>>>> aa31787a6eb46884dbb641193cbc2381f6dedc09
		break;
	case 'x':
		thetaZ -= 10;
		//thetaX = 0, thetaY = 0;
		//glRotatef(thetaZ, 0, 0, 1);
		break;
	case 'l':
<<<<<<< HEAD
		s -= 0.1;
		break;
	case 'o':
		s += 0.1;
=======
		s += 0.1;
		break;
	case 'o':
		s -= 0.1;
>>>>>>> aa31787a6eb46884dbb641193cbc2381f6dedc09
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void mySpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		tx -= 1;
		//glTranslatef(tx, ty, tz);
		break;
	case GLUT_KEY_RIGHT:
		tx += 1;
		//glTranslatef(tx, ty, tz);
		break;
	case GLUT_KEY_UP:
		ty += 1;
		//glTranslatef(tx, ty, tz);
		break;
	case GLUT_KEY_DOWN:
		ty -= 1;
		//glTranslatef(tx, ty, tz);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
GLfloat mycos(int theta) {
	return cos(theta*PI / 180.0);
}
GLfloat mysin(int theta) {
	return sin(theta*PI / 180.0);
}