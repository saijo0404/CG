/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
/***freeglut***/
#include "C:\freeglut\include\GL\freeglut.h"

void SetupRC();
void ChangeSize(int w,int h);
void RenderScence();
void myKeyboard(unsigned char key, int x, int y);
void mySpecialKey(int key,int x, int y);

int tx = 0, ty = 0, tz = 0;
int thetaX = 0, thetaY = 0, thetaZ = 0;

int main(int argc, char** argv)
{
	tx = 0, ty = 0, tz = 0;
	thetaX = 0, thetaY = 0, thetaZ = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScence);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKey);

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

	glRotatef(thetaX, 1, 0, 0);
	glRotatef(thetaY, 0, 1, 0);
	glRotatef(thetaZ, 0, 0, 1);
	glTranslatef(tx, ty, tz);

	glColor3f(1, 1, 0);
	glutSolidCube(6);
	glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'r':
			thetaX = 0, thetaY = 0, thetaZ = 0;
			tx = 0, ty = 0, tz = 0;
			break;
		case 'a':
			thetaX += 10;
			//glRotatef(thetaX, 1, 0, 0);
			break;
		case 'd':
			thetaX -= 10;
			//glRotatef(thetaX, 1, 0, 0);
			break;
		case 'w':
			thetaY += 10;
			//glRotatef(thetaY, 0, 1, 0);
			break;
		case 's':
			thetaY -= 10;
			//glRotatef(thetaY, 0, 1, 0);
			break;
		case 'z':
			thetaZ += 10;
			//glRotatef(thetaX, 0, 0, 1);
			break;
		case 'x':
			thetaZ -= 10;
			//glRotatef(thetaZ, 0, 0, 1);
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