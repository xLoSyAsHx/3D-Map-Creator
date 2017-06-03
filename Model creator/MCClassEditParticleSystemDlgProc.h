#ifndef MC_CLASS_EDIT_PARTICLE_SYSTEM_DLG_PROC_H
#define MC_CLASS_EDIT_PARTICLE_SYSTEM_DLG_PROC_H

#include <iostream>
#include <string>
#include <Windows.h>

#include "DlgHelper.h"

#include "structs.h"

#include "addItemsDlg.h"
#include "editParticleSysDlg.h"

#include "MCClassItemsAddDlgProc.h"

class MCClassEditParticleSystemDlgProc
{
	static HINSTANCE hInstance;

	static AddVars *addVars;

	static bool *alreadyPressedP;
	static ParticleInfo *particleInfo;
	static std::string *particleName;

	static std::map<std::string, DrawableObject2*> *drawableObjsMap;
	static std::map<std::string, HWND> *elementsMap;
public:
	static INT_PTR CALLBACK EditParticleSysDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static void setNecessaryData(HINSTANCE hInstance,
		AddVars *addVars, bool *alreadyPressedP, ParticleInfo *particleInfo,
		std::string *particleName, std::map<std::string, DrawableObject2*> *drawableObjsMap,
		std::map<std::string, HWND> *elementsMap);
};

#endif EDIT_PARTICLE_SYSTEM_DLG_PROC_H