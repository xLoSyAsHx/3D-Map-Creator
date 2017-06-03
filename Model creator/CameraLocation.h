#ifndef CAMERA_LOCATION_H
#define CAMERA_LOCATION_H

#include <GL\freeglut.h>
#include <iostream>
#include <cmath>

#include "Vect4f.h"
#include "gloabalVars.h"

class CameraLocation {
	GLfloat x_angle, y_angle;
	Vect3f WS_Vect, AD_Vect, UPDOWN_Vect;
	Vect3f pos;// = { 0, 0, 10 };

	GLfloat x_mousePos, y_mousePos;

	static GLfloat yDelta;
public:

	CameraLocation();

	void defRotVect();

	void updatePos(GLfloat ws, GLfloat updown, GLfloat ad, GLfloat speedKoef);

	void updateMousePos();

	void defAngles();

	GLfloat getXAngle() const;

	GLfloat getYAngle() const;

	GLfloat getX() const;

	GLfloat getY() const;

	GLfloat getZ() const;

	Vect2f getMousePos() const;

	Vect3f getDirect() const;

};

#endif CAMERA_LOCATION_H