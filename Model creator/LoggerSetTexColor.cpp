#include "LoggerSetTexColor.h"

LoggerSetTexColor::LoggerSetTexColor(std::vector<OpenGL_Base*>* vect, bool canDelete,
	std::pair<ColorTexture*, std::vector<ColorTexture*>*> pair) :
	vect(vect), canDelete(canDelete)
{
	newStruct = pair.first;
	oldStruct = pair.second;
}

std::vector<OpenGL_Base*>* LoggerSetTexColor::getOGLB_vect()
{
	return vect;
}

std::vector<ColorTexture*>* LoggerSetTexColor::getOldStruct()
{
	return oldStruct;
}

ColorTexture * LoggerSetTexColor::getNewStruct()
{
	return newStruct;
}

LoggerSetTexColor::~LoggerSetTexColor()
{
	for (auto it = oldStruct->begin(); it != oldStruct->end(); ++it)
		delete *it;

	delete oldStruct;
	delete newStruct;
	if (canDelete)
		delete vect;
}
