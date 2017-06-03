#ifndef LOGGER_MOVE_VERTS_H
#define LOGGER_MOVE_VERTS_H

#include <iostream>
#include <vector>

#include "Vect3f.h"

#include "BaseLoggerObject.h"

class LoggerMoveVerts :
	public BaseLoggerObject
{
	Vect3f shift;
	std::vector<Vect3f*> *vect;

	LoggerMoveVerts(const LoggerMoveVerts&) = delete;
	void operator=(const LoggerMoveVerts&) = delete;
public:
	LoggerMoveVerts(std::vector<Vect3f*> *vect, Vect3f shift);

	std::vector<Vect3f*> * getVertVect();

	Vect3f getShift();

	~LoggerMoveVerts();
};

#endif LOGGER_MOVE_VERTS_H