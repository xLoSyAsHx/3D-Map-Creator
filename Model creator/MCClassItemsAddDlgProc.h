#ifndef MC_CLASS_ITEMS_ADD_DLG_PROC_H
#define MC_CLASS_ITEMS_ADD_DLG_PROC_H

#include <iostream>
#include <string>
#include <Windows.h>

#include "DlgHelper.h"
#include "helper.h"

#include "DrawableObjectPresenter2.h"
#include "structs.h"

#include "addItemsDlg.h"

class MCClassItemsAddDlgProc
{
	static ParticleInfo *particleInfo;
	static std::string *particleName;
	static std::map<std::string, DrawableObject2*> *drawableObjsMap;
	static std::map<std::string, HWND> *elementsMap;

	static std::vector<std::string> getDrObjKeys(std::map<std::string, DrawableObject2*> map);
public:

	static INT_PTR ItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static void setNecessaryDatas(ParticleInfo *_particleInfo, std::string *_particleName,
		std::map<std::string, DrawableObject2*> *_drawableObjsMap,
		std::map<std::string, HWND> *_elementsMap);
};

#endif MC_CLASS_ITEMS_ADD_DLG_PROC_H