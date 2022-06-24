// FinalProject.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
using namespace std;

#include "C:\freeglut\include\GL\freeglut.h"
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

const char *objAry[] = { "gourd.obj", "octahedron.obj", "teapot.obj", "teddy.obj" };
float point[5000][6];
int vetexes[5000][3];
int vn = 0, fn = 0;
float boundingbox[3][2];
bool singleColor = true;
bool isboundingbox = true;

#define GROUND_TEXTURE  0
#define TORUS_TEXTURE   1
#define SUN_TEXTURE  2
#define NINESQUARE_TEXTURE  3
#define BASEBALL_TEXTURE  12
#define NUM_TEXTURES    13
GLuint  textureObjects[NUM_TEXTURES];
const char *szTextureFiles[] = { "grass.jpg", "wood.jpg", "2k_sun.jpg","1.jpg","2.jpg","3.jpg",
								"4.jpg","5.jpg","6.jpg","7.jpg","8.jpg","9.jpg","baseball.jpg" };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat fLightPos[4] = { -7.0f, 3.0f, -10.0f, 1.0f };
GLfloat shadowMat[16];

static GLfloat yRot = 0.0f;
static GLfloat xRot = 0.0f;
static GLfloat zBaseball = 0.0f;
static GLfloat fiveRot = 0.0f;
static GLfloat obj = 0.0f;
float normalvector[5000][3];

bool isReadObj = false;
bool isnormalvector = false;
bool isHit = false;
bool isPoint = false;
bool isLine = false;
bool isFace = true;
bool isExplode = false;
#define X .525731112119133606
#define Z .850650808352039932
static GLfloat vdata[12][3] = {
	{-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{-X,0.0,-Z},
	{0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
	{Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0}
};
static GLuint tindices[20][3] = {
	{1,4,0},{4,9,0},{4,9,5},{8,5,4},{1,8,4},
	{1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
	{3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
	{10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
};
GLfloat d1[3], d2[3], n[3];
long depth = 6;
void normalize(float v[3]) {
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= d; v[1] /= d; v[2] /= d;
}
void subdivide(float *v1, float *v2, float *v3, long depth) {
	GLfloat v12[3], v23[3], v31[3];
	GLint i;
	if (0 == depth) {
		glBegin(GL_TRIANGLES);
		glVertex3fv(v1);
		glVertex3fv(v2);
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

void readTexture() {
	glGenTextures(NUM_TEXTURES, textureObjects);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	Mat image[NUM_TEXTURES];
	for (int i = 0; i < NUM_TEXTURES; i++) {
		glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
		image[i] = imread(szTextureFiles[i]);
		flip(image[i], image[i], 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[i].cols, image[i].rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image[i].ptr());
	}
}
void normCrossProd(float u[3], float v[3], float out[3]) {
	out[0] = u[1] * v[2] - u[2] * v[1];
	out[1] = u[2] * v[0] - u[0] * v[2];
	out[2] = u[0] * v[1] - u[1] * v[0];
	normalize(out);
}
void GetPlaneEquation(GLfloat* planeEq, const GLfloat* p1, const GLfloat* p2, const GLfloat* p3)
{
	// Get two vectors... do the cross product
	GLfloat v1[3], v2[3];

	// V1 = p3 - p1
	v1[0] = p3[0] - p1[0];
	v1[1] = p3[1] - p1[1];
	v1[2] = p3[2] - p1[2];

	// V2 = P2 - p1
	v2[0] = p2[0] - p1[0];
	v2[1] = p2[1] - p1[1];
	v2[2] = p2[2] - p1[2];

	// Unit normal to plane - Not sure which is the best way here
	normCrossProd(v1, v2, planeEq);
	// Back substitute to get D
	planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
	cout << planeEq[3] << endl;
}
void MakePlanarShadowMatrix(GLfloat* proj, const GLfloat* planeEq, const GLfloat* vLightPos) {
	// These just make the code below easier to read. They will be 
	// removed by the optimizer.	
	float a = planeEq[0];
	float b = planeEq[1];
	float c = planeEq[2];
	float d = planeEq[3];

	float dx = -vLightPos[0];
	float dy = -vLightPos[1];
	float dz = -vLightPos[2];

	// Now build the projection matrix
	proj[0] = b * dy + c * dz;
	proj[1] = -a * dy;
	proj[2] = -a * dz;
	proj[3] = 0.0;

	proj[4] = -b * dx;
	proj[5] = a * dx + c * dz;
	proj[6] = -b * dz;
	proj[7] = 0.0;

	proj[8] = -c * dx;
	proj[9] = -c * dy;
	proj[10] = a * dx + b * dy;
	proj[11] = 0.0;

	proj[12] = -d * dx;
	proj[13] = -d * dy;
	proj[14] = -d * dz;
	proj[15] = a * dx + b * dy + c * dz;
	// Shadow matrix ready
}
void SetupRC() {
	glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glClearStencil(0);
	glStencilFunc(GL_EQUAL, 0x0, 0x01);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	readTexture();
}
void ChangeSize(int w, int h) {
	GLfloat fAspect;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void ShutdownRC(void)
{
	// Delete the textures
	glDeleteTextures(NUM_TEXTURES, textureObjects);
}

void drawBoundingbox() {
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][0], boundingbox[2][0]);
	glVertex3f(boundingbox[0][0], boundingbox[1][0], boundingbox[2][1]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][0], boundingbox[2][1]);
	glVertex3f(boundingbox[0][0], boundingbox[1][1], boundingbox[2][1]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][1], boundingbox[2][1]);
	glVertex3f(boundingbox[0][0], boundingbox[1][1], boundingbox[2][0]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][1], boundingbox[2][0]);
	glVertex3f(boundingbox[0][0], boundingbox[1][0], boundingbox[2][0]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][1], boundingbox[1][0], boundingbox[2][0]);
	glVertex3f(boundingbox[0][1], boundingbox[1][0], boundingbox[2][1]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][1], boundingbox[1][0], boundingbox[2][1]);
	glVertex3f(boundingbox[0][1], boundingbox[1][1], boundingbox[2][1]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][1], boundingbox[1][1], boundingbox[2][1]);
	glVertex3f(boundingbox[0][1], boundingbox[1][1], boundingbox[2][0]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][1], boundingbox[1][1], boundingbox[2][0]);
	glVertex3f(boundingbox[0][1], boundingbox[1][0], boundingbox[2][0]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][1], boundingbox[2][1]);
	glVertex3f(boundingbox[0][1], boundingbox[1][1], boundingbox[2][1]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][1], boundingbox[2][0]);
	glVertex3f(boundingbox[0][1], boundingbox[1][1], boundingbox[2][0]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][0], boundingbox[2][0]);
	glVertex3f(boundingbox[0][1], boundingbox[1][0], boundingbox[2][0]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(boundingbox[0][0], boundingbox[1][0], boundingbox[2][1]);
	glVertex3f(boundingbox[0][1], boundingbox[1][0], boundingbox[2][1]);
	glEnd();
	glPopAttrib();
}
void draw() {
	float u[3], v[3];
	if (isHit && isExplode == false) { obj += 0.01; }
	if (isHit && isExplode) { obj -= 0.01; }
	for (int i = 0; i < fn; i++) {
		if (isnormalvector) {
			for (int j = 0; j < 3; ++j) {
				v[j] = point[vetexes[i][1] - 1][j] - point[vetexes[i][0] - 1][j];
				u[j] = point[vetexes[i][2] - 1][j] - point[vetexes[i][0] - 1][j];
				normCrossProd(u, v, normalvector[i]);
			}
		}
		glPushMatrix();
		glPushAttrib(GL_CURRENT_BIT);
		if (isPoint) { glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); }
		if (isLine) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		if (isFace) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
		if (isHit) {
			if (obj > 1) { isExplode = true; }
			if (isExplode && obj <= 0) {
				obj = 0;
				isHit = false;
				isExplode = false;
				xRot = 0.0f;
				zBaseball = 0.0f;
				fiveRot = 0.0f;
			}
			glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(-1 * normalvector[i][0] * obj, -1 * normalvector[i][1] * obj, -1 * normalvector[i][2] * obj);
		}
		glBegin(GL_TRIANGLES);
		if (singleColor) {
			glColor3f(1, 0, 0); glVertex3f(point[vetexes[i][0] - 1][0], point[vetexes[i][0] - 1][1], point[vetexes[i][0] - 1][2]);
			glColor3f(1, 0, 0); glVertex3f(point[vetexes[i][1] - 1][0], point[vetexes[i][1] - 1][1], point[vetexes[i][1] - 1][2]);
			glColor3f(1, 0, 0); glVertex3f(point[vetexes[i][2] - 1][0], point[vetexes[i][2] - 1][1], point[vetexes[i][2] - 1][2]);
		}
		else {
			glColor3f(point[vetexes[i][0] - 1][3], point[vetexes[i][0] - 1][4], point[vetexes[i][0] - 1][5]);
			glVertex3f(point[vetexes[i][0] - 1][0], point[vetexes[i][0] - 1][1], point[vetexes[i][0] - 1][2]);
			glColor3f(point[vetexes[i][1] - 1][3], point[vetexes[i][1] - 1][4], point[vetexes[i][1] - 1][5]);
			glVertex3f(point[vetexes[i][1] - 1][0], point[vetexes[i][1] - 1][1], point[vetexes[i][1] - 1][2]);
			glColor3f(point[vetexes[i][2] - 1][3], point[vetexes[i][2] - 1][4], point[vetexes[i][2] - 1][5]);
			glVertex3f(point[vetexes[i][2] - 1][0], point[vetexes[i][2] - 1][1], point[vetexes[i][2] - 1][2]);
		}
		glEnd();
		glPopAttrib();
		glPopMatrix();
	}
	if (isboundingbox) {
		drawBoundingbox();
	}
	isnormalvector = false;
}
void DrawGround()
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLfloat iStrip, iRun;
	GLfloat s = 0.0f;
	GLfloat t = 0.0f;
	GLfloat texStep = 1.0f / (fExtent * .075f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
	{
		t = 0.0f;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLE_STRIP);
		for (iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
		{
			glTexCoord2f(s, t);
			glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
			glVertex3f(iStrip, y, iRun);
			glTexCoord2f(s + texStep, t);
			glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
			glVertex3f(iStrip + fStep, y, iRun);
			t += texStep;
		}
		glEnd();
		s += texStep;
	}
	glDisable(GL_TEXTURE_2D);
}
void DrawRobot() {

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 1);
	glTranslatef(1.0f, 0.6f, 5.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	glutSolidCube(0.1);
	glTranslatef(0.0f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glTranslatef(0.0f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPopAttrib();
	glPopMatrix();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 1, 1);
	glTranslatef(1.0f, 0.8f, 5.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	glutSolidCube(0.1);
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 1);
	glPushMatrix();
	glTranslatef(-0.1f, -0.05f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(-0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(-0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();

	xRot += 0.5f;
	glRotatef(-xRot * 2.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.1f, -0.05f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(0.05f, -0.1f, 0.0f);
	glScalef(0.04f, 0.04f, 0.04f);

	if (xRot == 140) {
		zBaseball += 0.1f;
		xRot -= 0.5f;
		if (zBaseball > 18.0f&& isHit == false) {
			isHit = true;
		}
		glTranslatef(-0.08*zBaseball, -6.4*zBaseball, -1.5*zBaseball);
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[SUN_TEXTURE]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glRotatef(-150.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(150.0f, 1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; i++) {
		subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], depth);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1f, -0.35f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(-0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(-0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1f, -0.35f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPushMatrix();
	glTranslatef(0.05f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopAttrib();
	glPopMatrix();
}
void DrawNinesquare() {
	int count = 0;

	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);
	glEnable(GL_TEXTURE_2D);
	for (int j = 2; j > -1; --j) {
		for (int i = 0; i < 3; ++i) {
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureObjects[NINESQUARE_TEXTURE + count]);
			if (count == 4 && isHit) {
				fiveRot += 0.1f;
				if (fiveRot >= 4) {
					fiveRot -= 0.1f;
					if (isReadObj == false) {
						isHit = false;
						isExplode = false;
						xRot = 0.0f;
						zBaseball = 0.0f;
						fiveRot = 0.0f;
					}
				}
				glRotatef(-fiveRot * 2.0f, 1.0f, 0.0f, 0.0f);
			}
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i + 1, j + 2, -10.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i + 1, j + 1, -10.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i + 2, j + 1, -10.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i + 2, j + 2, -10.0f);
			glEnd();
			glPopMatrix();
			count += 1;
		}
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void DarwSun() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[SUN_TEXTURE]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glScalef(0.1f, 0.1f, 0.1f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; i++) {
		subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], depth);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void RenderScence() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.f, 1.f, 10.f, 0, 0, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

	static GLfloat zRot = 0.0f;
	glPushMatrix();
	zRot += 0.1f;
	glRotatef(-zRot * 2.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(fLightPos[0], fLightPos[1], fLightPos[2]);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
	DarwSun();
	glPopMatrix();

	glPushMatrix();
	if (isHit == false) {
		yRot += 0.1f;
		glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
	}
	draw();
	glPopMatrix();

	DrawGround();
	DrawRobot();
	DrawNinesquare();

	glutSwapBuffers();
}

void readObj(string objName) {

	vn = 0;
	fn = 0;
	for (int i = 0; i < 3; ++i) {
		boundingbox[i][0] = FLT_MAX;
		boundingbox[i][1] = -1 * FLT_MAX;
	}

	FILE * file = fopen(objName.c_str(), "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}
	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			fscanf(file, "%f %f %f\n", &point[vn][0], &point[vn][1], &point[vn][2]);
			point[vn][3] = (double)rand() / (RAND_MAX + 1.0);
			point[vn][4] = (double)rand() / (RAND_MAX + 1.0);
			point[vn][5] = (double)rand() / (RAND_MAX + 1.0);
			if (point[vn][0] < boundingbox[0][0]) { boundingbox[0][0] = point[vn][0]; }
			if (point[vn][0] > boundingbox[0][1]) { boundingbox[0][1] = point[vn][0]; }
			if (point[vn][1] < boundingbox[1][0]) { boundingbox[1][0] = point[vn][1]; }
			if (point[vn][1] > boundingbox[1][1]) { boundingbox[1][1] = point[vn][1]; }
			if (point[vn][2] < boundingbox[2][0]) { boundingbox[2][0] = point[vn][2]; }
			if (point[vn][2] > boundingbox[2][1]) { boundingbox[2][1] = point[vn][2]; }
			vn += 1;
		}
		else if (strcmp(lineHeader, "f") == 0) {
			fscanf(file, "%d %d %d\n", &vetexes[fn][0], &vetexes[fn][1], &vetexes[fn][2]);
			fn += 1;
		}
	}

	float Normalization[3][2];
	for (int i = 0; i < 3; ++i) {
		Normalization[i][0] = (boundingbox[i][1] - boundingbox[i][0]);
		Normalization[i][1] = 0 - (boundingbox[i][1] + boundingbox[i][0]) / 2;
	}
	for (int i = 0; i < vn; ++i) {
		point[i][0] = (point[i][0] + Normalization[0][1]) / Normalization[0][0];
		point[i][1] = (point[i][1] + Normalization[1][1]) / Normalization[1][0];
		point[i][2] = (point[i][2] + Normalization[2][1]) / Normalization[2][0];
	}
	for (int i = 0; i < 3; ++i) {
		boundingbox[i][0] = (boundingbox[i][0] + Normalization[i][1]) / Normalization[i][0];
		boundingbox[i][1] = (boundingbox[i][1] + Normalization[i][1]) / Normalization[i][0];
	}

	readTexture();
}
void menuSelect(int option) {
	switch (option)
	{
	case 0:
		break;
	case 1:

		break;
	case 2:

		break;
	case 3:

		break;
	default:
		break;
	}
}
void subMenu1(int option) {
	string objName;
	switch (option)
	{
	case 0:
		isReadObj = true;
		isnormalvector = true;
		objName = "gourd.obj";
		break;
	case 1:
		isReadObj = true;
		isnormalvector = true;
		objName = "octahedron.obj";
		break;
	case 2:
		isReadObj = true;
		isnormalvector = true;
		objName = "teapot.obj";
		break;
	case 3:
		isReadObj = true;
		isnormalvector = true;
		objName = "teddy.obj";
		break;
	default:
		break;
	}
	readObj(objName);
	glutPostRedisplay();
}
void subMenu2(int option) {
	switch (option)
	{
	case 0:
		isPoint = true;
		isLine = false;
		isFace = false;
		break;
	case 1:
		isPoint = false;
		isLine = true;
		isFace = false;
		break;
	case 2:
		isPoint = false;
		isLine = false;
		isFace = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void subMenu3(int option) {
	switch (option)
	{
	case 0:
		singleColor = true;
		glShadeModel(GL_FLAT);
		break;
	case 1:
		singleColor = false;
		glShadeModel(GL_SMOOTH);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void subMenu4(int option) {
	switch (option)
	{
	case 0:
		isboundingbox = true;
		break;
	case 1:
		isboundingbox = false;
		break;
	default:
		break;
	}
}
void buildPopupMenu() {
	//create sub-menu 1
	int submenu1 = glutCreateMenu(subMenu1);
	glutAddMenuEntry("groud", 0);
	glutAddMenuEntry("octahedron", 1);
	glutAddMenuEntry("teapot", 2);
	glutAddMenuEntry("teddy", 3);
	//create sub-menu 2
	int submenu2 = glutCreateMenu(subMenu2);
	glutAddMenuEntry("Point", 0);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Face", 2);
	//create sub-menu 3
	int submenu3 = glutCreateMenu(subMenu3);
	glutAddMenuEntry("Single Color", 0);
	glutAddMenuEntry("Random colors", 1);
	//create sub-menu 4
	int submenu4 = glutCreateMenu(subMenu4);
	glutAddMenuEntry("On", 0);
	glutAddMenuEntry("Off", 1);

	int menu_id = glutCreateMenu(menuSelect);
	glutAddSubMenu("File", submenu1);
	glutAddSubMenu("Render Mode", submenu2);
	glutAddSubMenu("Color Mode", submenu3);
	glutAddSubMenu("Bounding Box", submenu4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void TimerFunction(int value)
{
	if (value == 0) { return; }
	// Redraw the scene with new coordinates
	glutPostRedisplay();
	glutTimerFunc(3, TimerFunction, 1);
}

int main(int argc, char* argv[])
{
	if (argv[1]) {
		isReadObj = true;
		isnormalvector = true;
		readObj(argv[1]);
	}
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();
	glutReshapeFunc(ChangeSize);
	buildPopupMenu();
	glutDisplayFunc(RenderScence);
	glutTimerFunc(33, TimerFunction, 1);
	glutMainLoop();
	ShutdownRC();
}