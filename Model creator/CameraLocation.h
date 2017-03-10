#ifndef CAMERA_LOCATION_H
#define CAMERA_LOCATION_H

#include <GL\freeglut.h>
#include <iostream>
#include <cmath>

#include "Vect.h"
#include "gloabalVars.h"

class CameraLocation {
	GLfloat x_angle, y_angle;
	Vect WS_Vect, AD_Vect, UPDOWN_Vect;
	Vect pos = { 0, 0, -5 };

	GLfloat x_mousePos, y_mousePos;

	static GLfloat yDelta;
public:

	void defRotVect();

	void updatePos(GLfloat ws, GLfloat updown, GLfloat ad);

	void updateMousePos();

	void defAngles();

	GLfloat getXAngle() const;

	GLfloat getYAngle() const;

	GLfloat getX() const;

	GLfloat getY() const;

	GLfloat getZ() const;

};

#endif CAMERA_LOCATION_H