#ifndef VECT_4F_H
#define VECT_4F_H


#include <GL\freeglut.h>
#include <iostream>

#include "Vect3f.h"

class Vect4f : public Vect3f
 {
 GLfloat alpha;
 public:

	 Vect4f();
	 Vect4f(Vect3f vect, GLfloat alpha = 0);
	 Vect4f(GLfloat x, GLfloat y, GLfloat z, GLfloat alpha = 0);

	 void set(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _alpha = 0);

	 void set(Vect4f vect);

	 void set(char key, GLfloat val);

	 void move(GLfloat _x, GLfloat _y, GLfloat _z);

	 void move(Vect3f vect);


	 GLfloat getAlpha() const;
	 void setAlpha(GLfloat _alpha);

	 Vect4f sum(Vect4f b);
	 Vect4f mul(Vect4f b);
	 Vect4f mul(float val);

	 friend std::istream& operator >> (std::istream & is, Vect4f &vect);

	 friend std::ostream& operator << (std::ostream & os, Vect4f &vect);
};

#endif VECT_4F_H