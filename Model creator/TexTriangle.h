#ifndef TEX_TRIANGLE_H
#define TEX_TRIANGLE_H

#include <GL\freeglut.h>

#include "helper.h"

#include "Triangle.h"

class TexTriangle :
	public Triangle
{
	GLuint *texture;
	std::vector<Vect2f*> *texCoord;
public:
	TexTriangle(std::vector<Vect3f*> *verts, GLuint *texture, std::vector<Vect2f*> *texCoord);

	void show();

	~TexTriangle();
};

#endif TEX_TRIANGLE_H