#include "ParticleSystem.h"

Vect3f ParticleSystem::genStartPoint()
{
	for (auto it = fromAreasBoards.begin(); it != fromAreasBoards.end(); ) {
		auto area = areasForFrom[it->first];

		int chance = area.chanceToAppear * 10;
		if (rand() % 1000 > chance)
			continue;
		

		Vect3f p;
		while (true)
		{
			GLfloat x = rand() % int((it->second.getRight() - it->second.getLeft()) * 1000);
			x = it->second.getLeft() + x / 1000;
			GLfloat z = rand() % int((it->second.getFront() - it->second.getBack()) * 1000);
			z = it->second.getBack() + z / 1000;

			if (area.type == "Disk")
			{
				GLfloat x = rand() % int(2 * area.a * 1000);
				x /= 2000.001;

				GLfloat z = sqrt(1 - (x*x) / (area.a*area.a));

				p = Vect3f(x, 0, z);

				p = Matrix::rotationMatrix_X(p, area.areaRot.getX());
				p = Matrix::rotationMatrix_Y(p, area.areaRot.getY());
				p = Matrix::rotationMatrix_Z(p, area.areaRot.getZ());

				p = p.sum(area.areaPos);
				return p;
			}
		}

		auto tempIt = it;
		tempIt++;
		if (tempIt == fromAreasBoards.end())
			it = fromAreasBoards.begin();
		else
			it++;
	}
}

Vect3f ParticleSystem::genEndPoint()
{
	for (auto it = toAreasBoards.begin(); it != toAreasBoards.end(); ) {
		auto area = areasForTo[it->first];

		int chance = area.chanceToAppear * 10;
		if (rand() % 1000 > chance)
			continue;


		Vect3f p;
		while (true)
		{
			GLfloat x = rand() % int((it->second.getRight() - it->second.getLeft()) * 1000);
			x = it->second.getLeft() + x / 1000;
			GLfloat z = rand() % int((it->second.getFront() - it->second.getBack()) * 1000);
			z = it->second.getBack() + z / 1000;

			if (area.type == "Disk")
			{
				GLfloat x = rand() % int(2 * area.a * 1000);
				x /= 2000.001;

				GLfloat z = sqrt(1 - (x*x) / (area.a*area.a));

				p = Vect3f(x, 0, z);

				p = Matrix::rotationMatrix_X(p, area.areaRot.getX());
				p = Matrix::rotationMatrix_Y(p, area.areaRot.getY());
				p = Matrix::rotationMatrix_Z(p, area.areaRot.getZ());

				p = p.sum(area.areaPos);
				return p;
			}
		}

		auto tempIt = it;
		tempIt++;
		if (tempIt == toAreasBoards.end())
			it = toAreasBoards.begin();
		else
			it++;
	}
}

void ParticleSystem::genParticle()
{
	/*
	for (auto it = areasForFrom.begin(); it != areasForFrom.end(); ++it)
	{
		auto type = it->type;

		if (type == "Disk")
		{
			


			
			Vect3f right = Vect3f(it->a, 0, 0);

			right = Matrix::rotationMatrix_X(right, it->areaRot.getX());
			right = Matrix::rotationMatrix_Y(right, it->areaRot.getY());
			right = Matrix::rotationMatrix_Z(right, it->areaRot.getZ());

			right = right.sum(it->areaPos);
			//----------

			Vect3f left = Vect3f(-it->a, 0, 0);

			left = Matrix::rotationMatrix_X(left, it->areaRot.getX());
			left = Matrix::rotationMatrix_Y(left, it->areaRot.getY());
			left = Matrix::rotationMatrix_Z(left, it->areaRot.getZ());

			left = left.sum(it->areaPos);
			//----------

			Vect3f back = Vect3f(0, -it->c, 0);

			back = Matrix::rotationMatrix_X(back, it->areaRot.getX());
			back = Matrix::rotationMatrix_Y(back, it->areaRot.getY());
			back = Matrix::rotationMatrix_Z(back, it->areaRot.getZ());

			back = back.sum(it->areaPos);
			//----------

			Vect3f toward = Vect3f(0, it->c, 0);

			toward = Matrix::rotationMatrix_X(toward, it->areaRot.getX());
			toward = Matrix::rotationMatrix_Y(toward, it->areaRot.getY());
			toward = Matrix::rotationMatrix_Z(toward, it->areaRot.getZ());

			toward = toward.sum(it->areaPos);
			//----------

			Vect3f up = it->areaPos;
			//----------

			Vect3f down = it->areaPos;
			
		}
		
	}
	*/

	//циклится генерация точек
	Vect3f startPoint, endPoint;
	for (auto it = fromAreasBoards.begin(); it != fromAreasBoards.end(); ++it) {
		auto area = areasForFrom[it->first];

		int chance = area.chanceToAppear * 10;
		if (rand() % 1000 < chance) {
			startPoint = genStartPoint();
			endPoint = genEndPoint();


			auto obj = DrawableObjectsPresenter2::genegateSimilarDrObj(particleInfo->particle);
			obj->setPosition(startPoint);

			Particle * particle = new Particle(startPoint, endPoint, obj);

			particlePresenter->addParticle(particle);
			break;
		}
	}


	
}

void ParticleSystem::updateCube()
{

}

ParticleSystem::ParticleSystem(DrawableObjectsPresenter2 * from,
	DrawableObjectsPresenter2 * to,
	std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*>& koefsForParticleSys,
	ParticleInfo *particleInfo, MoveBtnsController * moveBtnsController) :
	FromDOPresenter(from), ToDOPresenter(to), 
	koefsForParticleSys(koefsForParticleSys), particleInfo(particleInfo),
	moveBtnsController(moveBtnsController)
{


	areasForFrom.resize(from->getobjMapSize());
	areasForTo.resize(to->getobjMapSize());

	for (auto it_koef = koefsForParticleSys.begin(); it_koef != koefsForParticleSys.end(); ++it_koef)
		for (auto it_name = (*it_koef)->begin(); it_name != (*it_koef)->end(); ++it_name)
			for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it) {
				if (it_koef - koefsForParticleSys.begin() == 0) {
					int id = FromDOPresenter->getObjId(it_name->first, it->first);
					areasForFrom[id] = it->second;
				}
				else {
					int id = ToDOPresenter->getObjId(it_name->first, it->first);
					areasForTo[id] = it->second;
				}
			}

	particlePresenter = new ParticlePresenter(
		particleInfo->particleFluct, particleInfo->particleFluctLen,
		particleInfo->particleRot, particleInfo->particleSpeed);

	if (particleInfo->speedOfOccur == 0)
		particleInfo->speedOfOccur = 500000;

	auto map = FromDOPresenter->getObjMapIndex();
	for (auto it_name = map->begin(); it_name != map->end(); ++it_name)
		for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it) {
			int id = it->second;
			auto obj = FromDOPresenter->getObj(id);

			fromAreasBoards[id] = (obj->detSelectedCube(moveBtnsController->getCamera()));
		}

	map = ToDOPresenter->getObjMapIndex();
	for (auto it_name = map->begin(); it_name != map->end(); ++it_name)
		for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it) {
			int id = it->second;
			auto obj = ToDOPresenter->getObj(id);

			toAreasBoards[id] = (obj->detSelectedCube(moveBtnsController->getCamera()));
		}
}

void ParticleSystem::showAreas(std::map<std::string, std::map<int, int>> &fromCurrentObjs,
	std::map<std::string, std::map<int, int>> &toCurrentObjs, bool isAreasForMapCreator, 
	ParticleSystem* curPartSys)
{
	std::pair<int, Vect3f*> vertForLink = { -1, nullptr };

	FromDOPresenter->show(moveBtnsController->getCamera(), fromCurrentObjs, false,
		std::map<int, std::map<Vect3f*, vector3ub>>(), vertForLink,
		std::map<int, std::set<OpenGL_Base*>>());

	ToDOPresenter->show(moveBtnsController->getCamera(), toCurrentObjs, false,
		std::map<int, std::map<Vect3f*, vector3ub>>(), vertForLink,
		std::map<int, std::set<OpenGL_Base*>>());

	std::vector<DrawableObjectsPresenter2*> presenters = {
		FromDOPresenter,
		ToDOPresenter
	};

	if (isAreasForMapCreator && isNeedToUpdate) {
		std::vector<std::map<Vect3f*, Vect3f>*> globalCoords;

		auto cam = moveBtnsController->getCamera();
		auto it_presenter = presenters.begin();
		for (int i = 0; i < 2; ++i)
		{
			auto objMapIndexes = (*it_presenter)->getObjMapIndex();

			for (auto it_name = objMapIndexes->begin(); it_name != objMapIndexes->end(); ++it_name) {
				auto pairMap = &it_name->second;
				for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {
					auto obj = (*it_presenter)->getObj(it->second);

					auto coords = obj->getAllGlobalCoord(cam);
					globalCoords.push_back(coords);
				}
			}

			it_presenter++;
		}

		Vect3f left, right, up, down, toward, back;
		bool check = false;
		for (auto it_map = globalCoords.begin(); it_map != globalCoords.end(); ++it_map)
			for (auto it = (*it_map)->begin(); it != (*it_map)->end(); ++it) {
				if (check == false) {
					check = true;
					left = it->second;
					right = it->second;
					up = it->second;
					down = it->second;
					toward = it->second;
					back = it->second;
				}

				auto checkFunc = [](Vect3f &point, Vect3f &low, Vect3f &high, char key) {
					if (point.get(key) < low.get(key))
						low = point;
					if (point.get(key) > high.get(key))
						high = point;
				};


				checkFunc(it->second, left, right, 'x');
				checkFunc(it->second, down, up, 'y');
				checkFunc(it->second, back, toward, 'z');
			}


		auto func = [&cam](Vect3f vect) -> Vect3f {
			return vect.minus(Vect3f(cam.getX(), cam.getY(), cam.getZ()));
		};


		//cube = SelectedCube(left, right, down, up, back, toward);
		cube = SelectedCube(func(left), func(right), func(down), func(up), func(back), func(toward));

		isNeedToUpdate = false;
	}

	if (isAreasForMapCreator && this == curPartSys)
	{
		cube.draw();
	}
}

void ParticleSystem::move(Vect3f vect)
{
	move(vect.getX(), vect.getY(), vect.getZ());
}

void ParticleSystem::move(GLfloat x, GLfloat y, GLfloat z)
{
	std::vector<DrawableObjectsPresenter2*> presenters = {
		FromDOPresenter,
		ToDOPresenter
	};

	auto it_presenter = presenters.begin();
	for (int i = 0; i < 2; ++i) {
		auto map = (*it_presenter)->getObjMapIndex();

		for (auto it_name = map->begin(); it_name != map->end(); ++it_name) {
			auto pairMap = &it_name->second;
			for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {
				auto obj = (*it_presenter)->getObj(it->second);

				obj->move(x, y, z);

				obj->updateCoords();
				obj->updateCube();
			}
		}
		it_presenter++;
	}

	updateCube();
}

void ParticleSystem::play()
{
	if (realNumOfParts < particleInfo->numOfParticles)
	{
		static auto time = std::chrono::system_clock::now();

		auto numOfMicrosec = chrono_cast_mics(time, chrono_now).count();

		if (numOfMicrosec > particleInfo->speedOfOccur * 100) {
			time = chrono_now;

			genParticle();
			realNumOfParts++;
		}
	}

	particlePresenter->moveParticles();

	static std::vector<int> vect;

	/*
	float x = 0;

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLES);

	for (float x = -1; x < 1; x += 0.2)
		for (float y = -1; y < 1; y += 0.2) {
			glVertex3f(x, y + 0, -2);
			glVertex3f(x, y + 0.15, -2);
			glVertex3f(0.2 + x, y + 0, -2);
		}
	glEnd();
	
	glPopMatrix();
	
	*/
	particlePresenter->draw(vect, moveBtnsController->getCamera());
	
	if (!vect.empty())
		for (auto it = vect.begin(); it != vect.end(); ++it) {
			auto startPoint = genStartPoint();
			auto endPoint = genEndPoint();

			particlePresenter->updateParticle(*it, startPoint, endPoint);
		}

	vect.clear();
	
}

void ParticleSystem::update()
{
	isNeedToUpdate = true;
}

std::pair<DrawableObjectsPresenter2*, DrawableObjectsPresenter2*> ParticleSystem::getPresenters()
{
	return std::pair<DrawableObjectsPresenter2*, DrawableObjectsPresenter2*>(
		FromDOPresenter,
		ToDOPresenter
		);
}

std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> ParticleSystem::getKoefs()
{
	return koefsForParticleSys;
}

ParticleInfo* ParticleSystem::getParticleInfo()
{
	return particleInfo;
}

ParticleSystem::~ParticleSystem()
{
	if (particlePresenter != nullptr)
		delete particlePresenter;

	//if (particleInfo != nullptr)
		//delete particleInfo;
}
