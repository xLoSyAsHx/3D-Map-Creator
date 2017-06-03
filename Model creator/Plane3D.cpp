#include "Plane3D.h"




Plane3D::Plane3D(Vect3f a, Vect3f b, Vect3f c)
{
	Vect3f ab(b.getX() - a.getX(), b.getY() - a.getY(), b.getZ() - a.getZ());
	Vect3f ac(c.getX() - a.getX(), c.getY() - a.getY(), c.getZ() - a.getZ());

	Vect3f n = ab.scalMul(ac);

	A = n.getX();
	B = n.getY();
	C = n.getZ();
	D = -A * b.getX() - B * b.getY() - C * b.getZ();

	distToZero = getDistToPoint(0.0f, 0.0f, 0.0f);
}

GLfloat Plane3D::getA() const
{
	return A;
}

GLfloat Plane3D::getB() const
{
	return B;
}

GLfloat Plane3D::getC() const
{
	return C;
}

GLfloat Plane3D::getD() const
{
	return D;
}

GLfloat Plane3D::getDistToPoint(GLfloat x, GLfloat y, GLfloat z)
{
	return GLfloat(
		abs(
			(A*x + B*y + C*z + D) /
			sqrt(A*A + B*B + C*C)
		)
	);
}

bool Plane3D::isInside(Vect3f vect)
{
	return A * vect.getX() + B * vect.getY() + C * vect.getZ() + D == 0;
}

Plane3D::~Plane3D()
{
}
