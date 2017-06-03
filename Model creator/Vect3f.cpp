#include "Vect3f.h"


GLfloat Vect3f::operator[](int i)
{
	if (i > 2 || i < 0)
		throw std::out_of_range("Индекс вектора не может быть больше 2 или меньше 0");
	if (i == 2)
		return z;
	return Vect2f(*this)[i];
}

Vect3f::Vect3f()
{
}

Vect3f::Vect3f(GLfloat x, GLfloat y, GLfloat z) :
	Vect2f(x,y), z(z)
{
}
/*
Vect3f::Vect3f(Vect3f& vect) :
	Vect2f(vect.getX(), vect.getY())
{
	z = vect.getZ();
}
*/

void Vect3f::set(GLfloat _x, GLfloat _y, GLfloat _z)
{
	Vect2f::set(_x, _y);
	z = _z;
}

void Vect3f::set(Vect3f vect)
{
	Vect2f::set(vect);
	z = vect.getZ();
}

void Vect3f::set(char key, GLfloat val)
{
	switch (key)
	{
	case 'x': x = val; break;
	case 'y': y = val; break;
	case 'z': z = val; break;
	}
}

void Vect3f::move(GLfloat _x, GLfloat _y, GLfloat _z)
{
	Vect2f::move(_x, _y);
	z += _z; 
}

void Vect3f::move(Vect3f vect)
{
	Vect2f::move(vect);
	z += vect.getZ();
}


GLfloat Vect3f::getZ() const 
{
	return z;
}

GLfloat Vect3f::get(char key) const
{
	switch (key)
	{
	case 'x':
		return x;
	case 'y':
		return y;
	case 'z':
		return z;
	default:
		return 0;
	}
}

void Vect3f::setZ(GLfloat _z)
{
	z = _z;
}



Vect3f Vect3f::sum(Vect3f b)
{
	Vect3f temp = *this;
	
	temp.set(b.getX() + temp.getX(), b.getY() + temp.getY(), b.getZ() + z);

	//temp.z += b.getZ();
	return temp;
}

Vect3f Vect3f::minus(Vect3f & b)
{
	Vect3f temp = *this;
	
	temp.set(temp.getX() - b.getX(), temp.getY() - b.getY(), z - b.getZ());

	//temp.z += b.getZ();
	return temp;
}

Vect3f Vect3f::mul(Vect3f b)
{
	Vect3f temp = *this;
	temp.set(b.getX() * temp.getX(), b.getY() * temp.getY(), b.getZ() * z);
	return temp;
}

Vect3f Vect3f::mul(float val)
{
	Vect3f temp = *this;
	temp.set(val * temp.getX(), val * temp.getY(),val * z);
	return temp;
}

Vect3f Vect3f::scalMul(Vect3f b)
{
	return Vect3f(
		getY() * b.getZ() - getZ() * b.getY(), 
		getZ() * b.getX() - getX() * b.getZ(), 
		getX() * b.getY() - getY() * b.getX()
	);
}

bool Vect3f::equal(Vect3f & b)
{
	return this->Vect2f::equal(b) && z == b.getZ();
}

void Vect3f::absV()
{
	z = abs(z);
	Vect2f::absV();
}

Vect3f Vect3f::invert()
{
	z = -z;
	Vect2f::invert();
	return *this;
}

std::istream& operator >> (std::istream & os, Vect3f &vect)
{
	GLfloat val1, val2, val3;
	os >> val1 >> val2 >> val3;
	
	vect.set(val1, val2, val3);

	return os;
}

std::ostream & operator<<(std::ostream & os, Vect3f & vect)
{
	os.setf(std::ios::fixed);
	os << vect.getX() << " " << vect.getY() << " " << vect.getZ();

	return os;
}
