#ifndef SAVER_H
#define SAVER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "structs.h"
#include "DrawableObjectPresenter2.h"
#include "ParticleSystemPresenter.h"
#include "MoveBtnsController.h"

class Saver
{
	static void saveMapForMapRedactor(std::string path, DrawableObjectsPresenter2* DOPresenter,
		ParticleSystemPresenter* PSPresenter, MoveBtnsController* moveBtnsContro);

	static std::string getDrObjString(std::string name, DrawableObject2 *map);

	static std::string getPartSysString(std::string name, ParticleSystem *partSys);
public:

	static void saveParticleSystem(std::string path,
		DrawableObjectsPresenter2 * FromDOPresenter, DrawableObjectsPresenter2 * ToDOPresenter,
		std::map<std::string, std::map<int, AreaForParticleSys>> *fromKoefsForParticleSys,
		std::map<std::string, std::map<int, AreaForParticleSys>> *toKoefsForParticleSys,
		ParticleInfo & particleInfo, std::string &particleName);

	static void saveMap(std::string path, DrawableObjectsPresenter2* DOPresenter, 
		ParticleSystemPresenter* PSPresenter, MoveBtnsController* moveBtnsControl);

	//static void saveDrawableObject();
};

#endif SAVER_H