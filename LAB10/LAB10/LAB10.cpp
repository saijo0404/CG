// LAB10.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
using namespace std;

#include "C:\freeglut\include\GL\freeglut.h" 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
String ImgPath = "E:\\git\\l3v7a8c9\\CG\\LAB10\\LAB10\\";

#define NUM_SPHERES      30
GLfloat spheres[NUM_SPHERES][3];
// Light and material Data
GLfloat fLightPos[4] = { -100.0f, 100.0f, 50.0f, 1.0f };  // Point source
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat mShadowMatrix[16];


#define GROUND_TEXTURE  0
#define TORUS_TEXTURE   1
#define SPHERE_TEXTURE  2
#define NUM_TEXTURES    3
GLuint  textureObjects[NUM_TEXTURES];

const char *szTextureFiles[] = { "grass.jpg", "wood.jpg", "orb.jpg" };

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
// This function does any needed initialization on the rendering
// context. 
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
	/*m3dCrossProduct(planeEq, v1, v2);
	m3dNormalizeVector(planeEq);*/
	normCrossProd(v1, v2, planeEq);
	/*planeEq[0] = v1[1] * v2[2] - v1[2] * v2[1];
	planeEq[1] = v1[2] * v2[0] - v1[0] * v2[2];
	planeEq[2] = v1[0] * v2[1] - v1[1] * v2[0];
	GLfloat tmp = sqrt(planeEq[0] * planeEq[0] + planeEq[1] * planeEq[1] + planeEq[2] * planeEq[2]);
	planeEq[0] = planeEq[0] / tmp;
	planeEq[1] = planeEq[1] / tmp;
	planeEq[2] = planeEq[2] / tmp;*/
	// Back substitute to get D
	planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
	cout << planeEq[3] << endl;
}
void MakePlanarShadowMatrix(GLfloat* proj, const GLfloat* planeEq, const GLfloat* vLightPos)
{
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

//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	GLfloat vPoints[3][3] = { { 0.0f, -0.4f, 0.0f },
							 { 10.0f, -0.4f, 0.0f },
							 { 5.0f, -0.4f, -5.0f } };
	int iSphere;
	int i;

	// Grayish background
	glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

	// Clear stencil buffer with zero, increment by one whenever anybody
	// draws into it. When stencil function is enabled, only write where
	// stencil value is zero. This prevents the transparent shadow from drawing
	// over itself
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glClearStencil(0);
	glStencilFunc(GL_EQUAL, 0x0, 0x01);

	// Cull backs of polygons
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE_ARB);

	// Setup light parameters
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Calculate shadow matrix
	/*M3DVector4f pPlane;
	m3dGetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);
	m3dMakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);*/
	GLfloat pPlane[4];
	GetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);
	MakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);

	// Mostly use material tracking
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
	glMateriali(GL_FRONT, GL_SHININESS, 128);


	// Randomly place the sphere inhabitants
	for (iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
	{
		// Pick a random location between -20 and 20 at .1 increments
		//spheres[iSphere].SetOrigin(((float)((rand() % 400) - 200) * 0.1f), 0.0, (float)((rand() % 400) - 200) * 0.1f);
		spheres[iSphere][0] = (float)(((rand() % 400) - 200) * 0.1f);
		spheres[iSphere][1] = 0.0;
		spheres[iSphere][2] = (float)(((rand() % 400) - 200) * 0.1f);
	}

	// Set up texture maps
	glEnable(GL_TEXTURE_2D);
	glGenTextures(NUM_TEXTURES, textureObjects);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	Mat image[NUM_TEXTURES];
	for (i = 0; i < NUM_TEXTURES; i++)
	{
		GLbyte *pBytes;
		GLint iWidth, iHeight, iComponents;
		GLenum eFormat;

		glBindTexture(GL_TEXTURE_2D, textureObjects[i]);

		// Load this texture map
		/*pBytes = gltLoadTGA(szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat);
		gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
		free(pBytes);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
		image[i] = imread(ImgPath + szTextureFiles[i]);
		flip(image[i], image[i], 0);
		/*namedWindow("Display Window", CV_WINDOW_NORMAL);
		imshow("Display Window", image[i]);
		waitKey(0);*/

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[i].cols, image[i].rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image[i].ptr());

	}

}

////////////////////////////////////////////////////////////////////////
// Do shutdown for the rendering context
void ShutdownRC(void)
{
	// Delete the textures
	glDeleteTextures(NUM_TEXTURES, textureObjects);
}

void DrawGround(void)
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

void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks) {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glBegin(GL_SPHERE_MAP);
	glutSolidSphere(fRadius, iSlices, iStacks);
	//glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
GLfloat tors = 0.0f;
GLfloat ra = 0.0f, rla = 0.0f, rwa = 0.0f;
GLfloat la = 0.0f, lla = 0.0f, lwa = 0.0f;
GLfloat ll = 0.0f, rl = 0.0f;
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor) {
	
	glPushMatrix();
	glRotatef(tors * 2.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.05f, 0.0f);
	glutSolidCube(0.1);
	glTranslatef(0.0f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glTranslatef(0.0f, -0.1f, 0.0f);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
	glTranslatef(0.0f, 0.2f, -0.5f);
	glutSolidCube(0.1);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

			glPushMatrix();
			glPushAttrib(GL_CURRENT_BIT);
			//glColor3f(0, 1, 0);
			glRotatef(la * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.1f, -0.05f, 0.0f);
			glutSolidCube(0.1);
			glPopAttrib();
				glPushMatrix();
				glRotatef(lla * 2.0f, 0.0f, 0.0f, 1.0f);
				glTranslatef(-0.05f, -0.1f, 0.0f);
				glutSolidCube(0.1);
					glPushMatrix();
					glRotatef(lwa * 2.0f, 0.0f, 0.0f, 1.0f);
					glTranslatef(-0.05f, -0.1f, 0.0f);
					glutSolidCube(0.1);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();

			glPushMatrix();
			glPushAttrib(GL_CURRENT_BIT);
			//glColor3f(0, 1, 0);
			glRotatef(ra * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.1f, -0.05f, 0.0f);
			glutSolidCube(0.1);
			glPopAttrib();
				glPushMatrix();
				glRotatef(rla * 2.0f, 0.0f, 0.0f, 1.0f);
				glTranslatef(0.05f, -0.1f, 0.0f);
				glutSolidCube(0.1);
					glPushMatrix();
					glRotatef(rwa * 2.0f, 0.0f, 0.0f, 1.0f);
					glTranslatef(0.05f, -0.1f, 0.0f);
					glutSolidCube(0.1);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();


			glPushMatrix();
			glPushAttrib(GL_CURRENT_BIT);
			//glColor3f(0, 1, 0);
			glRotatef(ll * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.1f, -0.35f, 0.0f);
			glutSolidCube(0.1);
			glPopAttrib();
			glPushMatrix();
			//glRotatef(lla * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.05f, -0.1f, 0.0f);
			glutSolidCube(0.1);
			glPushMatrix();
			//glRotatef(lwa * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-0.05f, -0.1f, 0.0f);
			glutSolidCube(0.1);
			glPopMatrix();
			glPopMatrix();
			glPopMatrix();

			glPushMatrix();
			glPushAttrib(GL_CURRENT_BIT);
			//glColor3f(0, 1, 0);
			glRotatef(rl * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.1f, -0.35f, 0.0f);
			glutSolidCube(0.1);
			glPopAttrib();
			glPushMatrix();
			//glRotatef(rla * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.05f, -0.1f, 0.0f);
			glutSolidCube(0.1);
			glPushMatrix();
			//glRotatef(rwa * 2.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.05f, -0.1f, 0.0f);
			glutSolidCube(0.1);
			glPopMatrix();
			glPopMatrix();
			glPopMatrix();

	glPopMatrix();
	
}

///////////////////////////////////////////////////////////////////////
// Draw random inhabitants and the rotating torus/sphere duo
void DrawInhabitants(GLint nShadow)
{
	static GLfloat yRot = 0.0f;         // Rotation angle for animation
	GLint i;

	if (nShadow == 0)
	{
		yRot += 0.5f;
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
		glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color


	// Draw the randomly located spheres


	for (i = 0; i < NUM_SPHERES; i++)
	{
		
		glPushMatrix();
		//spheres[i].ApplyActorTransform();
		glTranslatef(spheres[i][0], spheres[i][1], spheres[i][2]);
		//glMultMatrixf(spheres[i]);
		gltDrawSphere(0.3f, 21, 11);
		glPopMatrix();

	}
	

	glPushMatrix();
	glTranslatef(0.0f, 0.1f, -2.5f);

	glPushMatrix();
	glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	gltDrawSphere(0.1f, 21, 11);
	glPopMatrix();
	
	if (nShadow == 0)
	{
		// Torus alone will be specular
		glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
	}

	//glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	//glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
	gltDrawTorus(0.35, 0.15, 61, 37);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
	glPopMatrix();

	if (nShadow != 0) {
		glPushMatrix();
		glMultMatrixf((GLfloat *)mShadowMatrix);
		glTranslatef(0.0f, 0.1f, -2.5f);

		glPushMatrix();
		glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(1.0f, 0.0f, 0.0f);
		gltDrawSphere(0.1f, 21, 11);
		glPopMatrix();

		if (nShadow == 0)
		{
			// Torus alone will be specular
			glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
		}
		//gltDrawTorus(0.35, 0.15, 61, 37);
		glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
		gltDrawTorus(0.35, 0.15, 61, 37);
		glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
		glPopMatrix();
		glPopMatrix();
	}
}

// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();
	//frameCamera.ApplyCameraTransform();

	// Position light before any other transformations
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

	// Draw the ground
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawGround();

	// Draw shadows first
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glPushMatrix();
	glMultMatrixf(mShadowMatrix);
	//DrawInhabitants(1);
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	// Draw inhabitants normally
	DrawInhabitants(0);
	DrawInhabitants(1);
	glPopMatrix();

	// Do the buffer Swap
	glutSwapBuffers();
}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
int timer_flag = 1;
void TimerFunction(int value)
{
	if (value == 0) { return; }
	// Redraw the scene with new coordinates
	glutPostRedisplay();
	glutTimerFunc(3, TimerFunction, timer_flag);
}

void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'p':
			if (timer_flag == 0) {
				timer_flag = 1;
				cout << "timer_flag : " << timer_flag << endl;
				glutTimerFunc(3, TimerFunction, timer_flag);
			}
			else {
				timer_flag = 0;
				cout << "timer_flag : " << timer_flag << endl;
			}
			break;
		case 'a':
			lwa += 0.1;
			break;
		case 'q':
			lwa -= 0.1;
			break;
		case 's':
			lla += 0.1;
			break;
		case 'w':
			lla -= 0.1;
			break;
		case 'd':
			la += 0.1;
			break;
		case 'e':
			la -= 0.1;
			break;
		case 'o':
			rwa += 0.1;
			break;
		case 'l':
			rwa -= 0.1;
			break;
		case 'i':
			rla += 0.1;
			break;
		case 'k':
			rla -= 0.1;
			break;
		case 'u':
			ra += 0.1;
			break;
		case 'j':
			ra -= 0.1;
			break;
		case 'r':
			ll += 0.1;
			break;
		case 'f':
			ll -= 0.1;
			break;
		case 'y':
			rl += 0.1;
			break;
		case 'h':
			rl -= 0.1;
			break;
		case 't':
			tors += 0.1;
			break;
		case 'g':
			tors -= 0.1;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}
void ChangeSize(int w, int h)
{
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

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL SphereWorld Demo + Texture Maps");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(myKeyboard);
	SetupRC();
	glutTimerFunc(33, TimerFunction, 1);

	glutMainLoop();

	ShutdownRC();

	return 0;
}