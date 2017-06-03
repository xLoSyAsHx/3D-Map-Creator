#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H


#include <GL\freeglut.h>

#include <vector>
#include <map>

#include "ProgramDefines.h"

#include "DrawableObjectPresenter2.h"
#include "MoveBtnsController.h"
#include "SelectedCube.h"
#include "structs.h"

#include "ParticlePresenter.h"


class ParticleSystem
{
	SelectedCube cube;
	bool isNeedToUpdate = true;

	DrawableObjectsPresenter2 *FromDOPresenter;
	std::vector<AreaForParticleSys> areasForFrom;

	DrawableObjectsPresenter2 *ToDOPresenter;
	std::vector<AreaForParticleSys> areasForTo;

	ParticleInfo *particleInfo;
	std::map<int, SelectedCube> fromAreasBoards;
	std::map<int, SelectedCube> toAreasBoards;

	MoveBtnsController *moveBtnsController = nullptr;

	ParticlePresenter *particlePresenter = nullptr;
	//DrawableObjectsPresenter2 *PartsDOPresenter = nullptr;
	long long realNumOfParts = 0;

	
	Vect3f genStartPoint();
	Vect3f genEndPoint();
	void genParticle();

	std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> koefsForParticleSys;
	
public:
	ParticleSystem(DrawableObjectsPresenter2 *from,
		DrawableObjectsPresenter2 *to,
		std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> &koefsForParticleSys,
		ParticleInfo *particleInfo, MoveBtnsController *moveBtnsController);

	void showAreas(std::map<std::string, std::map<int, int>> &fromCurrentObjs,
		std::map<std::string, std::map<int, int>> &toCurrentObjs, bool isAreasForMapCreator = false,
		ParticleSystem* curPartSys = nullptr);

	void move(Vect3f vect);

	void move(GLfloat x, GLfloat y, GLfloat z);

	void play();

	void update();

	void updateCube();

	std::pair<DrawableObjectsPresenter2*, DrawableObjectsPresenter2*> getPresenters();

	std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> getKoefs();

	ParticleInfo* getParticleInfo();

	~ParticleSystem();
};

#endif PARTICLE_SYSTEM_H