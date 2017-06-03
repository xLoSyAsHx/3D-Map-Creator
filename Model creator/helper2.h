#pragma once

#include <GL\freeglut.h>

#include <iostream>
#include <vector>
#include <chrono>


#include "CameraLocation.h"
#include "DrawableObjectPresenter2.h"
#include "ParticleSystemPresenter.h"
#include "MoveBtnsController.h"
#include "Plane3D.h"
#include "ProgramDefines.h"

#include "Logger.h"

void objectsControl(MoveBtnsController& moveBtnsControl, DrawableObjectsPresenter2 *DOPresenter,
	std::map<std::string, std::map<int, int>> & currentObjs, bool(keys[][256]),
	int &wheelDelta, std::map<std::string, HWND> &elementsMap, Logger &logger,
	int cam_speed_el);

void particleSysControl(MoveBtnsController& moveBtnsControl, ParticleSystemPresenter *PSPresenter,
	ParticleSystem *currentPartSys, bool(keys[][256]),
	int &wheelDelta, std::map<std::string, HWND> &elementsMap, Logger &logger,
	int cam_speed_el);

void cameraControl(MoveBtnsController& moveBtnsControl, bool(keys[][256]),
	int &wheelDelta, std::map<std::string, HWND> &elementsMap, int cam_speed_el);
