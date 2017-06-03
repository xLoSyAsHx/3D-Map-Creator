#include "Triangle.h"

Triangle::Triangle(std::vector<Vect3f*>* verts) :
	verts(verts)
{
}

Triangle::~Triangle()
{
	for (auto it = verts->begin(); it != verts->end(); ++it)
		delete *it;

	delete verts;
}
