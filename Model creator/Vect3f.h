#ifndef VECT_3F_H
#define VECT_3F_H


#include <GL\freeglut.h>
#include <iostream>

#include "Vect2f.h"

class Matrix;

class Vect3f : public Vect2f
{
	GLfloat z;

	GLfloat operator[](int i);
public:

	Vect3f();
	Vect3f(GLfloat x, GLfloat y, GLfloat z);
	//Vect3f(Vect3f& vect);

	void set(GLfloat _x, GLfloat _y, GLfloat _z);

	void set(Vect3f vect);

	void set(char key, GLfloat val);

	void move(GLfloat _x, GLfloat _y, GLfloat _z);

	void move(Vect3f vect);

	GLfloat getZ() const;

	GLfloat get(char key) const;

	void setZ(GLfloat _z);

	Vect3f sum(Vect3f b);
	Vect3f minus(Vect3f& b);
	Vect3f mul(Vect3f b);
	Vect3f mul(float val);
	Vect3f scalMul(Vect3f b);

	bool equal(Vect3f& b);

	void absV();
	Vect3f invert();

	friend std::istream& operator>>(std::istream & is, Vect3f &vect);

	friend std::ostream& operator<<(std::ostream & os, Vect3f &vect);

	friend Matrix;
};

#endif VECT_3F_H