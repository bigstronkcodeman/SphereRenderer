#include "Vec3.h"

Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float xin, float yin, float zin)
{
	x = xin;
	y = yin;
	z = zin;
}

Vec3::Vec3(const Vec3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vec3 Vec3::crossProduct(const Vec3& v, const Vec3& w)
{
	Vec3 result;
	result.x = v.y * w.z - v.z * w.y;
	result.y = v.z * w.x - v.x * w.z;
	result.z = v.x * w.y - v.y * w.x;
	return result;
}

void Vec3::scale(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vec3::normalize()
{
	float magnitude = sqrt(x * x + y * y + z * z);
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}

Vec3 Vec3::operator+(const Vec3& v)
{
	Vec3 w;
	w.x = x + v.x;
	w.y = y + v.y;
	w.z = z + v.z;
	return w;
} 
