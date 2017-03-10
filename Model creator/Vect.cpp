#include "Vect.h"


Vect::Vect()
{
}

Vect::Vect(GLfloat x, GLfloat y, GLfloat z) :
	x(x), y(y), z(z) 
{
}


void Vect::set(GLfloat _x, GLfloat _y, GLfloat _z)
{
	x = _x;
	y = _y;
	z = _z;
}


GLfloat Vect::getX() const { return x; }

GLfloat Vect::getY() const { return y; }

GLfloat Vect::getZ() const { return z; }


Vect Vect::operator+(Vect b)
{
	Vect temp = *this;

	temp.x += b.x;
	temp.y += b.y;
	temp.z += b.z;
	return temp;
}

Vect Vect::operator*(Vect b)
{
	Vect temp = *this;

	temp.x *= b.x;
	temp.y *= b.y;
	temp.z *= b.z;
	return temp;
}

Vect Vect::operator*(float val)
{
	Vect temp = *this;

	temp.x *= val;
	temp.y *= val;
	temp.z *= val;
	return temp;
}
