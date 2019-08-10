#include "Sphere.h"

Sphere::Sphere()
{
	slices = 25;
	rings = 25;
	radius = 1;
	initVertices();
}

Sphere::Sphere(float x, float y, float z)
{
	center = Vec3(x, y, z);
	slices = 25;
	rings = 25;
	radius = 1;
	initVertices();
}

Sphere::Sphere(float x, float y, float z, float r)
{
	center = Vec3(x, y, z);
	slices = 25;
	rings = 25;
	radius = r;
	initVertices();
}

Sphere::Sphere(float x, float y, float z, int numSlices, int numRings)
{
	center = Vec3(x, y, z);
	slices = numSlices > 2 ? numSlices : 3;
	rings = numRings > 2 ? numRings : 3;
	radius = 1;
	initVertices();
}

Sphere::Sphere(float x, float y, float z, float r, int numSlices, int numRings)
{
	center = Vec3(x, y, z);
	slices = numSlices > 2 ? numSlices : 3;
	rings = numRings > 2 ? numRings : 3;
	radius = r;
	initVertices();
	initSurfaceNormals();
	initVertexNormals();
}

Sphere::Sphere(const Sphere& other)
{
	center = other.center;
	slices = other.slices;
	rings = other.rings;
	radius = other.radius;

	for (int i = 0; i < other.uvMap.size(); i++)
	{
		uvMap.push_back(other.uvMap[i]);
	}

	for (int i = 0; i < other.surfaceNormals.size(); i++)
	{
		surfaceNormals.push_back(other.surfaceNormals[i]);
		centroids.push_back(other.centroids[i]);
	}

	for (int i = 0; i < other.vertexNormals.size(); i++)
	{
		vertexNormals.push_back(other.vertexNormals[i]);
	}
}



void Sphere::initVertices()
{
	float ringStep = (2 * M_PI) / slices;
	float sliceStep = M_PI / rings;

	uvMap.reserve(rings + 1);

	for (int slice = 0; slice <= rings; slice++)
	{
		Vec3 p;
		float sliceAngle = (M_PI / 2) - ((double)slice * sliceStep);
		float base = radius * cosf(sliceAngle);
		p.y = radius * sinf(sliceAngle) + center.y;

		uvMap.push_back(vector<Vec3>());
		uvMap[slice].reserve(slices + 1);

		for (int ring = 0; ring <= slices; ring++)
		{
			float ringAngle = ring * ringStep;
			p.x = base * sinf(ringAngle) + center.x;
			p.z = base * cosf(ringAngle) + center.z;

			uvMap[slice].push_back(p);
		}
	}
}

void Sphere::initSurfaceNormals()
{
	for (int i = 0; i < uvMap.size() - 1; i++)
	{
		surfaceNormals.push_back(vector<Vec3>());
		centroids.push_back(vector<Vec3>());
		for (int j = 0; j < uvMap[i].size() - 1; j++)
		{
			Vec3 k1 = uvMap[i][j];
			Vec3 k2 = uvMap[i][j + 1];
			Vec3 k3 = uvMap[i + 1][j + 1];
			Vec3 k4 = uvMap[i + 1][j];

			Vec3 n;
			Vec3 centroid;

			if (i > 0)
			{
				n = triangleSurfaceNormal(k4, k2, k1);
				centroid = getCentroid(k4, k2, k1);
				surfaceNormals[i].push_back(n);
				centroids[i].push_back(centroid);
			}

			if (i < uvMap.size() - 2)
			{
				n = triangleSurfaceNormal(k4, k3, k2);
				centroid = getCentroid(k4, k3, k2);
				surfaceNormals[i].push_back(n);
				centroids[i].push_back(centroid);
			}
		}
	}
}

void Sphere::initVertexNormals()
{
	int polewardLeft;
	int polewardCenter;
	int polewardRight;
	int centerwardLeft;
	int centerwardMiddle;
	int centerwardRight;

	for (int i = 0; i < uvMap.size(); i++)
	{
		vertexNormals.push_back(vector<Vec3>());

		if (i == 1)
		{
			polewardLeft = surfaceNormals[0].size() - 1;
			polewardRight = 0;
			centerwardLeft = surfaceNormals[1].size() - 2;
			centerwardMiddle = centerwardLeft + 1;
			centerwardRight = 0;
		}
		else if (i == uvMap.size() - 2)
		{
			polewardLeft = surfaceNormals[surfaceNormals.size() - 1].size() - 1;
			polewardRight = 0;
			centerwardLeft = surfaceNormals[surfaceNormals.size() - 2].size() - 1;
			centerwardMiddle = 0;
			centerwardRight = 1;
		}
		if (i < 2 || i > uvMap.size() - 3)
		{
			for (int j = 0; j < uvMap[i].size(); j++)
			{
				Vec3 sumVec;
				if (i == 0 || i == uvMap.size() - 1)
				{
					int normalIndex = i == 0 ? i : i - 1;
					for (int k = 0; k < surfaceNormals[normalIndex].size(); k++)
					{
						sumVec = sumVec + surfaceNormals[normalIndex][k];
					}
					sumVec.normalize();
					//sumVec.scale(0.01);
					sumVec = sumVec + uvMap[i][j];
					vertexNormals[i].push_back(sumVec);
					j = uvMap[i].size();
				}
				else if (i == 1)
				{
					sumVec = sumVec + surfaceNormals[0][polewardLeft]
						+ surfaceNormals[0][polewardRight];
					polewardLeft = (polewardLeft + 1) % surfaceNormals[0].size();
					polewardRight = (polewardRight + 1) % surfaceNormals[0].size();
					sumVec = sumVec + surfaceNormals[1][centerwardLeft]
						+ surfaceNormals[1][centerwardMiddle]
						+ surfaceNormals[1][centerwardRight];
					centerwardLeft = (centerwardLeft + 2) % surfaceNormals[1].size();
					centerwardMiddle = (centerwardMiddle + 2) % surfaceNormals[1].size();
					centerwardRight = (centerwardRight + 2) % surfaceNormals[1].size();
					sumVec.normalize();
					//sumVec.scale(0.01);
					sumVec = sumVec + uvMap[i][j];
					vertexNormals[i].push_back(sumVec);
				}
				else if (i == uvMap.size() - 2)
				{
					sumVec = sumVec + surfaceNormals[surfaceNormals.size() - 2][centerwardLeft]
						+ surfaceNormals[surfaceNormals.size() - 2][centerwardMiddle]
						+ surfaceNormals[surfaceNormals.size() - 2][centerwardRight];
					centerwardLeft = (centerwardLeft + 2) % surfaceNormals[surfaceNormals.size() - 2].size();
					centerwardMiddle = (centerwardMiddle + 2) % surfaceNormals[surfaceNormals.size() - 2].size();
					centerwardRight = (centerwardRight + 2) % surfaceNormals[surfaceNormals.size() - 2].size();
					sumVec = sumVec + surfaceNormals[surfaceNormals.size() - 1][polewardLeft]
						+ surfaceNormals[surfaceNormals.size() - 1][polewardRight];
					polewardLeft = (polewardLeft + 1) % surfaceNormals[surfaceNormals.size() - 1].size();
					polewardRight = (polewardRight + 1) % surfaceNormals[surfaceNormals.size() - 1].size();
					sumVec.normalize();
					//sumVec.scale(0.01);
					sumVec = sumVec + uvMap[i][j];
					vertexNormals[i].push_back(sumVec);
				}
			}
		}
	}

	if (uvMap.size() > 2)
	{
		for (int i = 2; i < uvMap.size() - 2; i++)
		{
			polewardLeft = surfaceNormals[i - 1].size() - 1;
			polewardCenter = 0;
			polewardRight = 1;
			centerwardLeft = surfaceNormals[i].size() - 2;
			centerwardMiddle = centerwardLeft + 1;
			centerwardRight = 0;
			for (int j = 0; j < uvMap[i].size(); j++)
			{
				Vec3 sumVec;
				for (int k = 0; k < 3; k++)
				{
					sumVec = sumVec + surfaceNormals[i - 1][(polewardLeft + k) % surfaceNormals[i - 1].size()];
				}
				polewardLeft = (polewardLeft + 2) % surfaceNormals[i - 1].size();

				for (int k = 0; k < 3; k++)
				{
					sumVec = sumVec + surfaceNormals[i][(centerwardLeft + k) % surfaceNormals[i].size()];
				}
				centerwardLeft = (centerwardLeft + 2) % surfaceNormals[i].size();

				sumVec.normalize();
				//sumVec.scale(0.01);
				sumVec = sumVec + uvMap[i][j];
				vertexNormals[i].push_back(sumVec);
			}
		}
	}
}

Vec3 Sphere::getCentroid(const Vec3& p1, const Vec3& p2, const Vec3& p3)
{
	Vec3 centroid;
	centroid.x = (p1.x + p2.x + p3.x) / 3;
	centroid.y = (p1.y + p2.y + p3.y) / 3;
	centroid.z = (p1.z + p2.z + p3.z) / 3;
	return centroid;
}

Vec3 Sphere::triangleSurfaceNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3)
{
	Vec3 v;
	v.x = p2.x - p1.x;
	v.y = p2.y - p1.y;
	v.z = p2.z - p1.z;

	Vec3 w;
	w.x = p3.x - p1.x;
	w.y = p3.y - p1.y;
	w.z = p3.z - p1.z;

	Vec3 centroid = getCentroid(p1, p2, p3);
	Vec3 n = Vec3::crossProduct(v, w);
	//normalize(n);
	n.x += centroid.x;
	n.y += centroid.y;
	n.z += centroid.z;

	return n;
}

void Sphere::drawWireframe()
{
	float outerRatio = (float)1 / (uvMap.size() - 1);
	for (int i = 0; i < uvMap.size() - 1; i++)
	{
		float innerRatio = (float)1 / (uvMap[i].size() - 1);
		for (int j = 0; j < uvMap[i].size() - 1; j++)
		{
			Vec3 k1 = uvMap[i][j];
			Vec3 k2 = uvMap[i][j + 1];
			Vec3 k3 = uvMap[i + 1][j + 1];
			Vec3 k4 = uvMap[i + 1][j];

			glColor3f(0.375, i * outerRatio, 1 / (j * innerRatio));

			glBegin(GL_LINE_LOOP);
			glVertex3f(k1.x, k1.y, k1.z);
			glVertex3f(k2.x, k2.y, k2.z);
			glVertex3f(k4.x, k4.y, k4.z);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex3f(k2.x, k2.y, k2.z);
			glVertex3f(k3.x, k3.y, k3.z);
			glVertex3f(k4.x, k4.y, k4.z);
			glEnd();
		}
	}
}

void Sphere::drawPolygon()
{
	float outerRatio = (float)1 / (uvMap.size() - 1);

	for (int i = 0; i < uvMap.size() - 1; i++)
	{
		float innerRatio = (float)1 / (uvMap[i].size() - 1);
		for (int j = 0; j < uvMap[i].size() - 1; j++)
		{
			Vec3 k1 = uvMap[i][j];
			Vec3 k2 = uvMap[i][j + 1];
			Vec3 k3 = uvMap[i + 1][j + 1];
			Vec3 k4 = uvMap[i + 1][j];

			glColor3f(0.375, i * outerRatio, 1 / (j * innerRatio));

			glBegin(GL_POLYGON);
			glVertex3f(k1.x, k1.y, k1.z);
			glVertex3f(k2.x, k2.y, k2.z);
			glVertex3f(k4.x, k4.y, k4.z);
			glEnd();

			glBegin(GL_POLYGON);
			glVertex3f(k2.x, k2.y, k2.z);
			glVertex3f(k3.x, k3.y, k3.z);
			glVertex3f(k4.x, k4.y, k4.z);
			glEnd();
		}
	}
}

void Sphere::drawSurfaceNormals()
{
	glColor3f(1, 0, 0);
	for (int i = 0; i < surfaceNormals.size(); i++)
	{
		for (int j = 0; j < surfaceNormals[i].size(); j++)
		{
			glBegin(GL_LINES);
			glVertex3f(centroids[i][j].x, centroids[i][j].y, centroids[i][j].z);
			glVertex3f(surfaceNormals[i][j].x, surfaceNormals[i][j].y, surfaceNormals[i][j].z);
			glEnd();
		}
	}
}

void Sphere::drawVertexNormals()
{
	glColor3f(1, 1, 1);
	for (int i = 0; i < vertexNormals.size(); i++)
	{
		for (int j = 0; j < vertexNormals[i].size(); j++)
		{
			glBegin(GL_LINES);
			glVertex3f(uvMap[i][j].x, uvMap[i][j].y, uvMap[i][j].z);
			glVertex3f(vertexNormals[i][j].x, vertexNormals[i][j].y, vertexNormals[i][j].z);
			glEnd();
		}
	}
}

void Sphere::operator=(const Sphere& other)
{
	center = other.center;
	slices = other.slices;
	rings = other.rings;
	radius = other.radius;

	uvMap.clear();
	surfaceNormals.clear();
	centroids.clear();
	vertexNormals.clear();

	for (int i = 0; i < other.uvMap.size(); i++)
	{
		uvMap.push_back(other.uvMap[i]);
	}

	for (int i = 0; i < other.surfaceNormals.size(); i++)
	{
		surfaceNormals.push_back(other.surfaceNormals[i]);
		centroids.push_back(other.centroids[i]);
	}

	for (int i = 0; i < other.vertexNormals.size(); i++)
	{
		vertexNormals.push_back(other.vertexNormals[i]);
	}
}