#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include <GL\freeglut.h>
#include <iostream>
#include <vector>

#include "Vect4f.h"
#include "structs.h"
#include "OpenGL_Base.h"
#include "helper.h"

class DrawableObject : public Drawable
{
	Vect3f objPosition, objRotation;
	std::vector<Vect3f*> *vertPosition, *vertRotation;
	std::vector<OpenGL_Base*> *vertexes3f;
	GLfloat alpha;
public:
	DrawableObject(std::vector<OpenGL_Base*>* vertxMas, std::vector<Vect3f*>* pos, std::vector<Vect3f*>* rot,
		//Vect3f objPos = Vect3f(0, 0, 0) , Vect3f objRot = Vect3f(0, 0, 0));
		Vect3f objPos, Vect3f objRot);

	void draw();

	void setPosition(Vect3f pos);

	void move(GLfloat x, GLfloat y, GLfloat z);

	void move(Vect3f vect);

	void setRotation(Vect3f rot);

	Vect3f getPosition() const;

	Vect3f getRotation() const;

	bool isTrans() const;

	~DrawableObject();

	//static DrawableObject* loadFromFile(std::string path);
};

#endif DRAWABLE_OBJECT_H