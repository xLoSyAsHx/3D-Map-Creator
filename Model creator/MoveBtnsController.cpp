#include "MoveBtnsController.h"



MoveBtnsController::MoveBtnsController()
{
}


MoveBtnsController::~MoveBtnsController()
{
}



void MoveBtnsController::setKeyState(char key, bool status)
{
	wasPressed[key] = status;
}

bool MoveBtnsController::isPresssed(char key)
{
	return wasPressed[key];
}

bool MoveBtnsController::canUse(char key)
{
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

	if (time_ms < 10) // 250
		return false;
	startTimer(key);
	wasPressed[key] = true;
	return true;
}

void MoveBtnsController::startTimer(char key)
{
	timers[key] = std::chrono::system_clock::now();
}

CameraLocation & MoveBtnsController::getCamera()
{
	return cam;
}
