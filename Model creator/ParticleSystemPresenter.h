#ifndef PARTICLE_SYSTEM_PRESENTER_H
#define PARTICLE_SYSTEM_PRESENTER_H

#include <iostream>
#include <vector>
#include <map>

#include "MoveBtnsController.h"

#include "ParticleSystem.h"

class ParticleSystemPresenter
{
	std::map<std::string, std::map<int, int>> *objMapIndex; //triangle 1, id = 4
	std::vector<ParticleSystem*> objMap;


	static ParticleSystem* genSimilarParticleSys(ParticleSystem* partSys, 
		MoveBtnsController *moveBtnsController);

	static std::map<std::string, std::map<int, int>> curObjsForShowAreas;
public:
	ParticleSystemPresenter();

	std::vector<int> getObjectIndexes(std::string name);

	int addParticleSystem(ParticleSystem* partSys, std::string name,
		MoveBtnsController *moveBtnsController);

	ParticleSystem* getObj(std::string name, int index);

	std::map<std::string, std::map<int, int>> *getObjMapIndexes();

	void show();

	void show(ParticleSystem *partSys);

	void showAreas(ParticleSystem *curPartSys);

	void showArea(ParticleSystem *curPartSys);

	std::string erase(ParticleSystem *curPartSys);

	int size();
};

#endif PARTICLE_SYSTEM_PRESENTER_H