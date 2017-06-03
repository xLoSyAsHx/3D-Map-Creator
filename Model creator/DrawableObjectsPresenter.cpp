#include "DrawableObjectsPresenter.h"



DrawableObjectsPresenter::DrawableObjectsPresenter()
{
}

DrawableObjectsPresenter::DrawableObjectsPresenter(
	std::map<std::string, int> commonMapIndex, std::map<std::string, int> transMapIndex,
	std::vector<DrawableObject*> commonMap,	std::vector<DrawableObject*> transMap) :
	commonMapIndex(commonMapIndex), transMapIndex(transMapIndex),
	commonMap(commonMap), transMap(transMap)
{
}

DrawableObject * DrawableObjectsPresenter::getObj(std::string name)
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

DrawableObject * DrawableObjectsPresenter::getObj(std::string name, int id)
{
	if (name == "common")
		if (id >= 0 && id < commonMap.size() - 1)
			return commonMap[id];

	if (name == "trans")
		if (id >= 0 && id < transMap.size() - 1)
			return transMap[id];

	return nullptr;
}

void DrawableObjectsPresenter::addObj(DrawableObject *obj, std::string name)
{
	std::map<std::string, int>::iterator t_it, c_it;
	if ((c_it = commonMapIndex.find(name)) == commonMapIndex.end() &&
		(t_it = transMapIndex.find(name)) == transMapIndex.end())
	{
		if (obj->isTrans() == true)
		{
			transMap.push_back(obj);
			transMapIndex[name] = transMap.size() - 1;
		}
		else
		{
			commonMap.push_back(obj);
			commonMapIndex[name] = commonMap.size() - 1;
		}
	}



}

std::map<std::string, std::vector<std::string>> DrawableObjectsPresenter::getKeys()
{
	std::vector<std::string> m_keys, t_keys;

	for (auto it = commonMapIndex.begin(); it != commonMapIndex.end(); ++it)
		m_keys.push_back(it->first);

	for (auto it = transMapIndex.begin(); it != transMapIndex.end(); ++it)
		t_keys.push_back(it->first);

	return std::map<std::string, std::vector<std::string>>{ {"common", m_keys}, {"trans", t_keys}};
}

void DrawableObjectsPresenter::show()
{
	for (auto obj : transMap)
		obj->draw();

	for (auto obj : commonMap)
		obj->draw();
}


DrawableObjectsPresenter* DrawableObjectsPresenter::loadFromFile(std::string path) // Õ≈ –¿¡Œ◊¿ﬂ ‘”Õ ÷»ﬂ
{
	/*
	//ÔÂÂ‰ÂÎ‡Ú¸
	std::fstream fin(path);
	if (!fin.is_open()) {
		char* err = "\nErr: DrawableObject::loadFromFile\tcan't open file.";

		///dbgprintf(err);
		fin.close();
		return nullptr;
	}

	
	commonSize transSize

	name
	glMode
	num_of_points
	x y z
	...
	x y z
	
	std::map<std::string, int> commonMapIndex, transMapIndex;
	std::vector<DrawableObject*> commonMap, transMap;

	int commonSize, transSize;
	fin >> commonSize >> transSize;

	for (int k = 0; k < transSize; ++k)
	{
		std::string name;
		fin >> name;

		int mode;
		fin >> mode;

		int vertSize;
		fin >> vertSize;

		GLfloat alpha;
		fin >> alpha;

		std::vector<Vect*> vertxMas;
		vertxMas.resize(vertSize);
		for (int i = 0; i < vertSize; ++i) {
			GLfloat x, y, z;
			fin >> x >> y >> z;
			vertxMas[i] = new Vect( x, y, z);
		}

		transMap.push_back(new DrawableObject(vertxMas, mode, alpha));
		transMapIndex[name] = transMap.size();
	}

	for (int k = 0; k < commonSize; ++k)
	{
		std::string name;
		fin >> name;

		int mode;
		fin >> mode;

		int vertSize;
		fin >> vertSize;

		std::vector<Vect*> vertxMas;
		vertxMas.resize(vertSize);
		for (int i = 0; i < vertSize; ++i) {
			float x, y, z;
			fin >> x >> y >> z;
			vertxMas[i] = new Vect(x, y, z);
		}

		commonMap.push_back(new DrawableObject(vertxMas, mode));
		commonMapIndex[name] = commonMap.size();
	}
	fin.close();
	*/
	return new DrawableObjectsPresenter();
}

DrawableObjectsPresenter::~DrawableObjectsPresenter()
{
}
