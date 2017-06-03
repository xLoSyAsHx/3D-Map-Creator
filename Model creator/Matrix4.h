#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vect4f.h"
#include "Plane3D.h"
#include "gloabalVars.h"
#include <fstream>
#include <vector>

#include "Matrix.h"

class Matrix4
{
	GLdouble mat[4][4];
	//GLfloat invertMat[3][3];
	GLdouble *det;

	void setVal(int i, int j, GLdouble val);
	GLdouble getMinor(int i, int j);
public:
	//friend Vect3f;

	Matrix4();

	Matrix4(Vect4f a, Vect4f b, Vect4f c, Vect4f d);

	void calculateDet();

	void rotate(float a, float b, float g);

	void rotate(Vect3f vect);

	void setColumn(Vect4f vect, int i);

	GLdouble getDet() const;

	Vect4f getLine(int i);

	Vect3f getColumn(int i) const;

	GLdouble getVal(int i, int j) const;

	Matrix getInvetMatrix();

	static Vect4f multMatOnVect(Matrix4 mat, Vect4f vect);

	static Vect3f rotationMatrix_X(Vect3f vect, GLdouble angle);

	static Vect3f rotationMatrix_Y(Vect3f vect, GLdouble angle);

	static Vect3f rotationMatrix_Z(Vect3f vect, GLdouble angle);




	Matrix operator*(const Matrix&  mat2);
	Matrix operator*(const GLdouble koef);

	~Matrix4();
};

#endif MATRIX_H