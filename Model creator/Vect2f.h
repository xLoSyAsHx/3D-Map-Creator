#ifndef VECT_2F_H
#define VECT_2F_H

#include <GL\freeglut.h>
#include <iostream>
#include <cmath>

class Vect3f;

class Vect2f
{
	GLfloat x, y;
	virtual GLfloat operator[](int i);

public:
	static Vect2f null;

	Vect2f();
	Vect2f(GLfloat x, GLfloat y);

	void set(GLfloat _x, GLfloat _y);

	void set(Vect2f vect);

	void move(GLfloat _x, GLfloat _y);

	void move(Vect2f vect);

	GLfloat getX() const;
	GLfloat getY() const;

	void setX(GLfloat _x);
	void setY(GLfloat _y);


	Vect2f sum(Vect2f& b);
	Vect2f minus(Vect2f& b);
	Vect2f mul(Vect2f& b);
	Vect2f mul(float val);

	bool equal(Vect2f& b);

	void absV();
	Vect2f invert();

	friend std::istream& operator >> (std::istream & is, Vect2f &vect);

	friend std::ostream& operator << (std::ostream & os, Vect2f &vect);

	friend Vect3f;
};

#endif VECT_2F_H