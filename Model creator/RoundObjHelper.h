#ifndef ROUND_OBJECT_HELPER_H
#define ROUND_OBJECT_HELPER_H

#include <iostream>

#include "DrawableObject2.h"



std::vector<OpenGL_Base*>* genOGLBVectByPointVect(std::vector<Vect3f> &minPointsVect,
	GLfloat x1, GLfloat x2, GLfloat a, GLfloat b, GLfloat c, GLfloat r);


DrawableObject2* genDisc(Vect3f pos, GLfloat a, GLfloat c, int n);

DrawableObject2* genCone(Vect3f botPos, Vect3f topPos,GLfloat a, GLfloat c, int n);

DrawableObject2* genCylinder(Vect3f botPos,	Vect3f topPos, GLfloat a, GLfloat c, int n);

DrawableObject2 * genEllipsoid(Vect3f pos, GLfloat a, GLfloat b, GLfloat c, int n);

#endif ROUND_OBJECT_HELPER_H
