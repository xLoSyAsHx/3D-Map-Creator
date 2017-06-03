#ifndef MOVE_BTNS_CONTROLLER_H
#define MOVE_BTNS_CONTROLLER_H

#include <GL\freeglut.h>
#include <iostream>
#include <map>
#include <chrono>

#include "CameraLocation.h"


class MoveBtnsController {
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

	MoveBtnsController();

	~MoveBtnsController();

	// Set key state (pressed or no)
	void setKeyState(char key, bool status);

	bool isPresssed(char key);

	bool canUse(char key);

	void startTimer(char key);

	CameraLocation& getCamera();
} ;



#endif MOVE_BTNS_CONTROLLER_H