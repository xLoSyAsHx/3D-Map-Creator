#include "LoggerMoveObjs.h"

LoggerMoveObjs::LoggerMoveObjs(std::vector<DrawableObject2*>* vect, Vect3f shift) :
	BaseLoggerObject(), vect(vect), shift(shift)
{
}

std::vector<DrawableObject2*>* LoggerMoveObjs::getDrObjsVect()
{
	return vect;
}

Vect3f LoggerMoveObjs::getShift() const
{
	return shift;
}
