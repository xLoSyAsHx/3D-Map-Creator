#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "structs.h"
#include "DrawableObjectPresenter2.h"
#include "ParticleSystemPresenter.h"
#include "Logger.h"

class Loader
{
public:
	static std::pair<std::string, DrawableObject2*> loadDrawableObject(std::ifstream &fin);

	static std::pair<std::string, ParticleSystem*> loadParticleSystem(std::ifstream &fin, 
		std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> *colorMap,
		MoveBtnsController *moveBtnsControl);

	static std::string loadParticleSystem(std::string path,
		Logger *logger,
		DrawableObjectsPresenter2 * FromDOPresenter, DrawableObjectsPresenter2 * ToDOPresenter,
		std::map<std::string, std::map<int, AreaForParticleSys>> *fromKoefsForParticleSys,
		std::map<std::string, std::map<int, AreaForParticleSys>> *toKoefsForParticleSys,
		ParticleInfo & particleInfo, CameraLocation const &cam,
		std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> &colorMap,
		HWND fromList, HWND toList, HWND fromListProb, HWND toListProb,
		HWND editNumOfParts, std::string &particleName);

	static void loadMapForMapRedactor(std::string path, DrawableObjectsPresenter2* DOPresenter,
		ParticleSystemPresenter* PSPresenter, MoveBtnsController* moveBtnsControl,
		HWND listParticleSystems);
};

#endif LOADER_H