#ifndef VECT_H
#define VECT_H


#include <GL\freeglut.h>
#include <iostream>

class Vect
 {
 GLfloat x, y, z;
 public:


	 

	 Vect();
	 Vect(GLfloat x, GLfloat y, GLfloat z);

	 void set(GLfloat _x, GLfloat _y, GLfloat _z);

	 GLfloat getX() const;
	 GLfloat getY() const;
	 GLfloat getZ() const;

	 Vect operator+(Vect b);
	 Vect operator*(Vect b);
	 Vect operator*(float val);
};

#endif VECT_H