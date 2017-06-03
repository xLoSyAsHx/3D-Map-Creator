#ifndef COL_TRIANGLE_H
#define COL_TRIANGLE_H

#include <GL\freeglut.h>

#include "Vect4f.h"

#include "helper.h"

#include "Triangle.h"

class ColTriangle :
	public Triangle
{
	std::vector<Vect4f*> *colors;
public:
	ColTriangle(std::vector<Vect3f*> *verts, std::vector<Vect4f*> *colors);

	void show();

	~ColTriangle();
};
#endif COL_TRIANGLE_H