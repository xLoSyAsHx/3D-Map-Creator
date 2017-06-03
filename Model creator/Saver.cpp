#include "Saver.h"



void Saver::saveParticleSystem(std::string path,
	DrawableObjectsPresenter2 * FromDOPresenter, DrawableObjectsPresenter2 * ToDOPresenter, 
	std::map<std::string, std::map<int, AreaForParticleSys>> *fromKoefsForParticleSys, 
	std::map<std::string, std::map<int, AreaForParticleSys>> *toKoefsForParticleSys,
	ParticleInfo & particleInfo, std::string &particleName)
{
	std::ofstream fout(path, std::ios_base::trunc);
	if (!fout.is_open()) {
		//char* err = "\nErr: DrawableObject::loadFromFile\tcan't open file.";

		//dbgprintf(err);
		//fin.close();
		return;
	}

	std::string name;
	int startPos, endPos;
	for (int i = path.length(); i >= 0; --i) {
		if (path[i] == '//' || path[i] == '\\') {
			startPos = i + 1;
			name = path.substr(startPos, endPos - startPos);
			//name.copy(path, endPos - startPos, startPos)
			break;
		}
		if (path[i] == '.')
			endPos = i;
	}


	//From and To Presenters
	fout << name << "\n";

	std::vector<DrawableObjectsPresenter2*> presenters = { 
		FromDOPresenter,
		ToDOPresenter
	};
	std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> koefs = {
		fromKoefsForParticleSys,
		toKoefsForParticleSys
	};


	auto it_presetner = presenters.begin();
	auto it_koef = koefs.begin();
	for (int j = 0; j < 2; ++j)
	{
		int numOfDrObjs = (*it_presetner)->getNumOfObjs();
		fout << numOfDrObjs << "\n";

		auto objIndexes = (*it_presetner)->getObjMapIndex();
		for (auto it_name = objIndexes->begin(); it_name != objIndexes->end(); ++it_name) {
			auto pairMap = &it_name->second;

			for (auto it_pair = pairMap->begin(); it_pair != pairMap->end(); ++it_pair) {
				auto obj = (*it_presetner)->getObj(it_name->first, it_pair->second);
				auto OGLB_verts = obj->getVertexes();

				fout << it_name->first << "\n";
				fout << *obj->getPosition() << " " << obj->getRotation() << "\n";
				fout << OGLB_verts->size() << "\n";

				auto vertPosition = obj->getVertPosition()->begin();
				auto vertRotation = obj->getVertRotation()->begin();
				auto vertexes3f = OGLB_verts->begin();

				for (int i = 0; i < OGLB_verts->size(); ++i) {
					fout << **vertPosition << " " << **vertRotation << "\n";

					auto verts = (*vertexes3f)->getVertexes();
					for (auto it = verts->begin(); it != verts->end(); ++it)
						fout << (**it) << "\n";


					


					vertPosition++;
					vertRotation++;
					vertexes3f++;
				}
				auto particleSysKoefs = &(**it_koef)[it_name->first][it_pair->first];

				fout << particleSysKoefs->a << "\n"
					<< particleSysKoefs->b << "\n"
					<< particleSysKoefs->c << "\n"
					<< particleSysKoefs->areaPos << " " << particleSysKoefs->areaRot << "\n"
					<< particleSysKoefs->chanceToAppear << "\n";
			}
		}
		it_presetner++;
		it_koef++;
	}

	//ParticleInfo

	fout << particleInfo.numOfParticles << "\n"
		<< particleInfo.speedOfOccur << "\n"
		<< particleInfo.particleFluct << "\n"
		<< particleInfo.particleFluctLen << "\n"
		<< particleInfo.particleRot << "\n"
		<< particleInfo.particleSpeed << "\n";

	fout << particleName << "\n";
	fout << *particleInfo.particle->getPosition() << " " << particleInfo.particle->getRotation() << "\n";
	fout << particleInfo.particle->getVertexes()->size() << "\n";

	auto vertPosition = particleInfo.particle->getVertPosition()->begin();
	auto vertRotation = particleInfo.particle->getVertRotation()->begin();
	auto vertexes3f = particleInfo.particle->getVertexes()->begin();
	for (int i = 0; i < particleInfo.particle->getVertexes()->size(); ++i)
	{
		fout << **vertPosition << " " << **vertRotation << "\n";
		fout << (*vertexes3f)->hasTexture() << "\n";
		fout << (*vertexes3f)->getMode() << " " << getNumOfPointsByMode((*vertexes3f)->getMode()) << "\n";

		for (auto it = (*vertexes3f)->getVertexes()->begin();
			it != (*vertexes3f)->getVertexes()->end(); ++it)
			fout << (**it) << "\n";
		//fout << (*vertexes3f)->getVertexes()

		if ((*vertexes3f)->hasTexture() == false)
		{
			auto colors = (*vertexes3f)->getColors();
			for (int i = 0; i < colors->size(); ++i)
				fout << *((*colors)[i]) << "\n";
		}
		else
		{
			auto textCoord = (*vertexes3f)->getTextureCoord();
			for (int i = 0; i < textCoord->size(); ++i)
			{
				fout << *((*textCoord)[i]) << "\n";
			}

			fout << (*vertexes3f)->getPathToTexture().c_str() << "\n";
		}

		vertPosition++;
		vertRotation++;
		vertexes3f++;
	}

	fout.close();
}

void Saver::saveMap(std::string path, DrawableObjectsPresenter2 * DOPresenter,
	ParticleSystemPresenter * PSPresenter, MoveBtnsController * moveBtnsControl)
{
	std::ofstream fout(path, std::ios_base::trunc);
	if (!fout.is_open()) {
		//char* err = "\nErr: DrawableObject::loadFromFile\tcan't open file.";

		//dbgprintf(err);
		//fin.close();
		return;
	}

	std::string name;
	int startPos, endPos;
	for (int i = path.length(); i >= 0; --i) {
		if (path[i] == '//' || path[i] == '\\') {
			startPos = i + 1;
			name = path.substr(startPos, endPos - startPos);
			//name.copy(path, endPos - startPos, startPos)
			break;
		}
		if (path[i] == '.')
			endPos = i;
	}

	int numOfPartSys = PSPresenter->size();
	int numOfObjects = DOPresenter->getNumOfObjs();

	fout << name << "\n";
	fout << numOfObjects << " " << numOfPartSys << "\n";

	auto objMapIndex = DOPresenter->getObjMapIndex();
	for (auto it_name = objMapIndex->begin(); it_name != objMapIndex->end(); ++it_name) {
		auto pairMap = &it_name->second;

		for (auto it_index = pairMap->begin(); it_index != pairMap->end(); ++it_index) {
			auto obj = DOPresenter->getObj(it_index->second);

			auto coords = obj->getAllGlobalCoord(moveBtnsControl->getCamera());
			auto OGLB_vect = obj->getVertexes();

			for (auto it_OGLB = OGLB_vect->begin(); it_OGLB != OGLB_vect->end(); ++it_OGLB) {
				std::vector<Vect2f*>::iterator p_it_texCoord;
				std::vector<Vect4f*>::iterator p_it_color;

				auto pVertexes = (*it_OGLB)->getVertexes();

				bool isTexCoord = false;
				bool isColor = false;

				if ((*it_OGLB)->getTextureCoord() != nullptr) {
					p_it_texCoord = (*it_OGLB)->getTextureCoord()->begin();
					isTexCoord = true;
				}
				else if ((*it_OGLB)->getColors() != nullptr) {
					p_it_color = (*it_OGLB)->getColors()->begin();
					isColor = true;
				}

				if (isTexCoord == false && isColor == false)
					break;

				switch ((*it_OGLB)->getMode())
				{

				case GL_TRIANGLES:
				{
					if (isTexCoord)
						fout << 1 << "\n";
					else if (isColor)
						fout << 0 << "\n";

					for (auto it = pVertexes->begin(); it != pVertexes->end(); ++it) {

						if (isTexCoord)
							fout << **p_it_texCoord << " " << (*coords)[*it] << "\n";
						else if (isColor)
							fout << **p_it_color << " " << (*coords)[*it] << "\n";
					}

					if (isTexCoord)
						fout << (*it_OGLB)->getPathToTexture() << "\n";

					break;
				}

				}

			}

			delete coords;
		}
	}

	//ParticlSystems
	objMapIndex = PSPresenter->getObjMapIndexes();

	for (auto it_name = objMapIndex->begin(); it_name != objMapIndex->end(); ++it_name) {
		auto pairMap = &it_name->second;

		for (auto it_index = pairMap->begin(); it_index != pairMap->end(); ++it_index) {
			auto obj = PSPresenter->getObj(it_name->first, it_index->first);

			auto str = getPartSysString(it_name->first, obj);
			fout << str << "\n";
		}
	}

	fout.close();

	std::string pathToFileForMapRedactor;
	pathToFileForMapRedactor.resize(path.length() + 1);


	for (int i = 0; i <  path.length() - 1; ++i) {
		pathToFileForMapRedactor[i] = path[i];
	}

	pathToFileForMapRedactor[path.length() - 1] = 'r';
	pathToFileForMapRedactor[path.length()] = 'm';


	saveMapForMapRedactor(pathToFileForMapRedactor, DOPresenter, PSPresenter, moveBtnsControl);
}

void Saver::saveMapForMapRedactor(std::string path, DrawableObjectsPresenter2 * DOPresenter, 
	ParticleSystemPresenter * PSPresenter, MoveBtnsController * moveBtnsControl)
{
	std::ofstream fout(path, std::ios_base::trunc);

	if (!fout.is_open())
		return;

	std::string name;
	int startPos, endPos;
	for (int i = path.length(); i >= 0; --i) {
		if (path[i] == '//' || path[i] == '\\') {
			startPos = i + 1;
			name = path.substr(startPos, endPos - startPos);
			//name.copy(path, endPos - startPos, startPos)
			break;
		}
		if (path[i] == '.')
			endPos = i;
	}

	int numOfPartSys = PSPresenter->size();
	int numOfObjects = DOPresenter->getNumOfObjs();

	fout << name << "\n";
	fout << numOfObjects << " " << numOfPartSys << "\n";

	auto objMapIndex = DOPresenter->getObjMapIndex();
	for (auto it_name = objMapIndex->begin(); it_name != objMapIndex->end(); ++it_name) {
		auto pairMap = &it_name->second;

		for (auto it_index = pairMap->begin(); it_index != pairMap->end(); ++it_index) {
			auto obj = DOPresenter->getObj(it_index->second);

			auto str = getDrObjString(it_name->first, obj);
			fout << str << "\n";
		}
	}

	//Particle Systems


	fout.close();
}

std::string Saver::getDrObjString(std::string name, DrawableObject2 * map)
{
	std::stringstream ss;

	std::string objName;
	Vect3f objPos, objRot;
	int numOfObjs;
	bool isTexture;
	unsigned int mode;
	//

	//std::getline(fin, objName);
	ss << name << "\n";
	ss << *map->getPosition() << " " << map->getRotation() << "\n";
	ss << map->getVertexes()->size() << "\n";



	auto vertPosition = map->getVertPosition()->begin();
	auto vertRotation = map->getVertRotation()->begin();
	auto vertexes3f = map->getVertexes()->begin();
	for (int i = 0; i < map->getVertexes()->size(); ++i)
	{
		ss << **vertPosition << " " << **vertRotation << "\n";
		ss << (*vertexes3f)->hasTexture() << "\n";
		ss << (*vertexes3f)->getMode() << " " << getNumOfPointsByMode((*vertexes3f)->getMode()) << "\n";

		for (auto it = (*vertexes3f)->getVertexes()->begin();
			it != (*vertexes3f)->getVertexes()->end(); ++it)
			ss << (**it) << "\n";
		//fout << (*vertexes3f)->getVertexes()

		if ((*vertexes3f)->hasTexture() == false)
		{
			auto colors = (*vertexes3f)->getColors();
			for (int i = 0; i < colors->size(); ++i)
				ss << *((*colors)[i]) << "\n";
		}
		else
		{
			auto textCoord = (*vertexes3f)->getTextureCoord();
			for (int i = 0; i < textCoord->size(); ++i)
			{
				ss << *((*textCoord)[i]) << "\n";
			}

			ss << (*vertexes3f)->getPathToTexture().c_str() << "\n";
		}

		vertPosition++;
		vertRotation++;
		vertexes3f++;
	}

	return ss.str();
}

std::string Saver::getPartSysString(std::string name, ParticleSystem * partSys)
{
	std::stringstream ss;

	auto presentersFromPS = &partSys->getPresenters();
	auto koefs = partSys->getKoefs();

	std::vector<DrawableObjectsPresenter2*> presenters = {
		presentersFromPS->first,
		presentersFromPS->second
	};

	auto particleInfo = partSys->getParticleInfo();

	auto it_presetner = presenters.begin();
	auto it_koef = koefs.begin();
	for (int j = 0; j < 2; ++j)
	{
		int numOfDrObjs = (*it_presetner)->getNumOfObjs();
		ss << numOfDrObjs << "\n";

		auto objIndexes = (*it_presetner)->getObjMapIndex();
		for (auto it_name = objIndexes->begin(); it_name != objIndexes->end(); ++it_name) {
			auto pairMap = &it_name->second;

			for (auto it_pair = pairMap->begin(); it_pair != pairMap->end(); ++it_pair) {
				auto obj = (*it_presetner)->getObj(it_name->first, it_pair->second);
				auto OGLB_verts = obj->getVertexes();

				ss << it_name->first << "\n";
				ss << *obj->getPosition() << " " << obj->getRotation() << "\n";
				ss << OGLB_verts->size() << "\n";

				auto vertPosition = obj->getVertPosition()->begin();
				auto vertRotation = obj->getVertRotation()->begin();
				auto vertexes3f = OGLB_verts->begin();

				for (int i = 0; i < OGLB_verts->size(); ++i) {
					ss << **vertPosition << " " << **vertRotation << "\n";

					auto verts = (*vertexes3f)->getVertexes();
					for (auto it = verts->begin(); it != verts->end(); ++it)
						ss << (**it) << "\n";





					vertPosition++;
					vertRotation++;
					vertexes3f++;
				}
				auto particleSysKoefs = &(**it_koef)[it_name->first][it_pair->first];

				ss << particleSysKoefs->a << "\n"
					<< particleSysKoefs->b << "\n"
					<< particleSysKoefs->c << "\n"
					<< particleSysKoefs->areaPos << " " << particleSysKoefs->areaRot << "\n"
					<< particleSysKoefs->chanceToAppear << "\n";
			}
		}
		it_presetner++;
		it_koef++;
	}

	//ParticleInfo

	ss << particleInfo->numOfParticles << "\n"
		<< particleInfo->speedOfOccur << "\n"
		<< particleInfo->particleFluct << "\n"
		<< particleInfo->particleFluctLen << "\n"
		<< particleInfo->particleRot << "\n"
		<< particleInfo->particleSpeed << "\n";

	ss << particleInfo->particleName << "\n";
	ss << *particleInfo->particle->getPosition() << " " << particleInfo->particle->getRotation() << "\n";
	ss << particleInfo->particle->getVertexes()->size() << "\n";

	auto vertPosition = particleInfo->particle->getVertPosition()->begin();
	auto vertRotation = particleInfo->particle->getVertRotation()->begin();
	auto vertexes3f = particleInfo->particle->getVertexes()->begin();
	for (int i = 0; i < particleInfo->particle->getVertexes()->size(); ++i)
	{
		ss << **vertPosition << " " << **vertRotation << "\n";
		ss << (*vertexes3f)->hasTexture() << "\n";
		ss << (*vertexes3f)->getMode() << " " << getNumOfPointsByMode((*vertexes3f)->getMode()) << "\n";

		for (auto it = (*vertexes3f)->getVertexes()->begin();
			it != (*vertexes3f)->getVertexes()->end(); ++it)
			ss << (**it) << "\n";
		//fout << (*vertexes3f)->getVertexes()

		if ((*vertexes3f)->hasTexture() == false)
		{
			auto colors = (*vertexes3f)->getColors();
			for (int i = 0; i < colors->size(); ++i)
				ss << *((*colors)[i]) << "\n";
		}
		else
		{
			auto textCoord = (*vertexes3f)->getTextureCoord();
			for (int i = 0; i < textCoord->size(); ++i)
			{
				ss << *((*textCoord)[i]) << "\n";
			}

			ss << (*vertexes3f)->getPathToTexture().c_str() << "\n";
		}

		vertPosition++;
		vertRotation++;
		vertexes3f++;
	}

	return ss.str();
}
