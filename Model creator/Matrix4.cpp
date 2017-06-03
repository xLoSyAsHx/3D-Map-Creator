#include "Matrix4.h"

Matrix4::Matrix4()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			mat[i][j] = 0;
}

Matrix4::Matrix4(Vect4f a, Vect4f b, Vect4f c, Vect4f d)
{
	Vect4f mas[] = { a, b, c, d };

	for (int i = 0; i < 3; ++i) {
		mat[i][0] = mas[i].getX();
		mat[i][1] = mas[i].getY();
		mat[i][2] = mas[i].getZ();
		mat[i][3] = mas[i].getAlpha();
	}

	det = nullptr;
}

void Matrix4::calculateDet()
{
	Vect4f l2 = getLine(1);
	Vect4f l3 = getLine(2);
	Vect4f l4 = getLine(3);

	Vect3f lines[] = { getLine(1), getLine(2), getLine(3)};

	char keys[] = { 'x', 'y', 'z', 'a'};
	
	Matrix m1;
	std::vector<Matrix> minors;
	for (int i = 0; i < 4; ++i) {
		Matrix m;

		for (int k = 0; k < 3; ++k) {
			std::vector<GLfloat> vals;
			for (int j = 0; j < 4; ++j)
				if (i != j)
					vals.push_back(lines[k].get(keys[j]));

			m.setLine(Vect3f(vals[0], vals[1], vals[2]), k);
		}
		
		minors.push_back(m);
	}

	if (det != nullptr)
		delete det;
	det = new double(0.0);
	for (int i = 0; i < 4; ++i) {
		double koef = (i + 1 % 2 == 0) ? 1 : -1;
		(*det) += koef * mat[1][i] * minors[i].getDet();
	}
}

void Matrix4::setColumn(Vect4f vect, int i)
{
	mat[0][i] = vect.getX();
	mat[1][i] = vect.getY();
	mat[2][i] = vect.getZ();
	mat[3][i] = vect.getAlpha();
}

GLdouble Matrix4::getDet() const
{
	return *det;
}

Vect4f Matrix4::getLine(int i)
{
	return Vect4f(mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
}

Vect4f Matrix4::multMatOnVect(Matrix4 mat, Vect4f vect)
{
	GLdouble coord[4];

	for (int i = 0; i < 4; ++i) {
		Vect4f line = mat.getLine(i);
		coord[i] = line.getX() * vect.getX() +
			line.getY() * vect.getY() +
			line.getZ() * vect.getZ() +
			line.getAlpha() * vect.getAlpha();
	}

	return Vect4f(coord[0], coord[1], coord[2], coord[3]);
}
