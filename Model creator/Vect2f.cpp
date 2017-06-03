#include "Vect2f.h"


GLfloat Vect2f::operator[](int i)
{
	return i == 0 ? x : y;
}

Vect2f::Vect2f()
{
}

Vect2f::Vect2f(GLfloat x, GLfloat y) :
	x(x), y(y)
{
}


void Vect2f::set(GLfloat _x, GLfloat _y)
{
	x = _x;
	y = _y;
}

void Vect2f::set(Vect2f vect)
{
	x = vect.getX();
	y = vect.getY();
}

void Vect2f::move(GLfloat _x, GLfloat _y)
{
	x += _x;
	y += _y;
}


void Vect2f::move(Vect2f vect)
{
	x += vect.getX();
	y += vect.getY();
}

GLfloat Vect2f::getX() const 
{ 
	return x; 
}

GLfloat Vect2f::getY() const 
{
	return y; 
}

void Vect2f::setX(GLfloat _x)
{
	x = _x;
}

void Vect2f::setY(GLfloat _y)
{
	y = _y;
}


Vect2f Vect2f::sum(Vect2f& b)
{
	Vect2f temp = *this;
	temp.x += b.x;
	temp.y += b.y;
	return temp;
}

Vect2f Vect2f::minus(Vect2f & b)
{
	Vect2f temp = *this;
	temp.x -= b.x;
	temp.y -= b.y;
	return temp;
}

Vect2f Vect2f::mul(Vect2f& b)
{
	Vect2f temp = *this;
	temp.x *= b.x;
	temp.y *= b.y;
	return temp;
}

Vect2f Vect2f::mul(float val)
{
	Vect2f temp = *this;
	temp.x *= val;
	temp.y *= val;
	return temp;
}

bool Vect2f::equal(Vect2f & b)
{
	return x == b.getX() && y == b.getY();
}

void Vect2f::absV()
{
	x = abs(x);
	y = abs(y);
}

Vect2f Vect2f::invert()
{
	x = -x;
	y = -y;
	return *this;
}

std::istream & operator >> (std::istream & is, Vect2f & vect)
{
	GLfloat val1, val2;

	is >> val1 >> val2;
	vect.set(val1, val2);

	return is;
}

std::ostream & operator<<(std::ostream & os, Vect2f & vect)
{
	os << vect.getX() << " " << vect.getY();

	return os;
}
