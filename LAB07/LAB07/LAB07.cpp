#include <iostream>
#include "C:\freeglut\include\GL\freeglut.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

String ImgPath = "D:\\saijo\\CG\\LAB07\\block\\";

GLuint textures[4];

// Called to draw scene
void RenderScene(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, -25.3f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, -25.3f, 100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, -25.3f, 100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100.0f, -25.3f, -100.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(25.0f, -25.0f, -25.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-25.0f, -25.0f, 25.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Flush drawing commands
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenTextures(4, textures);
	Mat image[4];
	// Load the texture objects
	image[0] = imread(ImgPath + "floor.jpg");
	flip(image[0], image[0], 0);
	glGenTextures(4, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0].cols, image[0].rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image[0].ptr());
	//glGenerateTextureMipmap(GL_TEXTURE_2D);

	image[1] = imread(ImgPath + "Block4.jpg");

	flip(image[1], image[1], 0);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1].cols, image[1].rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image[1].ptr());
	//glGenerateTextureMipmap(GL_TEXTURE_2D);

	image[2] = imread(ImgPath + "Block5.jpg");
	flip(image[2], image[2], 0);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2].cols, image[2].rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image[2].ptr());
	//glGenerateTextureMipmap(GL_TEXTURE_2D);

	image[3] = imread(ImgPath + "Block6.jpg");
	flip(image[3], image[3], 0);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[3].cols, image[3].rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image[3].ptr());
	//glGenerateTextureMipmap(GL_TEXTURE_2D);

}

void ChangeSize(int w, int h)
{
	// Calculate new clipping volume
	GLfloat windowWidth;
	GLfloat windowHeight;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	// Keep the square square
	if (w <= h)
	{
		windowHeight = 100.0f*(GLfloat)h / (GLfloat)w;
		windowWidth = 100.0f;
	}
	else
	{
		windowWidth = 100.0f*(GLfloat)w / (GLfloat)h;
		windowHeight = 100.0f;
	}
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	glOrtho(-100.0f, windowWidth, -100.0f, windowHeight, -200.0f, 200.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(330.0f, 0.0f, 1.0f, 0.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Effects Demo");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	SetupRC();

	glutMainLoop();
	//glDeleteTextures(4, textures);
	return 0;
}
