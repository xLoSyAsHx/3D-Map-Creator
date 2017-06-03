#ifndef DRAWABLE_OBJECT_2_H
#define DRAWABLE_OBJECT_2_H

#include <GL\freeglut.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "Vect4f.h"
#include "structs.h"
#include "OpenGL_Base.h"
#include "CameraLocation.h"
#include "Plane3D.h"
#include "helper.h"
#include "Matrix.h"
#include "SelectedCube.h"

class DrawableObjectsPresenter2;

class DrawableObject2 : public Drawable
{
	SelectedCube *cube;
	std::map<Vect3f*, Vect3f> *coords;
	bool isNeedUpdateCoords = true;
	bool isNeedUpdateCube = true;
	GLfloat vertSize = 0.15f;
	Vect3f scale = Vect3f(1.0f, 1.0f, 1.0f);
	Vect3f *objPosition, objRotation;
	std::vector<Vect3f*> *vertPosition, *vertRotation;
	std::vector<OpenGL_Base*> *vertexes3f;

public:
	DrawableObject2(std::vector<OpenGL_Base*>* vertxMas, std::vector<Vect3f*>* pos, std::vector<Vect3f*>* rot,
		//Vect3f objPos = Vect3f(0, 0, 0) , Vect3f objRot = Vect3f(0, 0, 0));
		Vect3f objPos, Vect3f objRot);

	SelectedCube draw(CameraLocation const &cam, bool isVertexesEnable, 
		std::map<Vect3f*, vector3ub> &colorMapForVertexes,
		std::pair<int, Vect3f*> &vertForLink,
		std::set<OpenGL_Base*> &OGLB_ForTex_set);

	void updateCoords();

	void updateCube();

	void simpleDraw(vector3ub &colorVect);

	void simpleDrawVertexes(std::map<Vect3f*, vector3ub> &colorMap);

	void setPosition(Vect3f pos);

	void setVertSize(GLfloat newVertSize);

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

	~DrawableObject2();


	friend DrawableObjectsPresenter2;
	//static DrawableObject* loadFromFile(std::string path);
};

#endif DRAWABLE_OBJECT_2_H