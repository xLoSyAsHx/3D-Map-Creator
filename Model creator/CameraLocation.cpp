#include "CameraLocation.h"


GLfloat CameraLocation::yDelta = 1.0f;


void CameraLocation::defRotVect()
{
	GLfloat a = camSpeed * sin(x_angle * aglToRad) * cos(y_angle * aglToRad);
	GLfloat b = camSpeed * sin(y_angle * aglToRad);
	GLfloat c = camSpeed * cos((x_angle)* aglToRad) * cos((y_angle)* aglToRad);

	WS_Vect.set(-a, b, c);

	AD_Vect.set(-c, 0, -a);

	UPDOWN_Vect.set(0, yDelta, 0);
}

void CameraLocation::updatePos(GLfloat ws, GLfloat updown, GLfloat ad)
{
	pos = pos +
		WS_Vect * ws +
		AD_Vect * ad +
		UPDOWN_Vect * updown;
}

void CameraLocation::updateMousePos()
{
	POINT pt;
	GetCursorPos(&pt);

	x_mousePos = pt.x;
	y_mousePos = pt.y;
}

void CameraLocation::defAngles()
{
	POINT pt;
	GetCursorPos(&pt);
	GLfloat dx = pt.x - x_mousePos;
	GLfloat dy = pt.y - y_mousePos;

	x_angle += dx / 3;
	y_angle += dy / 3;

	x_mousePos = pt.x;
	y_mousePos = pt.y;
}

GLfloat CameraLocation::getXAngle() const { return x_angle; }

GLfloat CameraLocation::getYAngle() const { return y_angle; }

GLfloat CameraLocation::getX() const { return pos.getX(); }

GLfloat CameraLocation::getY() const { return pos.getY(); }

GLfloat CameraLocation::getZ() const { return pos.getZ(); }
