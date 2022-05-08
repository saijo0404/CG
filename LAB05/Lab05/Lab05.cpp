#include <math.h>
#include <iostream>

/***freeglut***/
#include "C:\freeglut\include\GL\freeglut.h"
#include <cmath>

using namespace std;

float winWidth = 400;
float winHeight = 400;
float gridCount = 10; // default
float mx;
float my;
bool isMouseClicked;

int gridMenu;

void changeSize(int, int);
void renderScene(void);
void init(void);
void drawGrid();
void drawSquare();

void processMainMenu(int option);
void processGridMenu(int option);
void createPopupMenus();

void onMouseClick(int button, int state, int x, int y);

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(600, 80);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("2D Grid");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMouseFunc(onMouseClick);

	createPopupMenus();

	glutMainLoop();

	return 1;
}
void changeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void renderScene(void)
{
	// clear color
	glClear(GL_COLOR_BUFFER_BIT);

	drawGrid();
	drawSquare();

	glutSwapBuffers();
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10.f, 0, 0, 0, 0, 1, 0);
}

void drawGrid() {

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);

	for (double i = -1* gridCount+1; i < gridCount; i++) {
		glVertex2f(-10.0, i*10/ gridCount);
		glVertex2f(10, i*10/ gridCount);
		glVertex2f(i*10/ gridCount, -10.0);
		glVertex2f(i*10/ gridCount, 10.0);
	}
	glEnd();
}

void drawSquare()
{
	if (isMouseClicked)
	{
		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		/*int tx=mx, ty=my;
		if (mx > 0) {
			tx = mx - 1;
		}
		else {
			ty = mx;
		}
		if (my > 0) {
			ty = my - 1;
		}
		else {
			ty = my;
		}
		glVertex2f((tx*10 / gridCount), (ty * 10 / gridCount));
		glVertex2f((tx * 10 / gridCount), ((ty + 1) * 10 / gridCount));
		glVertex2f(((tx + 1) * 10 / gridCount), ((ty + 1) * 10 / gridCount));
		glVertex2f(((tx + 1) * 10 / gridCount), (ty * 10 / gridCount));*/
		int ix=0, iy=0;
		if (mx > 0) {
			ix = -1 ;
		}
		else {
			ix = 1 ;
		}
		if (my > 0) {
			iy = -1;
		}
		else {
			iy = 1 ;
		}
		if (mx != 0 && my != 0) {
			glVertex2f(mx * 10 / gridCount, my * 10 / gridCount);
			glVertex2f(mx * 10 / gridCount, (my + iy) * 10 / gridCount);
			glVertex2f((mx + ix) * 10 / gridCount, (my + iy) * 10 / gridCount);
			glVertex2f((mx + ix) * 10 / gridCount, my * 10 / gridCount);
		}
		
		glEnd();
	}
}

void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//mx = floor((x / (2.0 * 10) - 10)*(gridCount/10));
		//my = floor((y / (-2.0 * 10) + 10)*(gridCount/10));
		mx = (x / (2.0 * 10) - 10)*(gridCount/10);
		my = (y / (-2.0 * 10) + 10)*(gridCount/10);

		if (mx > 0) {
			mx = floor(mx) + 1;
		}
		else {
			mx = floor(mx);
		}
		if (my > 0) {
			my = floor(my) + 1;
		}
		else {
			my = floor(my);
		}
		cout << "(" << mx << ", " << my << ")" << endl;
		isMouseClicked = true;
	}
}

void processMainMenu(int option) {

	// nothing to do in here
	// all actions are for submenus
}

void processGridMenu(int option) {
	switch (option) {
	case 1: //10x10
		gridCount = 10;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	case 2: //15x15
		gridCount = 15;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	case 3: //20x20
		gridCount = 20;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	}
}


void createPopupMenus() {

	gridMenu = glutCreateMenu(processGridMenu);
	glutAddMenuEntry("10x10", 1);
	glutAddMenuEntry("15x15", 2);
	glutAddMenuEntry("20x20", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}