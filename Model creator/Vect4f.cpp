#include "Vect4f.h"


Vect4f::Vect4f()
{
}

Vect4f::Vect4f(Vect3f vect, GLfloat alpha):
	Vect3f(vect), alpha(alpha)
{
}

Vect4f::Vect4f(GLfloat x, GLfloat y, GLfloat z, GLfloat alpha) : 
	Vect3f(x, y, z), alpha(alpha)
{
}


void Vect4f::set(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _alpha)
{
	Vect3f::set(_x, _y, _z);
	alpha = _alpha;
}

void Vect4f::set(Vect4f vect)
{
	Vect3f::set(vect);
	alpha = vect.getAlpha();
}

void Vect4f::set(char key, GLfloat val)
{
	Vect3f::set(key, val);
}

void Vect4f::move(GLfloat _x, GLfloat _y, GLfloat _z)
{
	Vect3f::move(_x, _y, _z);
}

void Vect4f::move(Vect3f vect)
{
	Vect3f::move(vect);
}


GLfloat Vect4f::getAlpha() const
{ 
	return alpha; 
}

void Vect4f::setAlpha(GLfloat _alpha) { alpha = _alpha; }

Vect4f Vect4f::sum(Vect4f b)
{
	Vect4f temp = *this;
	temp.set(b.getX() + temp.getX(), b.getY() + temp.getY(), b.getZ() + temp.getZ(), alpha);
	return *this;
}

Vect4f Vect4f::mul(Vect4f b)
{
	Vect4f temp = *this;
	temp.set(b.getX() * temp.getX(), b.getY() * temp.getY(), b.getZ() * temp.getZ(), alpha);
	return *this;
}

Vect4f Vect4f::mul(float val)
{
	Vect4f temp = *this;
	temp.set(val * temp.getX(), val * temp.getY(), val * temp.getZ(), alpha);
	return *this;
}

std::istream & operator >> (std::istream & is, Vect4f & vect)
{
	GLfloat val1, val2, val3, val4;
	is >> val1 >> val2 >> val3 >> val4;

	vect.set(val1, val2, val3, val4);

	return is;
}

std::ostream & operator<<(std::ostream & os, Vect4f & vect)
{
	os.setf(std::ios::fixed);
	os << vect.getX() << " " << vect.getY() << " " << vect.getZ() << " " << vect.getAlpha();

	return os;
}
