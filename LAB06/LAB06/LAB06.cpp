#include <math.h>
#include <iostream>

/***freeglut***/
#include "C:\freeglut\include\GL\freeglut.h"
#include <cmath>

using namespace std;

float winWidth = 400;
float winHeight = 400;
float gridCount = 10; // default
float mx,mx1;
float my,my1;
bool isMouseClicked;
int gridMenu;
int change = 0;

void changeSize(int, int);
void renderScene(void);
void init(void);
void drawGrid();
void drawSquare();
void processMainMenu(int option);
void processGridMenu(int option);
void createPopupMenus();
void onMouseClick(int button, int state, int x, int y);
void Line(int x0, int y0, int x1, int y1);
void Line1(int x0, int y0, int x1, int y1);
void setPixel(int x, int y);
void setPixel1(int x, int y, float f);

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
	if (change == 0) {
		Line(0, 0, mx, my);
	}
	if (change == 1) {
		Line1(0, 0, mx, my);
	}
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

	for (double i = -1 * gridCount + 1; i < gridCount; i++) {
		glVertex2f(-10.0, i * 10 / gridCount);
		glVertex2f(10, i * 10 / gridCount);
		glVertex2f(i * 10 / gridCount, -10.0);
		glVertex2f(i * 10 / gridCount, 10.0);
	}
	glEnd();
}
void drawSquare()
{
	if (isMouseClicked)
	{
		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		int ix = 0, iy = 0;
		if (mx > 0) {
			ix = -1;
		}
		else {
			ix = 1;
		}
		if (my > 0) {
			iy = -1;
		}
		else {
			iy = 1;
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
			mx = (x / (2.0 * 10) - 10)*(gridCount / 10);
			my = (y / (-2.0 * 10) + 10)*(gridCount / 10);

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
	case 1: //Bresenham
		//gridCount = 10;
		change = 0;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	case 2: //Anti
		//gridCount = 15;
		change = 1;
		isMouseClicked = false;
		glutPostRedisplay();
		break;
	}
}
void createPopupMenus() {

	gridMenu = glutCreateMenu(processGridMenu);
	glutAddMenuEntry("Bresenham", 1);
	glutAddMenuEntry("Anti", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void Line(int x0, int y0, int x1, int y1) {
	if (isMouseClicked) {
		if (x0 > x1) { swap(x0, x1); swap(y0, y1);}
		int dx = x1 - x0, dy = y1 - y0;
		double m = dy * 1.0 / dx;
		cout << "m: " << m << endl;
		if (m > 0 && m <= 1) {
			int d = 2 * dy - dx;
			int delE = 2 * dy;
			int delNE = 2 * (dy - dx);
			int x = x0, y = y0;
			cout << "d: " << d << endl;
			setPixel(x, y);

			while (x < x1) {
				if (d <= 0) {
					d += delE;
					x = x + 1;
				}
				else {
					d += delNE;
					x = x + 1;
					y = y + 1;
				}
				setPixel(x, y);
			}
		}
		if (m > 1) {
			int d = 2 * dx - dy;
			int delE = 2 * dx;
			int delNE = 2 * (dx - dy);
			cout << "d: " << d << endl;
			int x = x0, y = y0;
			setPixel(x, y);

			while (y < y1) {
				if (d <= 0) {
					d += delE;
					y = y + 1;
				}
				else {
					d += delNE;
					x = x + 1;
					y = y + 1;
				}
				setPixel(x, y);
			}
		}
		if (m >= -1 && m < 0) {
			int d = 2 * dy + dx;
			int delE = 2 * dy;
			int delNE = 2 * (dy + dx);
			cout << "d: " << d << endl;
			int x = x0, y = y0;
			setPixel(x, y);

			while (x < x1) {
				if (d > 0) {
					d += delE;
					x = x + 1;
				}
				else {
					d += delNE;
					x = x + 1;
					y = y - 1;
				}
				setPixel(x, y);
			}
		}
		if (m < -1) {
			int d = 2 * dx + dy;
			int delE = 2 * dx;
			int delNE = 2 * (dx + dy);
			cout << "d: " << d << endl;
			int x = x0, y = y0;
			setPixel(x, y);

			while (y > y1) {
				if (d <= 0) {
					d += delE;
					y = y - 1;
				}
				else {
					d += delNE;
					x = x + 1;
					y = y - 1;
				}
				setPixel(x, y);
			}
		}
	}
}
void Line1(int x0, int y0, int x1, int y1) {
	float dx = x1 - x0;
	float dy = y1 - y0;
	float m = dy / dx;
	float x = x0, y = y0;

	for (x = x0; x <= x1; x++ ) {
		int yi = floor(y);
		float f = y - yi;
		setPixel1(x, yi, 1 - f);
		setPixel1(x, yi + 1, f);
		y = y + m;
	}
}
void setPixel(int x, int y) {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	int ix = 0, iy = 0;
	if (x > 0) {
		ix = -1;
	}
	else {
		ix = 1;
	}
	if (y > 0) {
		iy = -1;
	}
	else {
		iy = 1;
	}
	if (x != 0 && y != 0) {
		glVertex2f(x * 10 / gridCount, y * 10 / gridCount);
		glVertex2f(x * 10 / gridCount, (y + iy) * 10 / gridCount);
		glVertex2f((x + ix) * 10 / gridCount, (y + iy) * 10 / gridCount);
		glVertex2f((x + ix) * 10 / gridCount, y * 10 / gridCount);
	}

	glEnd();
}
void setPixel1(int x, int y, float f) {
	glBegin(GL_QUADS);
	glColor3f(f, 0.0, 0.0);
	int ix = 0, iy = 0;
	if (x > 0) {
		ix = -1;
	}
	else {
		ix = 1;
	}
	if (y > 0) {
		iy = -1;
	}
	else {
		iy = 1;
	}
	if (x != 0 && y != 0) {
		glVertex2f(x * 10 / gridCount, y * 10 / gridCount);
		glVertex2f(x * 10 / gridCount, (y + iy) * 10 / gridCount);
		glVertex2f((x + ix) * 10 / gridCount, (y + iy) * 10 / gridCount);
		glVertex2f((x + ix) * 10 / gridCount, y * 10 / gridCount);
	}

	glEnd();
}