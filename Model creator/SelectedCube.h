#ifndef SELECTED_CUBE_H
#define SELECTED_CUBE_H

#include <iostream>
#include <string>
#include <map>

#include "helper.h"

#include "Vect3f.h"


class SelectedCube
{
	std::map<std::string, Vect3f> cube;
public:
	SelectedCube(Vect3f &left, Vect3f &right, Vect3f &down, Vect3f &up, Vect3f &back, Vect3f &toward);
	SelectedCube();

	GLfloat getLeft();
	GLfloat getRight();
	GLfloat getDown();
	GLfloat getUp();
	GLfloat getFront();
	GLfloat getBack();

	void scal(Vect3f vect);
	void scal(GLfloat x, GLfloat y, GLfloat z);

	void draw();

	static SelectedCube makeCubeFrom2(SelectedCube &a, SelectedCube &b);
};

#endif SELECTED_CUBE_H