#ifndef WIN_3D_MAP_REDACTOR_H
#define WIN_3D_MAP_REDACTOR_H

#include "GL\freeglut.h"

#include <commdlg.h>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include <thread>

#include "DlgHelper.h"
#include "WindowHelper.h"
#include "GraphicHelper.h"
#include "RoundObjHelper.h"
#include "Inits.h"
#include "helper.h"
#include "helper2.h"
#include "ProgramDefines.h"
#include "ID.h"

#include "DrawableObject2.h"
#include "DrawableObjectPresenter2.h"
#include "ParticleSystemPresenter.h"
#include "MoveBtnsController.h"
#include "gloabalVars.h"
#include "structs.h"

#include "Logger.h"

#include "editParticleSysDlg.h"
#include "ParticleSystemsDlg.h"
#include "addItemsDlg.h"

#include "MCClassItemEditDlgProc.h"
#include "MCClassEditParticleSystemDlgProc.h"

#include "WinCreateParticleSystem.h"
#include "WinCreateModel.h"


class Win3DMapRedactor {
	static HWND		 MapCreator_hWnd;
	static HDC		 MapCreator_hDC;
	static HGLRC	 MapCreator_hRC;
	static HINSTANCE MapCreator_hInstance;

	static Logger logger;
	static DrawableObjectsPresenter2* DOPresenter;// ñòàòè÷åñêèå îáúåêòû
	static ParticleSystemPresenter* PSPresenter;
	static MoveBtnsController moveBtnsControl;
	static ParticleSystem* currentPartSys;
	static std::map<std::string, DrawableObject2*> *drawableObjsMap;
	static std::map<std::string, HWND> elementsMap;

	BOOL blend, light, lp, bp, pp, zp, yp, canSelVert = true, canSelObj = true, canLinkObjsThrowVert = true;
	GLuint base;

	static bool haveSmthgNew;
	static std::string newObjName;
	static AddVars addVars;
	static std::string particleName;
	static std::map<std::string, GLuint> modelsMap;

	static std::map<std::string, ParticleSystem*> particleSysMap;
	static std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> ñolorMapForObjects;

	void Init();

	static std::vector<std::string> getParticleSysKeys(std::map<std::string, ParticleSystem*> map);
	static void loadMap();
	static void saveMap();

	static bool alreadyPressedE;
	static bool alreadyPressedP;
public:
	static LRESULT CALLBACK MapCreatorWndProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM	lParam);

	static INT_PTR CALLBACK ParticleSysAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK ParticleSystemsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	Win3DMapRedactor();

	int DrawGLScene();
};


#endif WIN_3D_MAP_REDACTOR_H
