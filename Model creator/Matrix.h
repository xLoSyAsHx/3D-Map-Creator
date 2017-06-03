#ifndef MATRIX_H
#define MATRIX_H

#include "Vect3f.h"
#include "Plane3D.h"
#include "gloabalVars.h"
#include <fstream>
#include <vector>

class Matrix
{
	GLdouble mat[3][3];
	//GLfloat invertMat[3][3];
	GLdouble *det;

	void setVal(int i, int j, GLdouble val);
	GLdouble getMinor(int i, int j);
public:
	//friend Vect3f;
	Matrix();

	Matrix(Vect3f a, Vect3f b, Vect3f c);

	Matrix(Plane3D a, Plane3D b, Plane3D c);

	void calculateDet();

	void rotate(float a, float b, float g);

	void rotate(Vect3f vect);

	void setColumn(Vect3f vect, int i);

	GLdouble getDet() const;
	
	Vect3f getLine(int i);

	void setLine(Vect3f vect, int i);

	Vect3f getColumn(int i) const;

	GLdouble getVal(int i, int j) const;

	Matrix getInvetMatrix();

	static Vect3f multMatOnVect(Matrix mat, Vect3f vect);

	static Vect3f rotationMatrix_X(Vect3f vect, GLdouble angle);

	static Vect3f rotationMatrix_Y(Vect3f vect, GLdouble angle);

	static Vect3f rotationMatrix_Z(Vect3f vect, GLdouble angle);




	Matrix operator*(const Matrix&  mat2);
	Matrix operator*(const GLdouble koef);

	~Matrix();
};

#endif MATRIX_H