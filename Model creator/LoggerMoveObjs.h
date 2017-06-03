#ifndef LOGGER_MOVE_OBJS_H
#define LOGGER_MOVE_OBJS_H

#include <iostream>
#include <vector>

#include "DrawableObject2.h"
#include "Vect3f.h"

#include "BaseLoggerObject.h"

class LoggerMoveObjs :
	public BaseLoggerObject
{
	Vect3f shift;
	std::vector<DrawableObject2*> *vect;

	LoggerMoveObjs(const LoggerMoveObjs&) = delete;
	void operator=(const LoggerMoveObjs&) = delete;
public:
	LoggerMoveObjs(std::vector<DrawableObject2*> *vect, Vect3f shift);

	std::vector<DrawableObject2*> *getDrObjsVect();

	Vect3f getShift() const;
};

#endif LOGGER_MOVE_OBJS_H