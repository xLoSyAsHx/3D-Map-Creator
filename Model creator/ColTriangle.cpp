#include "ColTriangle.h"

ColTriangle::ColTriangle(std::vector<Vect3f*>* verts, std::vector<Vect4f*>* colors) :
	Triangle(verts), colors(colors)
{
}

void ColTriangle::show()
{
	auto it_col = colors->begin();
	auto it_vert = verts->begin();

	glBegin(4);

	for (; it_vert != verts->end(); ++it_col, ++it_vert) {
		glColor4ub(**it_col);
		glVertex3f(**it_vert);
	}


	glEnd();
}

ColTriangle::~ColTriangle()
{
	for (auto it = colors->begin(); it != colors->end(); ++it)
		delete *it;

	delete colors;
}
