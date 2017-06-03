#ifndef LOGGER_CREATE_DEL_OBJ_H
#define LOGGER_CREATE_DEL_OBJ_H

#include <iostream>

#include "DrawableObject2.h"

#include "structs.h"

#include "BaseLoggerObject.h"

class LoggerCreateDelObj :
	public BaseLoggerObject
{
	DrawableObject2 *drObj = nullptr;
	StructForCreateObj additionalInfo;
public:
	LoggerCreateDelObj(DrawableObject2 *drObj, StructForCreateObj _struct);

	DrawableObject2 * getDrObj();

	StructForCreateObj getAdditionalInfo();

	std::string getPresenterName();
};

#endif LOGGER_CREATE_DEL_OBJ_H