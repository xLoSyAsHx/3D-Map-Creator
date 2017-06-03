#include "DrawableObject.h"



DrawableObject::DrawableObject(std::vector<OpenGL_Base*>* vertxMas, 
	std::vector<Vect3f*>* pos, std::vector<Vect3f*>* rot,
	Vect3f objPos, Vect3f objRot) :
	Drawable(), vertexes3f(vertexes3f), vertPosition(pos), vertRotation(rot),
	objPosition(objPos), objRotation(objRot)
{
}

void DrawableObject::draw()
{


	auto it_vertPos = vertPosition->begin();
	auto it_vertRot = vertRotation->begin();

	glPushMatrix();

	glTranslatef(objPosition);
	glRotatef(objRotation);

	for (auto vert = vertexes3f->begin(); vert != vertexes3f->end(); ++vert)
	{

		//glBegin((*vert)->getMode());
		glPushMatrix();

		glTranslatef(**it_vertPos);
		
		(*vert)->draw();

		glPopMatrix();
		//glEnd();
	}

	glPopMatrix();
}

void DrawableObject::setPosition(Vect3f pos)
{
	objPosition = pos;
}

void DrawableObject::move(GLfloat x, GLfloat y, GLfloat z)
{
	objPosition.move(x, y, z);

}

void DrawableObject::move(Vect3f vect)
{
	objPosition.move(vect);
}

void DrawableObject::setRotation(Vect3f rot)
{
	objRotation.set(rot);
}

Vect3f DrawableObject::getPosition() const
{
	return objPosition;
}

Vect3f DrawableObject::getRotation() const
{
	return objRotation;
}


bool DrawableObject::isTrans() const
{
	return alpha == 0.0f;
}

DrawableObject::~DrawableObject()
{
	delete vertexes3f;
	delete vertPosition;
	delete vertRotation;
}
