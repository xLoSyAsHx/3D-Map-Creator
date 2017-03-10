#ifndef HELPER_H
#define HELPER_H

#include <GL\freeglut.h>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <stdio.h> 
#include <malloc.h>
#include <chrono>
#include <map>

#include <SOIL.h>

#include "Vect.h"
#include "CameraLocation.h"
#include "gloabalVars.h"


void print(float f);
void dbgprintf(char * format, ...);


int LoadGLTextures(char* filename, GLuint *texture)                                    // Load Bitmaps And Convert To Textures
{
	/* load an image file directly as a new OpenGL texture */
	*texture = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	if (texture[0] == 0)
		return 1;


	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return 0;                                        // Return Success
}

void createGrid() {
	glLoadIdentity();

	glBegin(GL_LINE);

	glEnd();
}

class {
	std::map<char, std::chrono::time_point<std::chrono::system_clock>> timers = { { 'W', std::chrono::system_clock::now() },
																		   		  { 'S', std::chrono::system_clock::now() },
																				  { 'A', std::chrono::system_clock::now() },
																				  { 'D', std::chrono::system_clock::now() } };

	std::map<char, bool> wasPressed = { { 'W', false },
										{ 'S', false },
										{ 'A', false },
										{ 'D', false },
										{ VK_LBUTTON, false } };

	CameraLocation cam;
public:
	// Set key state (pressed or no)
	void setKeyState(char key, bool status) { 
		wasPressed[key] = status;
	}


	bool isPresssed(char key) { return wasPressed[key]; }

	bool canUse(char key) {

		auto end = std::chrono::system_clock::now();

		int time_ms = std::chrono::duration_cast<std::chrono::milliseconds>	(end - timers[key]).count();

		if (isPresssed(key)) {
			if (time_ms < 70)
				return false;
			else 
			{
				startTimer(key);
				return true;
			}
		}

		if (time_ms < 250)
			return false;
		startTimer(key);
		wasPressed[key] = true;
		return true;
	}

	void startTimer(char key) {
		timers[key] = std::chrono::system_clock::now();
	}

	CameraLocation& getCamera() { return cam; }
} moveBtnsControl;



void control(bool	(keys[][256]), GLfloat &x, GLfloat &y, GLfloat &z, int &wheelDelta) {
	//Объект
	//---------------------------------------------------
	if ((*keys)[VK_CONTROL])
	{
		if ((*keys)['W'])
		{
			if (moveBtnsControl.canUse('W'))
				z -= 1;
		}
		else
			moveBtnsControl.setKeyState('W', false);

		if ((*keys)['S'])
		{
			if (moveBtnsControl.canUse('S'))
				z += 1;
		}
		else
			moveBtnsControl.setKeyState('S', false);

		if ((*keys)['A'])
		{
			if (moveBtnsControl.canUse('A'))
				x -= 1;
		}
		else
			moveBtnsControl.setKeyState('A', false);

		if ((*keys)['D'])
		{
			if (moveBtnsControl.canUse('D'))
				x += 1;
		}
		else
			moveBtnsControl.setKeyState('D', false);

		if (wheelDelta != 0)
		{
			y += wheelDelta / abs(wheelDelta);
			wheelDelta = 0;
		}
	}



	//Камера
	//---------------------------------------------------

	GLfloat temp_x = 0, temp_y = 0, temp_z = 0;
	if (!(*keys)[VK_CONTROL])
	{
		if ((*keys)['W'])
			temp_x = 1.0f;

		if ((*keys)['S'])
			temp_x = -1.0f;

		if ((*keys)['A'])
			temp_z = -1.0f;

		if ((*keys)['D'])
			temp_z = 1.0f;

		if (wheelDelta != 0)
		{
			temp_y += GLfloat(wheelDelta) / abs(wheelDelta);
			wheelDelta = 0;
		}
	}
	
	//Мышь
	//---------------------------------------------------
	CameraLocation& cam = moveBtnsControl.getCamera();


	if ((*keys)[VK_LBUTTON] && !(*keys)[VK_CONTROL])
	{

		if (!moveBtnsControl.isPresssed(VK_LBUTTON))
		{
			moveBtnsControl.setKeyState(VK_LBUTTON, true);
			cam.updateMousePos();
		}

		cam.defAngles();
		cam.defRotVect();
	}
	else
		moveBtnsControl.setKeyState(VK_LBUTTON, false);

	if (temp_x != 0.0f || temp_z != 0.0f || temp_z != 0.0f)
		cam.updatePos(temp_x, temp_y, temp_z);
}

void print(float f) {
	char str[10];
	sprintf_s(str, "%f\n", f);
	dbgprintf(str);
}

void dbgprintf(char * format, ...)
{
	va_list args;
	int len;
	char * buffer;

	va_start(args, format);
	len = _vscprintf(format, args) // _vscprintf doesn't count
		+ 4; // terminating '\0'
	buffer = (char*)malloc(len * sizeof(char));
	unsigned u = vsprintf_s(buffer, len, format, args);
	buffer[u] = 13;
	buffer[u + 1] = 10;
	buffer[u + 2] = 0;
	OutputDebugString(buffer);
	free(buffer);
}



#endif HELPER_H