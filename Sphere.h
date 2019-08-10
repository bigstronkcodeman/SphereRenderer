#define _USE_MATH_DEFINES
#pragma once

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <vector>
#include <math.h>
#include "Vec3.h"

using namespace std;

class Sphere
{
private:
	vector<vector<Vec3>> uvMap;
	vector<vector<Vec3>> centroids;
	vector<vector<Vec3>> surfaceNormals;
	vector<vector<Vec3>> vertexNormals;
	Vec3 center;
	int slices;
	int rings;
	float radius;

	void initVertices();
	void initSurfaceNormals();
	void initVertexNormals();
	Vec3 getCentroid(const Vec3& p1, const Vec3& p2, const Vec3& p3);
	Vec3 triangleSurfaceNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3);

public:
	Sphere();
	Sphere(float x, float y, float z);
	Sphere(float x, float y, float z, float r);
	Sphere(float x, float y, float z, int numSlices, int numRings);
	Sphere(float x, float y, float z, float r, int numSlices, int numRings);
	Sphere(const Sphere& other);

	void drawWireframe();
	void drawPolygon();
	void drawSurfaceNormals();
	void drawVertexNormals();

	void operator=(const Sphere& other);
};