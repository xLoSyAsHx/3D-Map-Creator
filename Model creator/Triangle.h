#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <vector>

#include "Vect3f.h"

class Triangle
{
protected:
	std::vector<Vect3f*> *verts;
public:
	Triangle(std::vector<Vect3f*> *verts);

	virtual void show() = 0;

	~Triangle();
};

#endif TRIANGLE_H