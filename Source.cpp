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

//Vec3 operator+ (const Vec3& u, const Vec3& v)
//{
//	Vec3 w;
//	w.x = u.x + v.x;
//	w.y = u.y + v.y;
//	w.z = u.z + v.z;
//	return w;
//}

//void scale(Vec3& v, float scalar)
//{
//	v.x *= scalar;
//	v.y *= scalar;
//	v.z *= scalar;
//}
//
//void normalize(Vec3& v)
//{
//	float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
//	v.x /= magnitude;
//	v.y /= magnitude;
//	v.z /= magnitude;
//}

//Vec3 crossProduct(const Vec3& v, const Vec3& w)
//{
//	Vec3 result;
//	result.x = v.y * w.z - v.z * w.y;
//	result.y = v.z * w.x - v.x * w.z;
//	result.z = v.x * w.y - v.y * w.x;
//	return result;
//}
//
//Vec3 triangleCentroid(const Vec3& p1, const Vec3& p2, const Vec3& p3)
//{
//	Vec3 centroid;
//	centroid.x = (p1.x + p2.x + p3.x) / 3;
//	centroid.y = (p1.y + p2.y + p3.y) / 3;
//	centroid.z = (p1.z + p2.z + p3.z) / 3;
//	return centroid;
//}

//Vec3 triangleNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3)
//{
//	Vec3 v;
//	v.x = p2.x - p1.x;
//	v.y = p2.y - p1.y;
//	v.z = p2.z - p1.z;
//
//	Vec3 w;
//	w.x = p3.x - p1.x;
//	w.y = p3.y - p1.y;
//	w.z = p3.z - p1.z;
//
//	Vec3 centroid = triangleCentroid(p1, p2, p3);
//	Vec3 n = crossProduct(v, w);
//	//normalize(n);
//	n.x += centroid.x;
//	n.y += centroid.y;
//	n.z += centroid.z;
//
//	return n;
//}

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

//void drawSphere(float xin, float yin, float zin, float r, int numRings, int numSlices)
//{
//	float ringStep = (2 * M_PI) / numSlices;
//	float sliceStep = M_PI / numRings;
//
//	vector<vector<Vec3>> uvMap;
//	uvMap.reserve(numRings + 1);
//
//	for (int slice = 0; slice <= numRings; slice++)
//	{
//		Vec3 p;
//		float sliceAngle = (M_PI / 2) - ((double)slice * sliceStep);
//		float base = r * cosf(sliceAngle);
//		p.y = r * sinf(sliceAngle) + yin;
//
//		uvMap.push_back(vector<Vec3>());
//		uvMap[slice].reserve(numSlices + 1);
//
//		for (int ring = 0; ring <= numSlices; ring++)
//		{
//			float ringAngle = ring * ringStep;
//			p.x = base * sinf(ringAngle) + xin;
//			p.z = base * cosf(ringAngle) + zin;
//
//			uvMap[slice].push_back(p);
//		}
//	}
//
//	float outerRatio = (float)1 / (uvMap.size() - 1);
//
//	vector<vector<Vec3>> normals;
//
//	for (int i = 0; i < uvMap.size() - 1; i++)
//	{
//		float innerRatio = (float)1 / (uvMap[i].size() - 1);
//		normals.push_back(vector<Vec3>());
//		for (int j = 0;  j < uvMap[i].size() - 1; j++)
//		{
//			Vec3 k1 = uvMap[i][j];
//			Vec3 k2 = uvMap[i][j + 1];
//			Vec3 k3 = uvMap[i + 1][j + 1];
//			Vec3 k4 = uvMap[i + 1][j];
//
//			glColor3f(i * outerRatio, 1 / (j * innerRatio), 0.375);
//
//			if (polygon) glBegin(GL_POLYGON);
//			else glBegin(GL_LINE_LOOP);
//			//glColor3f((float)j / (uvMap[i].size() - 1), 1, (float)i / (uvMap.size() - 1));
//			glVertex3f(k1.x, k1.y, k1.z);
//			glVertex3f(k2.x, k2.y, k2.z);
//			//glVertex3f(k3.x, k3.y, k3.z);
//			glVertex3f(k4.x, k4.y, k4.z);
//			glEnd();
//			
//			if (polygon) glBegin(GL_POLYGON);
//			else glBegin(GL_LINE_STRIP);
//			//glVertex3f(k1.x, k1.y, k1.z);
//			glVertex3f(k2.x, k2.y, k2.z);
//			glVertex3f(k3.x, k3.y, k3.z);
//			glVertex3f(k4.x, k4.y, k4.z);
//			glEnd();
//
//			glColor3f(1, 0, 0);
//
//			Vec3 n;
//			Vec3 centroid;
//
//			if (i > 0)
//			{
//				n = triangleNormal(k4, k2, k1);
//				centroid = triangleCentroid(k4, k2, k1);
//				if (showNormals)
//				{
//					glBegin(GL_LINES);
//					glVertex3f(centroid.x, centroid.y, centroid.z);
//					//glVertex3f(0, 0, 0);
//					glVertex3f(n.x, n.y, n.z);
//					glEnd();
//				}
//				normals[i].push_back(n);
//			}
//
//			if (i < uvMap.size() - 2)
//			{
//				n = triangleNormal(k4, k3, k2);
//				centroid = triangleCentroid(k4, k3, k2);
//				if (showNormals)
//				{
//					glBegin(GL_LINES);
//					glVertex3f(centroid.x, centroid.y, centroid.z);
//					//glVertex3f(0, 0, 0);
//					glVertex3f(n.x, n.y, n.z);
//					glEnd();
//				}
//				normals[i].push_back(n);
//			}
//		}
//	}
//
//	if (showNormals)
//	{
//		vector<vector<Vec3>> vertexNormals;
//		vertexNormals.reserve(uvMap.size());
//
//		for (int i = 0; i < uvMap.size(); i++)
//		{
//			vertexNormals.push_back(vector<Vec3>());
//			vertexNormals[i].reserve(uvMap[i].size());
//
//			Vec3 sumVec;
//			for (int j = 0; j < uvMap[i].size(); j++)
//			{
//				if (i == 0 || i == uvMap.size() - 1)
//				{
//					int normali = i > 0 ? i - 1 : i;
//					for (int k = 0; k < normals[normali].size(); k++)
//					{
//						sumVec = sumVec + normals[normali][k];
//					}
//					normalize(sumVec);
//					glBegin(GL_LINES);
//					glColor3f(1, 1, 1);
//					glVertex3f(sumVec.x + uvMap[i][0].x, sumVec.y + uvMap[i][0].y, sumVec.z + uvMap[i][0].z);
//					glVertex3f(uvMap[i][0].x, uvMap[i][0].y, uvMap[i][0].z);
//					glEnd();
//					j = uvMap[normali].size();
//				}
//				else if (i == 1)
//				{
//					for (int k = 0; k < 2; k++)
//					{
//						sumVec = sumVec + normals[0][(k - 1) % normals[0].size()];
//					}
//
//					for (int k = 0; k < 3; k++)
//					{
//						sumVec = sumVec + normals[1][(k - 2) % normals[normals.size() - 1].size()];
//					}
//					normalize(sumVec);
//					glBegin(GL_LINES);
//					glColor3f(1, 1, 1);
//					glVertex3f(sumVec.x + uvMap[i][j].x, sumVec.y + uvMap[i][j].y, sumVec.z + uvMap[i][j].z);
//					glVertex3f(uvMap[i][j].x, uvMap[i][j].y, uvMap[i][j].z);
//					glEnd();
//				}
//				else if (i == uvMap.size() - 2)
//				{
//
//				}
//			}
//		}
//	}
//}

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

	//drawSphere(0, 0, 0, radius, ringNum, sliceNum);

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