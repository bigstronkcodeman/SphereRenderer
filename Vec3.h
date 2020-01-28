#pragma once
#include <math.h>

using namespace std;

class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3();
	Vec3(float xin, float yin, float zin);
	Vec3(const Vec3& other);

	static Vec3 crossProduct(const Vec3& v, const Vec3& w);

	void scale(float scalar);
	void normalize();

	Vec3 operator+(const Vec3& v);
}; 
