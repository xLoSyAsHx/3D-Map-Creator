#ifndef LOGGER_SCAL_OBJS_H
#define LOGGER_SCAL_OBJS_H

#include <iostream>
#include <map>

#include "DrawableObject2.h"
#include "Vect3f.h"

#include "BaseLoggerObject.h"

class LoggerScalObjs :
	public BaseLoggerObject
{
	std::map<Vect3f*, Vect3f> *shifts;
	Vect3f oldScale, newScale;
	std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>  *vect;

	LoggerScalObjs(const LoggerScalObjs&) = delete;
	void operator=(const LoggerScalObjs&) = delete;
public:
	LoggerScalObjs(std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>  *vect, std::map<Vect3f*, Vect3f> *shifts,
		Vect3f oldScale, Vect3f newScale);

	std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>  *getVertVect();

	std::map<Vect3f*, Vect3f> * getShifts() const;

	Vect3f getOldScale();

	Vect3f getNewScale();

	~LoggerScalObjs();
};

#endif LOGGER_MOVE_OBJS_H