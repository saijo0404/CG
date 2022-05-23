/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
using namespace std;
/***freeglut***/
#include "C:\freeglut\include\GL\freeglut.h"
#include <math.h>

#define X .525731112119133606 
#define Z .850650808352039932


static GLfloat vdata[12][3] = {
	{ -X, 0.0, Z }, { X, 0.0, Z }, { -X, 0.0, -Z }, { X, 0.0, -Z },
	{ 0.0, Z, X }, { 0.0, Z, -X }, { 0.0, -Z, X }, { 0.0, -Z, -X },
	{ Z, X, 0.0 }, { -Z, X, 0.0 }, { Z, -X, 0.0 }, { -Z, -X, 0.0 }
};

static GLuint tindices[20][3] = {
	{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4}, {1, 8, 4},
	{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
	{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
	{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}
};

GLfloat d1[3], d2[3], n[3];

GLfloat LightPosition[] = { 5.0f, 5.0f, 0.0f, 1.0f }; 
GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; 
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
bool isLine = false;
long depth = 0;

void init()
{
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
	GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
	GLfloat mat_shininess = { 20.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glShadeModel(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_LIGHT0); /* enable light 0 */

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0.1f, 0, 0, 0, 0, 1, 0);
}
void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void normalize(float v[3])
{
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= d; v[1] /= d; v[2] /= d;
}
void normCrossProd(float u[3], float v[3], float out[3])
{
	out[0] = u[1] * v[2] - u[2] * v[1];
	out[1] = u[2] * v[0] - u[0] * v[2];
	out[2] = u[0] * v[1] - u[1] * v[0];
	normalize(out);
}
void icoNormVec(int i)
{
	for (int k = 0; k < 3; k++) {
		d1[k] = vdata[tindices[i][0]][k] - vdata[tindices[i][1]][k];
		d2[k] = vdata[tindices[i][1]][k] - vdata[tindices[i][2]][k];
	}
	normCrossProd(d1, d2, n);
	glNormal3fv(n);
}

void subdivide(float *v1, float *v2, float *v3, long depth) 
{
	GLfloat v12[3], v23[3], v31[3];
	GLint i;
	if (0 >= depth){
		glBegin(GL_TRIANGLES);
		glNormal3fv(v1);
		glVertex3fv(v1);
		glNormal3fv(v2);
		glVertex3fv(v2);
		glNormal3fv(v3);
		glVertex3fv(v3);
		glEnd();
		return;
	}
	for (i = 0; i < 3; i++)
	{
		v12[i] = (v1[i] + v2[i]) / 2.0;
		v23[i] = (v2[i] + v3[i]) / 2.0;
		v31[i] = (v3[i] + v1[i]) / 2.0;
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'l':
			if (isLine) {
				isLine = false;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glutPostRedisplay();
			}
			else {
				isLine = true;
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glutPostRedisplay();
			}
			break;
		case 'z':
			xrot = 0, yrot = 0;
			depth += 1;
			break;
		case 'x':
			xrot = 0, yrot = 0;
			depth -= 1;
			break;
		case 'a':
			xrot = 0, yrot = -1;
			break;
		case 'd':
			xrot = 0, yrot = 1;
			break;
		case 'w':
			xrot = -1, yrot = 0;
			break;
		case 's':
			xrot = 1, yrot = 0;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f); 
	glRotatef(xrot, 1.0f, 0.0f, 0.0f); 

	glViewport(0, 0, 400, 400);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; i++) {
		icoNormVec(i);
		glVertex3fv(&vdata[tindices[i][0]][0]);
		glVertex3fv(&vdata[tindices[i][1]][0]);
		glVertex3fv(&vdata[tindices[i][2]][0]);
	}
	glEnd();
	glViewport(400, 0, 400, 400);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; i++) {
		glNormal3fv(&vdata[tindices[i][0]][0]);
		glVertex3fv(&vdata[tindices[i][0]][0]);
		glNormal3fv(&vdata[tindices[i][1]][0]);
		glVertex3fv(&vdata[tindices[i][1]][0]);
		glNormal3fv(&vdata[tindices[i][2]][0]);
		glVertex3fv(&vdata[tindices[i][2]][0]);
	}
	glEnd();

	glViewport(800, 0, 400, 400);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; i++) {
		subdivide(&vdata[tindices[i][0]][0],&vdata[tindices[i][1]][0],&vdata[tindices[i][2]][0],depth);
	}
	glEnd();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Simple Rectangle");

	init();
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(RenderScene);

	glutMainLoop();
	return 0;
}