#ifndef LIST_CONTROLLER_H
#define LIST_CONTROLLER_H

#include "GL\freeglut.h"

#include <iostream>
#include <string>
#include <map>

#include "DrawableObjectPresenter2.h"

#include "particleSysDlg.h"

class ListController
{
	static void applyChangesForMultList(HWND hDlg, HWND list,
		DrawableObjectsPresenter2 *DOPresenter,
		std::map<std::string, std::map<int, int>> &currentObjs);

	static void applyChangesForSingleList(HWND hDlg, HWND list,
		DrawableObjectsPresenter2 *DOPresenter,
		std::map<std::string, std::map<int, int>> &currentObjs);
public:

	static void addObjToCurObjs(HWND hDlg, HWND list,
		DrawableObjectsPresenter2* DOPresenter,
		std::map<std::string, std::map<int, int>> &currentObjs, int id);

	static void addObjToCurObjs(std::map<std::string, std::map<int, int>> &currentObjs,
		const char * objName, int intNum, int objId);

	static void checkEqualityBetweenCurObjsAndList(HWND hDlg, HWND list, 
		DrawableObjectsPresenter2 *DOPresenter,
		std::map<std::string, std::map<int, int>> &currentObjs,
		int checkedBoxIsMultiple);
};

#endif LIST_CONTROLLER_H