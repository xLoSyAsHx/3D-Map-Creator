#ifndef LOGGER_SET_TEX_COLOR
#define LOGGER_SET_TEX_COLOR

#include <iostream>
#include <vector>

#include "OpenGL_Base.h"
#include "Vect4f.h"

#include "structs.h"

#include "BaseLoggerObject.h"

class LoggerSetTexColor :
	public BaseLoggerObject
{
	std::vector<OpenGL_Base*> *vect;
	std::vector<ColorTexture*>* oldStruct;
	ColorTexture *newStruct;
	bool canDelete;

	LoggerSetTexColor(const LoggerSetTexColor&) = delete;
	void operator=(const LoggerSetTexColor&) = delete;
public:
	LoggerSetTexColor(std::vector<OpenGL_Base*> *vect, bool canDelete, std::pair<ColorTexture*, std::vector<ColorTexture*>*> pair);

	std::vector<OpenGL_Base*> * getOGLB_vect();
	std::vector<ColorTexture*>* getOldStruct();
	ColorTexture*  getNewStruct();


	~LoggerSetTexColor();
};

#endif // !LOGGER_SET_TEX_COLOR