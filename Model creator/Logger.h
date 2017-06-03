#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>

#include "rotationDlg.h"
#include "resource.h"

#include "DlgHelper.h"
#include "structs.h"

#include "ParticleSystem.h"
#include "DrawableObject2.h"
#include "DrawableObjectPresenter2.h"

#include "LoggerRotObjs.h"
#include "LoggerMoveObjs.h"
#include "LoggerMoveVerts.h"
#include "LoggerScalObjs.h"
#include "LoggerSetTexColor.h"
#include "LoggerCreateDelObj.h"
#include "LoggerMovePartSys.h"

#include "BaseLoggerObject.h"

class Logger
{
public:
	static enum LogType { Move_Objs, Move_Verts, Rot_Objs, Scal_Objs, Set_Color, 
		Set_Texture, Create_Obj, Delete_Obj, Move_PartSys };

private:
	//DrawableObjectsPresenter2* DOPresenter = nullptr;
	std::map<std::string, DrawableObjectsPresenter2*> DOPresenterMap;
	std::map<std::string, HWND> *elementsMap;

	std::vector<BaseLoggerObject*> sysLog;
	std::vector<LogType> logTypeVect;
	int index = -1;

	void check();
	void applyChangesFromLog();
	void rollBackChangesFromLog();

	void changeDlgData(bool isDlgExist, Vect3f iShift, int e1, int e2, int e3);
	void updateAllObjs();
public:

	void addDrObjPresenter(std::string name, DrawableObjectsPresenter2 * presenter);

	void setElementsMap(std::map<std::string, HWND> *_map);

	void log(void *obj, void * _struct, LogType type);

	void forward();
	void backward();

	void clear();
};

#endif LOGGER_H