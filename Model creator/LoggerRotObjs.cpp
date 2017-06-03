#include "LoggerRotObjs.h"

LoggerRotObjs::LoggerRotObjs(std::vector<DrawableObject2*>* vect, Vect3f shift) :
	BaseLoggerObject(), vect(vect), shift(shift)
{
}

std::vector<DrawableObject2*>* LoggerRotObjs::getDrObjsVect()
{
	return vect;
}

Vect3f LoggerRotObjs::getShift() const
{
	return shift;
}

LoggerRotObjs::~LoggerRotObjs()
{
	delete vect;
}
