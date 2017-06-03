#include "DrawableObjectPresenter2.h"
//#include "DrawableObjectsPresenter2.h"



OpenGL_Base * DrawableObjectsPresenter2::generateSimilarOGL_B(OpenGL_Base * base)
{
	OpenGL_Base* newObj = new OpenGL_Base();

	newObj->setMode(base->mode);

	std::vector<Vect4f*> *vects4 = nullptr;
	if (base->colors != nullptr) {
		vects4 = new std::vector<Vect4f*>;

		for (auto it = base->colors->begin(); it != base->colors->end(); ++it) {
			GLfloat x = (*it)->getX();
			GLfloat y = (*it)->getY();
			GLfloat z = (*it)->getZ();

			Vect4f *vect = new Vect4f(x, y, z);
			vects4->push_back(vect);
		}
	}
	newObj->setColors(vects4);

	std::vector<Vect3f*> *vects3 = nullptr;
	if (base->vertexes != nullptr) {
		vects3 = new std::vector<Vect3f*>;

		for (auto it = base->vertexes->begin(); it != base->vertexes->end(); ++it) {
			GLfloat x = (*it)->getX();
			GLfloat y = (*it)->getY();
			GLfloat z = (*it)->getZ();

			Vect3f *vect = new Vect3f(x, y, z);
			vects3->push_back(vect);
		}
	}
	newObj->setVertexes(vects3);

	std::vector<Vect2f*> *vects2 = nullptr;
	if (base->textureCoord != nullptr) {
		vects2 = new std::vector<Vect2f*>;

		for (auto it = base->textureCoord->begin(); it != base->textureCoord->end(); ++it) {
			GLfloat x = (*it)->getX();
			GLfloat y = (*it)->getY();

			Vect2f *vect = new Vect2f(x, y);
			vects2->push_back(vect);
		}
		
		GLuint *texture = new GLuint;
		LoadGLTextures(base->pathToTexture, texture);
		newObj->setTexture(texture, vects2, base->getPathToTexture());
	}
	
	
	return newObj;
}

DrawableObject2 * DrawableObjectsPresenter2::genegateSimilarDrObj(DrawableObject2 * obj)
{
	std::vector<OpenGL_Base*> *vertexMas = new std::vector<OpenGL_Base*>;
	for (auto it = obj->vertexes3f->begin(); it != obj->vertexes3f->end(); ++it)
		vertexMas->push_back(generateSimilarOGL_B(*it));


	std::vector<Vect3f*>* position = new std::vector<Vect3f*>;
	for (auto it = obj->vertPosition->begin(); it != obj->vertPosition->end(); ++it) {
		GLfloat x = (*it)->getX();
		GLfloat y = (*it)->getY();
		GLfloat z = (*it)->getZ();

		Vect3f *vect = new Vect3f(x, y, z);
		position->push_back(vect);
	}


	std::vector<Vect3f*>* rotation = new std::vector<Vect3f*>;
	for (auto it = obj->vertRotation->begin(); it != obj->vertRotation->end(); ++it) {
		GLfloat x = (*it)->getX();
		GLfloat y = (*it)->getY();
		GLfloat z = (*it)->getZ();

		Vect3f *vect = new Vect3f(x, y, z);
		rotation->push_back(vect);
	}

	DrawableObject2 *drawableObject = new DrawableObject2(
		vertexMas,
		position,
		rotation,
		*obj->getPosition(),
		//Vect3f(-45, -45, 0)
		obj->getRotation()
	);



	return drawableObject;
}

DrawableObjectsPresenter2 * DrawableObjectsPresenter2::genSimilarDrObjPresenter(CameraLocation & const cam)
{
	DrawableObjectsPresenter2* drObjPresenter = new DrawableObjectsPresenter2();

	for (auto it_name = objMapIndex->begin(); it_name != objMapIndex->end(); ++it_name) {
		auto map = &it_name->second;
		for (auto it = map->begin(); it != map->end(); ++it) {
			auto obj = getObj(it->second);

			drObjPresenter->addObj(obj, it_name->first, cam);
		}
	}

	return drObjPresenter;
}

DrawableObjectsPresenter2::DrawableObjectsPresenter2()
{
	objMapIndex = new std::map<std::string, std::map<int, int>>;
}

DrawableObjectsPresenter2::DrawableObjectsPresenter2(
	std::map<std::string, std::map<int, int>> *objMapIndex, std::vector<DrawableObject2*> objMap) :
	objMapIndex(objMapIndex), objMap(objMap)
{
}





//DrawableObjectsPresenter2::DrawableObjectsPresenter2()
/*
DrawableObjectsPresenter2::DrawableObjectsPresenter2(
	std::map<std::string, int> commonMapIndex, std::map<std::string, int> transMapIndex,
	std::vector<DrawableObject*> commonMap, std::vector<DrawableObject*> transMap) :
	commonMapIndex(commonMapIndex), transMapIndex(transMapIndex),
	commonMap(commonMap), transMap(transMap)
{
}
*/



/*
DrawableObject2 * DrawableObjectsPresenter2::getObj(std::string name)
{
	std::map<std::string, int>::iterator t_it, c_it;
	if ((c_it = commonMapIndex.find(name)) != commonMapIndex.end() ||
		(t_it = transMapIndex.find(name)) != transMapIndex.end())
	{
		if (c_it != commonMapIndex.end())
			return commonMap[c_it->second];
		else
			return transMap[t_it->second];
	}
	return nullptr;
}
*/
HWND DrawableObjectsPresenter2::getMainHWND() const
{
	return hWnd;
}

void DrawableObjectsPresenter2::setMainHWND(HWND _hWnd)
{
	hWnd = _hWnd;
}

int DrawableObjectsPresenter2::getobjMapSize()
{
	return objMap.size();
}

int DrawableObjectsPresenter2::getNumOfObjs()
{
	return objMap.size();
}

bool DrawableObjectsPresenter2::isObjExist(std::string name, int num)
{
	if (objMapIndex->find(name) != objMapIndex->end())
		if ((*objMapIndex)[name].find(num) != (*objMapIndex)[name].end())
			return true;
		
	return false;
}

DrawableObject2 * DrawableObjectsPresenter2::getObj(int id)
{
	if (id >= 0 && id < objMap.size())
		return objMap[id];

	return nullptr;
}

DrawableObject2 * DrawableObjectsPresenter2::getObj(std::string name, int num)
{
	auto it = objMapIndex->find(name);

	if (it != objMapIndex->end()) {
		if (num >= 0 && num < (*objMapIndex)[name].size())
		return getObj(it->second[num]);
	}

	return nullptr;
}

int DrawableObjectsPresenter2::getObjId(std::string name, int index)
{
	return (*objMapIndex)[name][index];
}

std::pair<std::string, int> DrawableObjectsPresenter2::getNameIndexById(int id)
{
	for (auto name : getObjNames()) {
		for (auto pair : (*objMapIndex)[name]) {
			if (pair.second == id)
				return{ name, pair.first };
		}
	}

	return {"error", -1};
}

int DrawableObjectsPresenter2::addObj(DrawableObject2 *obj, std::string name, CameraLocation const &cam)
{
	auto ptr = genegateSimilarDrObj(obj);
	ptr->setPosition(Vect3f(-cam.getX(), -cam.getY(), -cam.getZ()));
	ptr->move(*obj->getPosition());

	/*
	Vect3f direct(0, 0, -1);

	direct = Matrix::rotationMatrix_X(direct, cam.getYAngle());
	direct = Matrix::rotationMatrix_Y(direct, cam.getXAngle());
	direct = direct.mul(5.0f);

	ptr->move(direct);
	*/
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

void DrawableObjectsPresenter2::addAlreadyExistObj(DrawableObject2 * obj,
	std::string name, int index)
{
	objMap.push_back(obj);

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
}

std::vector<std::string> DrawableObjectsPresenter2::getObjNames()
{
	std::vector<std::string> keys;

	for (auto it = objMapIndex->begin(); it != objMapIndex->end(); ++it)
		keys.push_back(it->first);


	return keys;
}

std::vector<int> DrawableObjectsPresenter2::getObjectIndexes(std::string name)
{
	std::vector<int> indexes;

	for (auto it = (*objMapIndex)[name].begin(); it != (*objMapIndex)[name].end(); ++it)
		indexes.push_back(it->first);
	//проверить правильность currentVertexes 
	return indexes;
}

std::map<std::string, std::map<int, int>>* DrawableObjectsPresenter2::getObjMapIndex()
{
	return objMapIndex;
}

void DrawableObjectsPresenter2::eraseObjFromObjMap(int index)
{
	std::vector<DrawableObject2*>::iterator itForDel = objMap.begin();

	for (int i = 0; i < index; ++i)
		itForDel++;

	if (itForDel != objMap.end())
		objMap.erase(itForDel);

	//objMap.erase()
}

void DrawableObjectsPresenter2::clear()
{
	for (auto map : *objMapIndex)
		map.second.clear();
	objMapIndex->clear();
	//delete objMapIndex;

	for (auto obj : objMap)
		delete obj;
	objMap.clear();
}

std::pair<int, OpenGL_Base*> DrawableObjectsPresenter2::getOJLBByIdAndPosVertPtr(std::pair<int, Vect3f*>& pair)
{
	auto drawableObjVerts = getObj(pair.first)->getVertexes();

	bool canExit = false;
	for (auto it_OJLB = drawableObjVerts->begin(); it_OJLB != drawableObjVerts->end(); ++it_OJLB)
		for (auto it = (*it_OJLB)->getVertexes()->begin(); it != (*it_OJLB)->getVertexes()->end(); ++it)
			if ((*it) == pair.second)
				return { it_OJLB - drawableObjVerts->begin(), *it_OJLB };

	return { -1, nullptr};
}



void DrawableObjectsPresenter2::show(CameraLocation const &cam,
	std::map<std::string, std::map<int, int>> &selectedMap, bool isVertexesEnable,
	std::map<int, std::map<Vect3f*, vector3ub>> &colorMapForVertexes,
	std::pair<int, Vect3f*> &vertForLink,
	std::map<int, std::set<OpenGL_Base*>> &OGLB_ForMappedTexture)
{
	int i = 0;
	auto nameIndex = getNameIndexById(i);

	std::vector<SelectedCube> cubes;

	for (auto it_name = objMapIndex->begin(); it_name != objMapIndex->end(); ++it_name) {
		for (auto it_pair = it_name->second.begin(); it_pair != it_name->second.end(); ++it_pair) {
			bool showVertexes = false;
			bool pushCube = false;

			if (selectedMap.find(it_name->first) != selectedMap.end())
				if (selectedMap[it_name->first].find(it_pair->first) != selectedMap[it_name->first].end()) {
					pushCube = true;
					showVertexes = true;
				}

			if (!isVertexesEnable)
				showVertexes = false;	

			SelectedCube cube;
			/*
			bool isVertForOGLB_On = false;
			if (OGLB_ForMappedTexture.find(it_pair->second) != OGLB_ForMappedTexture.end())
			{
				auto OGLB_vect = objMap[it_pair->second]->getVertexes();
				for (auto it_objMap = OGLB_vect->begin(); it_objMap != OGLB_vect->end(); ++it_objMap)
					for (auto it_OGLB_ForTex = OGLB_ForMappedTexture[it_pair->second].begin();
						it_OGLB_ForTex != OGLB_ForMappedTexture[it_pair->second].end(); ++it_OGLB_ForTex)
						if ((*it_objMap) == (*it_OGLB_ForTex)) {//проверяем для каждого DrawableObj есть ли
							isVertForOGLB_On = true;		    //в нём OGLB такой же, как  в OGLB_ForMapTex

							goto DOP_nextStep;					//Если да, то для этого объекта надо показать
						}									    //точки для прикрепления текстуры
			}

			DOP_nextStep:
			*/
			if (OGLB_ForMappedTexture.size() != 0)
				std::cout << 1;

			bool isObjFind = false;
			if (OGLB_ForMappedTexture.find(it_pair->second) != OGLB_ForMappedTexture.end())
				isObjFind = true;


			//std::map<Vect3f*, vector3ub> mapVectPtr_color;
			
			if (colorMapForVertexes.find(it_pair->second) != colorMapForVertexes.end())
			{
				if (isObjFind)
					cube = objMap[it_pair->second]->draw(cam, showVertexes,
						colorMapForVertexes[it_pair->second], vertForLink,
						OGLB_ForMappedTexture[it_pair->second]);
				else
					cube = objMap[it_pair->second]->draw(cam, showVertexes,
						colorMapForVertexes[it_pair->second], vertForLink,
						std::set<OpenGL_Base*>());
			}
			else
			{
				if (isObjFind)
					cube = objMap[it_pair->second]->draw(cam, showVertexes,
						std::map<Vect3f*, vector3ub>(), vertForLink, 
						OGLB_ForMappedTexture[it_pair->second]);
				else
					cube = objMap[it_pair->second]->draw(cam, showVertexes,
						std::map<Vect3f*, vector3ub>(), vertForLink,
						std::set<OpenGL_Base*>());
			}

			if (pushCube)
				cubes.push_back(cube);
		}
	}

	for (auto obj : cubes)
		obj.draw();


}

void DrawableObjectsPresenter2::simpleShow(std::map<int, vector3ub> &colorMapForObjects)
{
	
	int index = 0;

	for (auto it_name = objMapIndex->begin(); it_name != objMapIndex->end(); ++it_name)
		for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it)
			objMap[it->second]->simpleDraw(colorMapForObjects[it->second]);
}

void DrawableObjectsPresenter2::simpleShowVertexes(std::map<std::string, std::map<int, int>> currentObjs,
	std::map<int, std::map<Vect3f*, vector3ub>> &colorMapForVecrtexes)
{

	for (auto it_name = currentObjs.begin(); it_name != currentObjs.end(); ++it_name)
		for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it)
			objMap[it->second]->simpleDrawVertexes(colorMapForVecrtexes[it->second]);
}

vector3ub DrawableObjectsPresenter2::getColorForSimpleDraw(std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>>& map)
{
	if (map.size() == 0)
		return{ 0,0,1 };

	long new_map_size = map.size() + 1;
	std::vector<bool> temp;

	temp.resize(new_map_size, false);
	temp[0] = true;

	for (auto it_pr = map.begin(); it_pr != map.end(); ++it_pr)
		for (auto it = it_pr->second.begin(); it != it_pr->second.end(); ++it) {
			if (it->second < new_map_size)
				temp[(long)it->second] = true;
		}

	auto it = std::find(temp.begin(), temp.end(), false);


	if (it != temp.end())
		return to_vector3ub(long(it - temp.begin()));
	else
		return to_vector3ub(new_map_size);
}

std::string DrawableObjectsPresenter2::loadFromFile(std::string path, std::map<std::string, DrawableObject2*> &map)
{
	std::ifstream fin(path);
	if (!fin.is_open()) {
		//char* err = "\nErr: DrawableObject::loadFromFile\tcan't open file.";

		//dbgprintf(err);
		//fin.close();
		return "";
	}

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

	if (map.find(objName) == map.end())
		map[objName] = drawableObject;
	else
	{
		delete vertexes3f;
		delete vertPosition;
		delete vertRotation;
	}
	/*
	auto keys = this->getKeys();
	bool isNameFind = false;
	for (auto it = keys.begin(); it != keys.end(); ++it)
		if (*it == objName) {
			isNameFind = true;
			break;
		}

	if (!isNameFind)
	{
		this->addObj(drawableObject, objName);
	}
	else
	{
		delete vertexes3f;
		delete vertPosition;
		delete vertRotation;
	}
	*/
	return objName;
}

void DrawableObjectsPresenter2::saveToFile(std::string path, DrawableObject2* map)

{
	std::ofstream fout(path, std::ios_base::trunc);
	if (!fout.is_open()) {
		//char* err = "\nErr: DrawableObject::loadFromFile\tcan't open file.";

		//dbgprintf(err);
		//fin.close();
		return;
	}

	std::string objName;
	Vect3f objPos, objRot;
	int numOfObjs;
	bool isTexture;
	unsigned int mode;
	//
	

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

	//std::getline(fin, objName);
	fout << name << "\n";
	fout << *map->getPosition() << " " << map->getRotation() << "\n";
	fout << map->getVertexes()->size() << "\n";
	


	auto vertPosition = map->getVertPosition()->begin();
	auto vertRotation = map->getVertRotation()->begin();
	auto vertexes3f = map->getVertexes()->begin();
	for (int i = 0; i < map->getVertexes()->size(); ++i)
	{
		fout << **vertPosition << " " << **vertRotation << "\n";
		fout << (*vertexes3f)->hasTexture() << "\n";
		fout << (*vertexes3f)->mode << " " << getNumOfPointsByMode((*vertexes3f)->mode) << "\n";
		
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

DrawableObjectsPresenter2::~DrawableObjectsPresenter2(){}