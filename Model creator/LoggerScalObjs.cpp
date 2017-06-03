#include "LoggerScalObjs.h"

LoggerScalObjs::LoggerScalObjs(std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>  *vect,
	std::map<Vect3f*, Vect3f> *shifts, Vect3f oldScale, Vect3f newScale) :
	BaseLoggerObject(), vect(vect), shifts(shifts), oldScale(oldScale), newScale(newScale)
{
}

std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>  * LoggerScalObjs::getVertVect()
{
	return vect;
}

std::map<Vect3f*, Vect3f> * LoggerScalObjs::getShifts() const
{
	return shifts;
}

Vect3f LoggerScalObjs::getOldScale()
{
	return oldScale;
}

Vect3f LoggerScalObjs::getNewScale()
{
	return newScale;
}

LoggerScalObjs::~LoggerScalObjs()
{
	delete vect;
	delete shifts;
}
