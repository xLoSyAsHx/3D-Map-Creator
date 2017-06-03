#include "LoggerCreateDelObj.h"

LoggerCreateDelObj::LoggerCreateDelObj(DrawableObject2 * drObj,
	StructForCreateObj _struct) :
	drObj(drObj), additionalInfo(_struct)
{
}

DrawableObject2 * LoggerCreateDelObj::getDrObj()
{
	return drObj;
}

StructForCreateObj LoggerCreateDelObj::getAdditionalInfo()
{
	return additionalInfo;
}

std::string LoggerCreateDelObj::getPresenterName()
{
	return additionalInfo.presenterName;
}
