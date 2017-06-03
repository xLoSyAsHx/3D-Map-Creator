#ifndef ROUND_DRAWABLE_OBJECT_H
#define ROUND_DRAWABLE_OBJECT_H

#include <GL\freeglut.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "Vect4f.h"
#include "structs.h"
#include "OpenGL_Base.h"
#include "SelectedCube.h"
#include "CameraLocation.h"

class RoundDrawableObject : public Drawable
{
	Vect3f scale = Vect3f(1.0f, 1.0f, 1.0f);
	Vect3f *objPosition, objRotation;
	std::vector<Vect3f*> *vertPosition, *vertRotation;
	std::vector<OpenGL_Base*> *vertexes3f;

public:
	RoundDrawableObject(std::vector<OpenGL_Base*>* vertxMas, std::vector<Vect3f*>* pos, std::vector<Vect3f*>* rot,
		//Vect3f objPos = Vect3f(0, 0, 0) , Vect3f objRot = Vect3f(0, 0, 0));
		Vect3f objPos, Vect3f objRot);

	SelectedCube draw(CameraLocation const &cam, bool isVertexesEnable,
		std::map<Vect3f*, vector3ub> &colorMapForVertexes,
		std::pair<int, Vect3f*> &vertForLink,
		std::set<OpenGL_Base*> &OGLB_ForTex_set);

	void simpleDraw(vector3ub &colorVect);

	void simpleDrawVertexes(std::map<Vect3f*, vector3ub> &colorMap);

	void setPosition(Vect3f pos);

	void move(GLfloat x, GLfloat y, GLfloat z);

	void move(Vect3f vect);

	void rotate(GLfloat x, GLfloat y, GLfloat z);

	void rotate(Vect3f vect);

	bool isSelected(CameraLocation cam, Vect2f windPos, HWND hWnd);

	void setRotation(Vect3f rot);

	Vect3f* getPosition();

	Vect3f getRotation();

	Vect3f getScal();

	void setScal(Vect3f newScal);

	std::vector<Vect3f*>* getVertPosition();

	std::vector<Vect3f*>* getVertRotation();

	std::vector<OpenGL_Base*>* getVertexes();

	SelectedCube detSelectedCube(CameraLocation const & cam);

	std::map<Vect3f*, Vect3f>* getAllGlobalCoord(CameraLocation const & cam);

	~RoundDrawableObject();
};

#endif ROUND_DRAWABLE_OBJECT_H