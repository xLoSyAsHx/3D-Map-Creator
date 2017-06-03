#ifndef LOGGER_ROT_OBJS_H
#define LOGGER_ROT_OBJS_H

#include <iostream>
#include <vector>

#include "DrawableObject2.h"
#include "Vect3f.h"

#include "BaseLoggerObject.h"

class LoggerRotObjs :
	public BaseLoggerObject
{
	Vect3f shift;
	std::vector<DrawableObject2*> *vect;

	LoggerRotObjs(const LoggerRotObjs&) = delete;
	void operator=(const LoggerRotObjs&) = delete;
public:
	LoggerRotObjs(std::vector<DrawableObject2*> *vect, Vect3f shift);

	std::vector<DrawableObject2*> *getDrObjsVect();

	Vect3f getShift() const;

	~LoggerRotObjs();
};

#endif LOGGER_MOVE_OBJS_H