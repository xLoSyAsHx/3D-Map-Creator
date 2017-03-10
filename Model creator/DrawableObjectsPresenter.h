#ifndef DRAWABLE_OBJECTS_PRESENTER_H
#define DRAWABLE_OBJECTS_PRESENTER_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "DrawableObject.h"

class DrawableObjectsPresenter
{
	std::map<std::string, DrawableObject> commonMap, transMap;
public:
	DrawableObjectsPresenter();



	~DrawableObjectsPresenter();
};

#endif DRAWABLE_OBJECTS_PRESENTER_H