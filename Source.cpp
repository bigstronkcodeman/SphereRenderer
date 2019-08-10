#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <cstdlib>

#include "Sphere.h"

using namespace std;

int ringNum = 5;
int sliceNum = 4;
float radius = 1.5;
bool polygon = false;
bool showNormals = false;
bool showVertexNormals = false;

Sphere s(0, 0, 0, radius, sliceNum, ringNum);

void print2d(vector<vector<Vec3>> uvMap)
{
	for (int i = 0; i < uvMap.size(); i++)
	{
		for (int j = 0; j < uvMap[i].size(); j++)
		{
			cout << setprecision(3) << "(" << uvMap[i][j].x << ", " << uvMap[i][j].y << ", " << uvMap[i][j].z << ")";
			if (j < uvMap[i].size() - 1)
			{
				cout << ", ";
			}
		}
		cout << endl;
	}
}

void drawAxisVectors()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.2, 0, 0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.2, 0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.2);
	glEnd();
}

void init()
{
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-3, 3, -3, 3, -3, 3);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(30, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);


}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawAxisVectors();

	if (polygon) s.drawPolygon();
	else s.drawWireframe();

	if (showNormals) s.drawSurfaceNormals();

	if (showVertexNormals) s.drawVertexNormals();

	glFlush();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'R':
		ringNum++;
		s = Sphere(0, 0, 0, radius, sliceNum, ringNum);
		break;
	case 'r':
		if (ringNum > 1) ringNum--;
		s = Sphere(0, 0, 0, radius, sliceNum, ringNum);
		break;
	case 'S':
		sliceNum++;
		s = Sphere(0, 0, 0, radius, sliceNum, ringNum);
		break;
	case 's':
		if (sliceNum > 1) sliceNum--;
		s = Sphere(0, 0, 0, radius, sliceNum, ringNum);
		break;
	case 'p':
	case 'P':
		polygon = !polygon;
		break;
	case '-':
		if (radius > 0.5) radius -= 0.0125;
		s = Sphere(0, 0, 0, radius, sliceNum, ringNum);
		break;
	case '+':
		if (radius < 3) radius += 0.0125;
		s = Sphere(0, 0, 0, radius, sliceNum, ringNum);
		break;
	case 'n':
	case 'N':
		showNormals = !showNormals;
		break;
	case 'v':
	case 'V':
		showVertexNormals = !showVertexNormals;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		glRotatef(1, 0, 1, 0);
		break;
	case GLUT_KEY_LEFT:
		glRotatef(-1, 0, 1, 0);
		break;
	case GLUT_KEY_UP:
		glRotatef(1, 1, 0, 0);
		break;
	case GLUT_KEY_DOWN:
		glRotatef(-1, 1, 0, 0);
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char* args[])
{
	glutInit(&argc, args);
	glutInitWindowSize(1200, 1200);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Project");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	init();
	glutMainLoop();

	return 0;
}
