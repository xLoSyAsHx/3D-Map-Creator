#ifndef DRAWABLE_OBJECTS_PRESENTER_H
#define DRAWABLE_OBJECTS_PRESENTER_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "DrawableObject.h"


class DrawableObjectsPresenter
{
	std::map<std::string, int> commonMapIndex, transMapIndex;
	std::vector<DrawableObject*> commonMap, transMap;
public:

	DrawableObjectsPresenter();

	DrawableObjectsPresenter(
		std::map<std::string, int> commonMapIndex, std::map<std::string, int> transMapIndex, 
		std::vector<DrawableObject*> commonMap,	std::vector<DrawableObject*> transMap);

	DrawableObject* getObj(std::string name);

	DrawableObject* getObj(std::string name, int id);

	void addObj(DrawableObject* obj, std::string name);

	std::map<std::string, std::vector<std::string>> getKeys();

	void show();

	static DrawableObjectsPresenter* loadFromFile(std::string path);

	~DrawableObjectsPresenter();
};

#endif DRAWABLE_OBJECTS_PRESENTER_H