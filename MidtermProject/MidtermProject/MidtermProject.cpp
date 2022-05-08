#include <math.h>
#include <iostream>

/***freeglut***/
#include "C:\freeglut\include\GL\freeglut.h"
#include <time.h>

using namespace std;

float winWidth = 400;
float winHeight = 400;
float gridCount = 10; // default
int n = 0;
float indexAry[20][5];
bool isMouseClicked;
int gridMenu;
bool ispolygon = false;
bool isLine = true;

void changeSize(int, int);
void renderScene(void);
void init(void);
void drawGrid();
void drawSquare(float x, float y, GLfloat R, GLfloat G, GLfloat B);
void processMainMenu(int option);
void processGridMenu(int option);
void createPopupMenus();
void onMouseClick(int button, int state, int x, int y);
void Line(int x0, int y0, int x1, int y1, int index);
void setPixelInterpolated(int x, int y, GLfloat R, GLfloat G, GLfloat B);
void crow();
void scanY(int iMin);
void differenceY(float* v1, float* v2, float* l, float* dl, int y);
void differenceX(float* v1, float* v2, float* e, float* de, int x);
void difference(float* v1, float* v2, float* e, float* de, float d, float f);
void increment(float* edge, float* delta);
void scanX(float* l, float* r, int y);

int main(int argc, char** argv)
{
	srand(time(NULL));
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
	crow();

	for (int i = 0; i < n - 1; ++i) {
		Line(indexAry[i][0], indexAry[i][1], indexAry[i + 1][0], indexAry[i + 1][1], i);
	}
	for (int i = 0; i < n; ++i) {
		drawSquare(indexAry[i][0], indexAry[i][1], indexAry[i][2], indexAry[i][3], indexAry[i][4]);
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
void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isMouseClicked = true;
		float mx = (x / (2.0 * 10) - 10)*(gridCount / 10);
		float my = (y / (-2.0 * 10) + 10)*(gridCount / 10);

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

		if (indexAry[0][0] == mx && indexAry[0][1] == my) {
			ispolygon = true;
			indexAry[n][0] = indexAry[0][0];
			indexAry[n][1] = indexAry[0][1];
			indexAry[n][2] = indexAry[0][2];
			indexAry[n][3] = indexAry[0][3];
			indexAry[n][4] = indexAry[0][4];
			n = n + 1;
		}
		if (!ispolygon) {
			indexAry[n][0] = mx;
			indexAry[n][1] = my;
			indexAry[n][2] = (double)rand() / (RAND_MAX + 1.0);
			indexAry[n][3] = (double)rand() / (RAND_MAX + 1.0);
			indexAry[n][4] = (double)rand() / (RAND_MAX + 1.0);
			n = n + 1;
		}
	}
}
void drawSquare(float x, float y, GLfloat R, GLfloat G, GLfloat B)
{
	if (isMouseClicked)
	{
		glBegin(GL_QUADS);
		glColor3f(R, G, B);
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
}
void Line(int x0, int y0, int x1, int y1, int index) {
	if (isMouseClicked) {
		bool flag = false;
		if (x0 > x1) {
			flag = true;
			swap(x0, x1);
			swap(y0, y1);
			swap(indexAry[index][2], indexAry[index + 1][2]);
			swap(indexAry[index][3], indexAry[index + 1][3]);
			swap(indexAry[index][4], indexAry[index + 1][4]);
		}
		int dx = x1 - x0, dy = y1 - y0;
		double m = dy * 1.0 / dx;
		GLfloat R = (indexAry[index][2] - indexAry[index + 1][2]);
		GLfloat G = (indexAry[index][3] - indexAry[index + 1][3]);
		GLfloat B = (indexAry[index][4] - indexAry[index + 1][4]);
		int count = 1;
		cout << "m: " << m << endl;
		if (m >= 0 && m <= 1) {
			int d = 2 * dy - dx;
			int delE = 2 * dy;
			int delNE = 2 * (dy - dx);
			int x = x0, y = y0;

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
				setPixelInterpolated(x, y, indexAry[index][2] + R / (x0 - x1)*count, indexAry[index][3] + G / (x0 - x1) * count, indexAry[index][4] + B / (x0 - x1) * count);
				++count;
			}
		}
		if (m > 1) {
			int d = 2 * dx - dy;
			int delE = 2 * dx;
			int delNE = 2 * (dx - dy);
			int x = x0, y = y0;

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
				setPixelInterpolated(x, y, indexAry[index][2] + R / (x0 - x1)*count, indexAry[index][3] + G / (x0 - x1) * count, indexAry[index][4] + B / (x0 - x1) * count);
				++count;
			}
		}
		if (m >= -1 && m < 0) {
			int d = 2 * dy + dx;
			int delE = 2 * dy;
			int delNE = 2 * (dy + dx);
			int x = x0, y = y0;

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
				setPixelInterpolated(x, y, indexAry[index][2] + R / (x0 - x1)*count, indexAry[index][3] + G / (x0 - x1) * count, indexAry[index][4] + B / (x0 - x1) * count);
				++count;
			}
		}
		if (m < -1) {
			int d = 2 * dx + dy;
			int delE = 2 * dx;
			int delNE = 2 * (dx + dy);
			int x = x0, y = y0;

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
				setPixelInterpolated(x, y, indexAry[index][2] + R / (x0 - x1)*count, indexAry[index][3] + G / (x0 - x1) * count, indexAry[index][4] + B / (x0 - x1) * count);
				++count;
			}
		}
		if (flag) {
			swap(indexAry[index][2], indexAry[index + 1][2]);
			swap(indexAry[index][3], indexAry[index + 1][3]);
			swap(indexAry[index][4], indexAry[index + 1][4]);
		}
	}
}
void crow() {
	if (ispolygon) {
		int iMin = 0;
		for (int i = 1; i < n - 1; ++i) {
			if (indexAry[i][1] < indexAry[iMin][1]) {
				iMin = i;
			}
		}
		scanY(iMin);
	}
}
void scanY(int iMin) {
	int li, ri;
	int ly, ry;
	float l[5], dl[5];
	float r[5], dr[5];
	int rem;
	int y;
	li = ri = iMin;
	ly = ry = y = ceil(indexAry[iMin][1]);
	for (rem = n; rem > 0; ) {
		while (ly <= y && rem > 0) {
			rem--;
			iMin = li - 1;
			if (iMin <= 0) {
				iMin = n - 1;
			}
			ly = ceil(indexAry[iMin][1]);
			if (ly > y) {
				differenceY(indexAry[li], indexAry[iMin], l, dl, y);
			}
			li = iMin;
		}
		while (ry <= y && rem > 0) {
			rem--;
			iMin = ri + 1;
			if (iMin > n - 1) {
				iMin = 0;
			}
			ry = ceil(indexAry[iMin][1]);
			if (ry > y) {
				differenceY(indexAry[ri], indexAry[iMin], r, dr, y);
			}
			ri = iMin;
		}
		for (; y < ly && y < ry; ++y) {
			scanX(l, r, y);
			increment(l, dl);
			increment(r, dr);
		}
	}
}
void differenceY(float* v1, float* v2, float* l, float* dl, int y){
	difference(v1,v2,l,dl,(v2[1]-v1[1]),y-v1[1]);
}
void differenceX(float* v1, float* v2, float* e, float* de, int x) {
	difference(v1, v2, e, de, (v2[0] - v1[0]), x - v1[0]);
}
void difference(float* v1, float* v2, float* e, float* de, float d, float f) {
	de[0] = (v2[0] - v1[0]) / d;
	e[0] = v1[0] + f * de[0];
	if (!isLine) {
		de[2] = (v2[2] - v1[2]) / d;
		e[2] = v1[2] + f * de[2];
		de[3] = (v2[3] - v1[3]) / d;
		e[3] = v1[3] + f * de[3];
		de[4] = (v2[4] - v1[4]) / d;
		e[4] = v1[4] + f * de[4];
	}
}
void increment(float* edge, float* delta) {
	edge[0] += delta[0];
	edge[2] += delta[2];
	edge[3] += delta[3];
	edge[4] += delta[4];
}
void scanX(float* l, float* r, int y) {
	int x, lx, rx;
	float s[5];
	float ds[5];
	lx = ceil(l[0]);
	rx = ceil(r[0]);
	bool flag = true;
	
	if (lx < rx) {
		differenceX(l, r, s, ds, lx);
		for (x = lx; x < rx; ++x) {
			/*for (int i = 0; i < n; ++i) {
				if (indexAry[i][0] == x && indexAry[i][1] == y && flag && (x!=lx || x!=rx) ) {
					flag = false;
					break;
				}
				if (indexAry[i][0] == x && indexAry[i][1] == y && !flag && (x != lx || x != rx) ) {
					flag = true;
					break;
				}
			}
			if (flag) {
				setPixelInterpolated(x, y, s[2], s[3], s[4]);
			}*/
			setPixelInterpolated(x, y, s[2], s[3], s[4]);
			increment(s, ds);
		}
	}
}
void setPixelInterpolated(int x, int y, GLfloat R, GLfloat G, GLfloat B) {
	glBegin(GL_QUADS);
	glColor3f(R, G, B);
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
void processMainMenu(int option) {
	// nothing to do in here
	// all actions are for submenus
}
void processGridMenu(int option) {
	switch (option) {
	case 1: //line
		isLine = true;
		glutPostRedisplay();
		break;
	case 2: //polygon
		isLine = false;
		glutPostRedisplay();
		break;
	case 3: //10X10
		gridCount = 10;
		glutPostRedisplay();
		break;
	case 4: //15X15
		gridCount = 15;
		glutPostRedisplay();
		break;
	case 5: //20X20
		gridCount = 20;
		glutPostRedisplay();
		break;
	case 6: //Reset
		gridCount = 10;
		isMouseClicked = false;
		ispolygon = false;
		for (int i = 0; i < n; ++i) {
			indexAry[i][0] = 0;
			indexAry[i][1] = 0;
			indexAry[i][2] = 0;
			indexAry[i][3] = 0;
			indexAry[i][4] = 0;
		}
		n = 0;
		glutPostRedisplay();
		break;
	}
}
void createPopupMenus() {

	gridMenu = glutCreateMenu(processGridMenu);
	glutAddMenuEntry("lines", 1);
	glutAddMenuEntry("polygon", 2);
	glutAddMenuEntry("10X10", 3);
	glutAddMenuEntry("15X15", 4);
	glutAddMenuEntry("20X20", 5);
	glutAddMenuEntry("Reset", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}