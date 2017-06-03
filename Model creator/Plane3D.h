#ifndef PLANE_3D_H
#define PLANE_3D_H

#include <GL\freeglut.h>

#include <cmath>

#include "Vect3f.h"

class Plane3D
{
	GLfloat A, B, C, D;
	GLfloat distToZero;
public:
	Plane3D(Vect3f a, Vect3f b, Vect3f c);

	GLfloat getA() const;
	GLfloat getB() const;
	GLfloat getC() const;
	GLfloat getD() const;

	GLfloat getDistToPoint(GLfloat x, GLfloat y, GLfloat z);

	bool isInside(Vect3f vect);

	~Plane3D();
};

#endif // !PLANE_3D_H