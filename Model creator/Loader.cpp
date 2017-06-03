#include "Loader.h"

std::pair<std::string, DrawableObject2*> Loader::loadDrawableObject(std::ifstream & fin)
{

	std::string objName;
	Vect3f objPos, objRot;
	int numOfObjs;
	bool isTexture;
	unsigned int mode;
	//
	std::vector<Vect3f*> *vertPosition = new std::vector<Vect3f*>;
	std::vector<Vect3f*> *vertRotation = new std::vector<Vect3f*>;
	std::vector<OpenGL_Base*> *vertexes3f = new std::vector<OpenGL_Base*>;

	
	std::getline(fin, objName);
	fin >> objPos >> objRot;
	fin >> numOfObjs;

	for (int i = 0; i < numOfObjs; ++i) {
		Vect3f *bObjPos = new Vect3f, *bObjRot = new Vect3f;
		int numOfVectexes;
		std::vector<Vect3f*> *vertexes = new std::vector<Vect3f*>;
		GLuint *texture = nullptr;
		std::vector<Vect2f*> *textureCoord = nullptr;
		std::vector<Vect4f*> *colors = nullptr;

		fin >> *bObjPos >> *bObjRot;
		fin >> isTexture;
		fin >> mode;
		fin >> numOfVectexes;

		for (int j = 0; j < numOfVectexes; ++j) {
			Vect3f *vert = new Vect3f;
			fin >> *vert;
			vertexes->push_back(vert);
		}

		std::string pathToTexture;
		if (isTexture == false)
		{
			colors = new std::vector<Vect4f*>;
			for (int j = 0; j < numOfVectexes; ++j) {
				Vect4f *vert = new Vect4f;
				fin >> *vert;
				colors->push_back(vert);
			}
		}
		else
		{
			textureCoord = new std::vector<Vect2f*>;
			for (int j = 0; j < numOfVectexes; ++j) {
				Vect2f *vert = new Vect2f;
				fin >> *vert;
				textureCoord->push_back(vert);
			}

			GLuint *texture = new GLuint;

			fin.ignore(1, '\n');
			std::getline(fin, pathToTexture);
			LoadGLTextures(pathToTexture, texture);
		}

		OpenGL_Base *base = new OpenGL_Base;

		base->setMode(mode);
		base->setVertexes(vertexes);
		if (colors != nullptr)
			base->setColors(colors);
		if (textureCoord != nullptr)
			base->setTexture(texture, textureCoord, pathToTexture);

		vertexes3f->push_back(base);
		vertPosition->push_back(bObjPos);
		vertRotation->push_back(bObjRot);
	}

	DrawableObject2*  drawableObject = new DrawableObject2(vertexes3f, vertPosition, vertRotation, objPos, objRot);
	
	return std::pair<std::string, DrawableObject2*>(objName, drawableObject);
}

std::pair<std::string, ParticleSystem*> Loader::loadParticleSystem(std::ifstream & fin,
	std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> *colorMap,
	 MoveBtnsController *moveBtnsController)
{
	std::string particleSystemName;
	int presenterSize;

	std::getline(fin, particleSystemName);


	std::vector<DrawableObjectsPresenter2*> presenters = {
		new DrawableObjectsPresenter2(),
		new DrawableObjectsPresenter2()
	};
	std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> koefs = {
		new std::map<std::string, std::map<int, AreaForParticleSys>>,
		new std::map<std::string, std::map<int, AreaForParticleSys>>
	};


	auto it_presenter = presenters.begin();
	auto it_koef = koefs.begin();
	bool isAddToFrom = true;
	ParticleInfo *particleInfo = new ParticleInfo;

	for (int s = 0; s < 2; ++s)
	{
		fin >> presenterSize;

		for (int i = 0; i < presenterSize; ++i)
		{
			std::string objName;
			Vect3f pos, rot;
			int numOfOGLB;
			std::vector<Vect3f*> *vertPosition = new std::vector<Vect3f*>;
			std::vector<Vect3f*> *vertRotation = new std::vector<Vect3f*>;
			std::vector<OpenGL_Base*> *vertexes3f = new std::vector<OpenGL_Base*>;

			fin.ignore(1, '\n');
			std::getline(fin, objName);
			fin >> pos >> rot;
			fin >> numOfOGLB;

			for (int j = 0; j < numOfOGLB; ++j)
			{
				Vect3f *OGLBPos = new Vect3f;
				Vect3f *OGLBRot = new Vect3f;
				std::vector<Vect3f*> *vertexes = new std::vector<Vect3f*>;

				fin >> *OGLBPos >> *OGLBRot;

				for (int k = 0; k < 3; ++k) {
					Vect3f *vert = new Vect3f;
					fin >> *vert;
					vertexes->push_back(vert);
				}

				std::vector<Vect4f*> *colors = new std::vector<Vect4f*>;
				for (int k = 0; k < 3; ++k)
					colors->push_back(new Vect4f(255, 255, 255, 255));

				OpenGL_Base *OGLB = new OpenGL_Base;
				OGLB->setColors(colors);
				OGLB->setMode(GL_TRIANGLES);
				OGLB->setVertexes(vertexes);

				vertPosition->push_back(OGLBPos);
				vertRotation->push_back(OGLBRot);
				vertexes3f->push_back(OGLB);
			}

			DrawableObject2 *drObj = new DrawableObject2(vertexes3f, vertPosition, vertRotation, pos, rot);

			auto objMap = &(*it_presenter)->objMap;
			auto indexMap = (*it_presenter)->objMapIndex;

			objMap->push_back(drObj);

			int index = 0;
			if (indexMap->find(objName) == indexMap->end()) {
				(*indexMap)[objName] = {};
				(*indexMap)[objName][index] = int(objMap->size()) - 1;
			}
			else
			{
				auto indexes = (*it_presenter)->getObjectIndexes(objName);

				for (auto it = indexes.begin(); it != indexes.end(); ++it, index++)
					if (index != *it)
						break;
				(*indexMap)[objName][index] = int(objMap->size()) - 1;
			}

			if (colorMap != nullptr)
				(*colorMap)[(*it_presenter)][objMap->size() - 1] = DrawableObjectsPresenter2::getColorForSimpleDraw(*colorMap);

			std::string newObjName = objName + "#" + std::to_string(index);

			//
			if ((*it_koef)->find(objName) == (*it_koef)->end())
				(**it_koef)[objName] = {};



			AreaForParticleSys area;
			fin >> area.a >> area.b >> area.c >>
				area.areaPos >> area.areaRot >>
				area.chanceToAppear;

			area.type = objName;

			if ((**it_koef)[objName].find(index) == (**it_koef)[objName].end())
				(**it_koef)[objName][index] = area;
		}

		it_presenter++;
		it_koef++;
		isAddToFrom = false;
	}

	fin >> particleInfo->numOfParticles >>
		particleInfo->speedOfOccur >>
		particleInfo->particleFluct >>
		particleInfo->particleFluctLen >>
		particleInfo->particleRot >>
		particleInfo->particleSpeed;


	fin.ignore(1, '\n');

	auto pair = loadDrawableObject(fin);

	if (particleInfo->particle != nullptr)
		delete particleInfo->particle;

	particleInfo->particle = pair.second;

	ParticleSystem * partSys = new ParticleSystem(presenters.front(), presenters.back(), 
		koefs, particleInfo, moveBtnsController);


	return std::pair<std::string, ParticleSystem*>(particleSystemName, partSys);
}

std::string Loader::loadParticleSystem(std::string path,
	Logger *logger,
	DrawableObjectsPresenter2 * FromDOPresenter, 
	DrawableObjectsPresenter2 * ToDOPresenter, 
	std::map<std::string, std::map<int, AreaForParticleSys>>* fromKoefsForParticleSys, 
	std::map<std::string, std::map<int, AreaForParticleSys>>* toKoefsForParticleSys, 
	ParticleInfo & particleInfo,
	CameraLocation const &cam, 
	std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> &colorMap,
	HWND fromList, HWND toList, HWND fromListProb, HWND toListProb, 
	HWND editNumOfParts, std::string &particleName)
{
	std::ifstream fin(path);
	if (!fin.is_open()) {
		return "";
	}

	std::string particleSystemName;
	int presenterSize;

	std::getline(fin, particleSystemName);
	

	std::vector<DrawableObjectsPresenter2*> presenters = {
		FromDOPresenter,
		ToDOPresenter
	};
	std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> koefs = {
		fromKoefsForParticleSys,
		toKoefsForParticleSys
	};

	std::vector<HWND> list = { fromList, toList };
	std::vector<HWND> probList = { fromListProb, toListProb };
	
	auto it_presenter = presenters.begin();
	auto it_koef = koefs.begin();
	auto it_list = list.begin();
	auto it_probList = probList.begin();
	bool isAddToFrom = true;

	for (int s = 0; s < 2; ++s)
	{
		fin >> presenterSize;

		for (int i = 0; i < presenterSize; ++i)
		{
			std::string objName;
			Vect3f pos, rot;
			int numOfOGLB;
			std::vector<Vect3f*> *vertPosition = new std::vector<Vect3f*>;
			std::vector<Vect3f*> *vertRotation = new std::vector<Vect3f*>;
			std::vector<OpenGL_Base*> *vertexes3f = new std::vector<OpenGL_Base*>;

			fin.ignore(1, '\n');
			std::getline(fin, objName);
			fin >> pos >> rot;
			fin >> numOfOGLB;

			for (int j = 0; j < numOfOGLB; ++j)
			{
				Vect3f *OGLBPos = new Vect3f;
				Vect3f *OGLBRot = new Vect3f;
				std::vector<Vect3f*> *vertexes = new std::vector<Vect3f*>;

				fin >> *OGLBPos >> *OGLBRot;

				for (int k = 0; k < 3; ++k) {
					Vect3f *vert = new Vect3f;
					fin >> *vert;
					vertexes->push_back(vert);
				}

				std::vector<Vect4f*> *colors = new std::vector<Vect4f*>;
				for (int k = 0; k < 3; ++k)
					colors->push_back(new Vect4f(255, 255, 255, 255));

				OpenGL_Base *OGLB = new OpenGL_Base;
				OGLB->setColors(colors);
				OGLB->setMode(GL_TRIANGLES);
				OGLB->setVertexes(vertexes);

				vertPosition->push_back(OGLBPos);
				vertRotation->push_back(OGLBRot);
				vertexes3f->push_back(OGLB);
			}

			DrawableObject2 *drObj = new DrawableObject2(vertexes3f, vertPosition, vertRotation, pos, rot);

			auto objMap = &(*it_presenter)->objMap;
			auto indexMap = (*it_presenter)->objMapIndex;

			objMap->push_back(drObj);

			int index = 0;
			if (indexMap->find(objName) == indexMap->end()) {
				(*indexMap)[objName] = {};
				(*indexMap)[objName][index] = int(objMap->size()) - 1;
			}
			else
			{
				auto indexes = (*it_presenter)->getObjectIndexes(objName);

				for (auto it = indexes.begin(); it != indexes.end(); ++it, index++)
					if (index != *it)
						break;
				(*indexMap)[objName][index] = int(objMap->size()) - 1;
			}

			//
			if (logger != nullptr)
			{
				StructForCreateObj _struct;
				_struct.cam = cam;
				_struct.index = index;
				_struct.objName = (LPTSTR)objName.c_str();
				_struct.presenterName = isAddToFrom ? "from" : "to";

				logger->log(drObj, &_struct, Logger::Create_Obj);
			}
			//


			colorMap[(*it_presenter)][objMap->size() - 1] = DrawableObjectsPresenter2::getColorForSimpleDraw(colorMap);

			std::string newObjName = objName + "#" + std::to_string(index);

			if (*it_list != NULL)
				SendMessage(*it_list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
			

			//
			if ((*it_koef)->find(objName) == (*it_koef)->end())
				(**it_koef)[objName] = {};

			//Vect3f direct(0, 0, -1);

			//direct = Matrix::rotationMatrix_X(direct, cam.getYAngle());
			//direct = Matrix::rotationMatrix_Y(direct, cam.getXAngle());
			//direct = direct.mul(5.0f);

			//drObj->move(direct);

			AreaForParticleSys area;
			fin >> area.a >> area.b >> area.c >>
				area.areaPos >> area.areaRot >>
				area.chanceToAppear;

			if (*it_probList != NULL)
				SendMessage(*it_probList, LB_ADDSTRING, 0, (LPARAM)std::to_string(area.chanceToAppear).c_str());

			area.type = objName;

			if ((**it_koef)[objName].find(index) == (**it_koef)[objName].end())
				(**it_koef)[objName][index] = area;
		}

		it_presenter++;
		it_koef++;
		it_list++;
		it_probList++;
		isAddToFrom = false;
	}

	fin >> particleInfo.numOfParticles >>
		particleInfo.speedOfOccur >>
		particleInfo.particleFluct >>
		particleInfo.particleFluctLen >>
		particleInfo.particleRot >>
		particleInfo.particleSpeed;

	if (editNumOfParts != NULL)
		SendMessage(editNumOfParts, WM_SETTEXT, 0, (LPARAM)std::to_string(particleInfo.numOfParticles).c_str());


	fin.ignore(1, '\n');

	auto pair = loadDrawableObject(fin);

	if (particleInfo.particle != nullptr)
		delete particleInfo.particle;

	particleInfo.particle = pair.second;

	return particleSystemName;
}

void Loader::loadMapForMapRedactor(std::string path, DrawableObjectsPresenter2 * DOPresenter,
	ParticleSystemPresenter * PSPresenter, MoveBtnsController * moveBtnsControl, 
	HWND listParticleSystems)
{
	std::ifstream fin(path);
	if (!fin.is_open()) {
		return;
	}

	std::string mapName;
	int numOfObjects, numOfPartSys;

	std::getline(fin, mapName);

	fin >> numOfObjects >> numOfPartSys;

	auto cam = moveBtnsControl->getCamera();
	for (int i = 0; i < numOfObjects; ++i) {
		fin.ignore(1, '\n');
		auto pair = loadDrawableObject(fin);

		DOPresenter->addObj(pair.second, pair.first, cam);
		delete pair.second;
	}

	for (int i = 0; i < numOfPartSys; ++i) {
		auto pair = loadParticleSystem(fin, nullptr, moveBtnsControl);

		int index = PSPresenter->addParticleSystem(pair.second, pair.first, moveBtnsControl);

		auto newObjName = pair.first + "#" + std::to_string(index);
		SendMessage(listParticleSystems, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
	}

	fin.close();
}
