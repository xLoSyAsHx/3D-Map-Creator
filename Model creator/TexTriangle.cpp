#include "TexTriangle.h"

TexTriangle::TexTriangle(std::vector<Vect3f*>* verts,
	GLuint * texture, std::vector<Vect2f*>* texCoord) :
	Triangle(verts), texture(texture), texCoord(texCoord)
{
}

void TexTriangle::show()
{
	glColor4ub(Vect3f(255, 255, 255));

	auto it_texCoord = texCoord->begin();
	auto it_vert = verts->begin();

	glBindTexture(GL_TEXTURE_2D, *texture);

	glBegin(4);

	for (; it_vert != verts->end(); ++it_vert, ++it_texCoord) {
		glTexCoord2f(**it_texCoord);
		glVertex3f(**it_vert);
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

TexTriangle::~TexTriangle()
{
	for (auto it = texCoord->begin(); it != texCoord->end(); ++it)
		delete *it;

	delete texCoord;
}
