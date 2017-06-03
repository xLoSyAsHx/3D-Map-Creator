#include "ParticleSystemPresenter.h"


std::map<std::string, std::map<int, int>> ParticleSystemPresenter::curObjsForShowAreas;


ParticleSystem * ParticleSystemPresenter::genSimilarParticleSys(ParticleSystem * partSys, 
	MoveBtnsController *moveBtnsController)
{
/*

for (auto it_koef = koefs.begin(); it_koef != koefs.end(); ++it_koef)
for (auto it_name = (*it_koef)->begin(); it_name != (*it_koef)->end(); ++it_name)
for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it) {
auto presenter = FromDOPresenter;
if (it_koef - koefs.begin() != 0)
presenter = ToDOPresenter;

int id = presenter->getObjId(it_name->first, it->first);
auto pos = *presenter->getObj(id)->getPosition();
auto rot = presenter->getObj(id)->getRotation();

it->second.areaPos = pos;
it->second.areaRot = rot;
}
*/
	if (partSys == nullptr)
		return nullptr;

	auto presenters = partSys->getPresenters();

	auto cam = moveBtnsController->getCamera();

	auto FromDOPresenter = presenters.first->genSimilarDrObjPresenter(cam);
	auto ToDOPresenter = presenters.second->genSimilarDrObjPresenter(cam);

	auto koefs = partSys->getKoefs();

	auto particleInfo = partSys->getParticleInfo();
	
	//
	Vect3f direct(0, 0, -1);

	direct = Matrix::rotationMatrix_X(direct, cam.getYAngle());
	direct = Matrix::rotationMatrix_Y(direct, cam.getXAngle());
	direct = direct.mul(5.0f);

	//

	for (auto it_koef = koefs.begin(); it_koef != koefs.end(); ++it_koef)
		for (auto it_name = (*it_koef)->begin(); it_name != (*it_koef)->end(); ++it_name)
			for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it) {
				auto presenter = FromDOPresenter;
				if (it_koef - koefs.begin() != 0)
					presenter = ToDOPresenter;

				int id = presenter->getObjId(it_name->first, it->first);
				auto pos = *presenter->getObj(id)->getPosition();
				auto rot = presenter->getObj(id)->getRotation();

				it->second.areaPos = pos.sum(direct);
				it->second.areaRot = rot;
			}
	//

	if (particleInfo->particle != nullptr)
		particleInfo->particle = DrawableObjectsPresenter2::genegateSimilarDrObj(
			particleInfo->particle);

	ParticleSystem *newPartSys = new ParticleSystem(
		FromDOPresenter, ToDOPresenter, koefs,
		particleInfo, moveBtnsController);

	return newPartSys;
}

ParticleSystemPresenter::ParticleSystemPresenter()
{
	objMapIndex = new std::map<std::string, std::map<int, int>>();
}

std::vector<int> ParticleSystemPresenter::getObjectIndexes(std::string name)
{
	std::vector<int> indexes;

	for (auto it = (*objMapIndex)[name].begin(); it != (*objMapIndex)[name].end(); ++it)
		indexes.push_back(it->first);
	//проверить правильность currentVertexes 
	return indexes;
}

int ParticleSystemPresenter::addParticleSystem(ParticleSystem * partSys, std::string name,
	MoveBtnsController *moveBtnsController)
{
	auto ptr = genSimilarParticleSys(partSys, moveBtnsController);

	auto cam = moveBtnsController->getCamera();

	auto presenters = ptr->getPresenters();
	auto presenter = presenters.first;
	for (int i = 0; i < 2; ++i) {
		auto mapIndexes = presenter->getObjMapIndex();
		for (auto it_name = mapIndexes->begin(); it_name != mapIndexes->end(); ++it_name) {
			auto map = &it_name->second;
			for (auto it = map->begin(); it != map->end(); ++it) {
				auto obj = presenter->getObj(it_name->first, it->first);

				Vect3f objPos = *obj->getPosition();
				//Vect3f camPos(Vect3f(-cam.getX(), -cam.getY(), -cam.getZ()));
				Vect3f direct(0, 0, -1);

				direct = Matrix::rotationMatrix_X(direct, cam.getYAngle());
				direct = Matrix::rotationMatrix_Y(direct, cam.getXAngle());
				direct = direct.mul(5.0f);

				//obj->setPosition(camPos.sum(objPos));
				obj->setPosition(objPos);
				obj->move(direct);
			}
		}
		presenter = presenters.second;
	}

	ptr->updateCube();
	ptr->update();

	objMap.push_back(ptr);

	int index = 0;
	if (objMapIndex->find(name) == objMapIndex->end()) {
		(*objMapIndex)[name] = {};
		(*objMapIndex)[name][index] = objMap.size() - 1;
	}
	else
	{


		auto indexes = getObjectIndexes(name);

		for (auto it = indexes.begin(); it != indexes.end(); ++it, index++)
			if (index != *it)
				break;
		(*objMapIndex)[name][index] = objMap.size() - 1;
	}

	return index;
}

ParticleSystem * ParticleSystemPresenter::getObj(std::string name, int index)
{
	if (objMapIndex->find(name) != objMapIndex->end()) {
		if ((*objMapIndex)[name].find(index) != (*objMapIndex)[name].end())
			return objMap[(*objMapIndex)[name][index]];
	}

	return nullptr;
}

std::map<std::string, std::map<int, int>>* ParticleSystemPresenter::getObjMapIndexes()
{
	return objMapIndex;
}

void ParticleSystemPresenter::show()
{
	for (auto it = objMap.begin(); it != objMap.end(); ++it)
		(*it)->play();
}

void ParticleSystemPresenter::show(ParticleSystem * partSys)
{
	partSys->play();
}

void ParticleSystemPresenter::showAreas(ParticleSystem *curPartSys)
{
	for (auto it = objMap.begin(); it != objMap.end(); ++it)
		(*it)->showAreas(curObjsForShowAreas, curObjsForShowAreas, true, curPartSys);
}

void ParticleSystemPresenter::showArea(ParticleSystem * curPartSys)
{
	curPartSys->showAreas(curObjsForShowAreas, curObjsForShowAreas, true, curPartSys);
}

std::string ParticleSystemPresenter::erase(ParticleSystem * curPartSys)
{
	for (auto it_name = objMapIndex->begin(); it_name != objMapIndex->end(); ++it_name) {
		auto pairMap = &it_name->second;

		for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {
			auto obj = getObj(it_name->first, it->first);

			if (obj == curPartSys) {
				auto name = it_name->first;
				int index = it->second;

				(*objMapIndex)[name].erase(it->first);

				if ((*objMapIndex)[name].size() == 0)
					objMapIndex->erase(name);

				objMap.erase(objMap.begin() + index);

				return name;
			}
		}
	}
}

int ParticleSystemPresenter::size()
{
	return objMap.size();
}
