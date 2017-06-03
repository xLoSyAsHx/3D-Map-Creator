#include "LoggerMoveVerts.h"

LoggerMoveVerts::LoggerMoveVerts(std::vector<Vect3f*>* vect, Vect3f shift) :
	BaseLoggerObject(), vect(vect), shift(shift)
{
}

std::vector<Vect3f*>* LoggerMoveVerts::getVertVect()
{
	return vect;
}

Vect3f LoggerMoveVerts::getShift()
{
	return shift;
}

LoggerMoveVerts::~LoggerMoveVerts()
{
	delete vect;
}
