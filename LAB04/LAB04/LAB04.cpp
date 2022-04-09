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
void My_Mouse(int button, int state, int x, int y);
void My_Timer(int id);
void RotationMatrix();

int timer_speed = 33, timer_flag = 1;
int theta = 0;
GLfloat mx = 0, my = 0, mz = 0;
GLfloat x = 0, y = 0, z = 0;
GLfloat m00 = 1, m04 = 0, m08 = 0, m12 = 0;
GLfloat m01 = 0, m05 = 1, m09 = 0, m13 = 0;
GLfloat m02 = 0, m06 = 0, m10 = 1, m14 = 0;
GLfloat m03 = 0, m07 = 0, m11 = 0, m15 = 1;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScence);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(My_Mouse);
	glutTimerFunc(timer_speed, My_Timer, timer_flag);

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
	gluLookAt(0, 0, 10.f, 0, 0, 0, 0, 1, 0);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-10, 0, 0);
	glVertex3f(10, 0, 0);
	glEnd();
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(0, -10, 0);
	glVertex3f(0, 10, 0);
	glEnd();
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, -10);
	glVertex3f(0, 0, 10);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(mx, my, 0);
	glEnd();

	GLfloat rotAry[] = { m00,m01,m02,m03,
					m04,m05,m06,m07,
					m08,m09,m10,m11,
					m12,m13,m14,m15 };
	glMultMatrixf(rotAry);

	glColor3f(1, 1, 0);
	glutSolidCube(6);
	glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		theta = 0;
		RotationMatrix();
		break;
	case 's':
		theta += 10;
		RotationMatrix();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void My_Mouse(int button, int state, int x, int y) {
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state==GLUT_DOWN) {
				cout << "Mouse left button down" << endl;
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN) {
				cout << "Mouse middle button down" << endl;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				cout << "Mouse right button down" << endl;
			}
			break;
	}
	mx = (x*20.0 / 400) - 10;
	my = -(y*20.0 / 400) + 10;
	cout << "X: " << mx << " Y: " << my << endl;
}

void My_Timer(int id) {
	glutTimerFunc(timer_speed, My_Timer, timer_flag);
}

void RotationMatrix() {
	GLfloat c = cos(theta*PI / 180.0);
	GLfloat s = sin(theta*PI / 180.0);
	GLfloat t = 1.0 - c;
	x = mx;
	y = my;
	z = mz;
	double magnitude = sqrt(mx*mx + my * my + mz * mz);
	if (magnitude == 0) ;
	else {
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	m00 = c + x * x * t;
	m05 = c + y * y * t;
	m10 = c + z * z * t;

	m01 = x * y * t + z * s;
	m04 = x * y * t - z * s;

	m02 = x * z * t - y * s;
	m08 = x * z * t + y * s;

	m06 = y * z * t + x * s;
	m09 = y * z * t - x * s;
}