#ifndef WIN_CREATE_PARTICLE_SYSTEM_H
#define WIN_CREATE_PARTICLE_SYSTEM_H

#define ID_EDIT_SET_PROBABIL	11001

#include "GL\freeglut.h"

#include <commdlg.h>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include <io.h>
#include <thread>

#include "ListController.h"
#include "DlgHelper.h"
#include "WindowHelper.h"
#include "GraphicHelper.h"
#include "RoundObjHelper.h"
#include "Inits.h"
#include "helper.h"
#include "helper2.h"
#include "ProgramDefines.h"
#include "Saver.h"

#include "DrawableObject2.h"
#include "DrawableObjectPresenter2.h"
#include "MoveBtnsController.h"
#include "gloabalVars.h"
#include "structs.h"
#include "ParticleSystem.h"

#include "Logger.h"


#include "AddRoundObjsDlg.h"
#include "particleSysDlg.h"
#include "setProbabilityDlg.h"
#include "editParticleSysDlg.h"
#include "addItemsDlg.h"

#include "MCClassItemEditDlgProc.h"

#include "Loader.h"
#include "Saver.h"


class WinCreateParticleSystem
{
	static HINSTANCE ParticleSysCreator_hInstance;
	static HWND ParticleSysCreator_hWnd;
	static HDC ParticleSysCreator_hDC;

	static MoveBtnsController *moveBtnsControl;
	static Logger logger;
	static ParticleInfo *particleInfo;

	static DrawableObjectsPresenter2* ToDOPresenter;
	static DrawableObjectsPresenter2* FromDOPresenter;
	static ParticleSystem* particleSystem;

	static std::map<std::string, DrawableObject2*> drawableObjsMap;
	static std::map<std::string, GLuint> modelsMap;
	static std::map<std::string, HWND> elementsMap;
	static std::map<std::string, std::map<int, int>> fromCurrentObjs;
	static std::map<std::string, std::map<int, int>> toCurrentObjs;
	static std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> ñolorMapForObjects; // id - color
	static std::map<std::string, std::map<int, AreaForParticleSys>> *fromKoefsForParticleSys;
	static std::map<std::string, std::map<int, AreaForParticleSys>> *toKoefsForParticleSys;
	static std::map<
		DrawableObjectsPresenter2*, 
		std::map<std::string, std::map<int, AreaForParticleSys>>> mapKoefsForPartSys;

	static std::vector<std::string> getDrObjKeys(std::map<std::string, DrawableObject2*> map);
	static bool isThisObjSelect(std::map<std::string, std::map<int, int>> &currentObjs, 
		NameIndexId nii);
	static bool canCreateParticleSystem(ParticleInfo &particleInfo);

	static void saveModel();

	static void loadModel();

	static AddVars addVars;

	static bool haveSmthgNew;
	static std::string newObjName;
	static bool isAddToFrom;
	static bool alreadyPressedE;
	static bool alreadyPressedP;
	static GLfloat probability;
	static std::string particleName;
	static bool isEditFromMapCreator;

	bool canSelObj = true;
	bool canSelVert = true;
	bool zp = false;
	bool yp = false;
	bool light;
	bool lp;
	bool pp;
	GLuint base;
	DrawableObjectsPresenter2 *mapDOPresenter;
	static ParticleSystem** particleSystemFromMapCreator;
	//static DrawableObjectsPresenter2 *

	void Init();

	static void clearAllParticleSystemCreator();

public:
	static LRESULT CALLBACK ParticleSysCreatorWndProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM	lParam);

	static INT_PTR CALLBACK ItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK RoundItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK ItemRotationDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK EditParticleSysDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK SetProbabilityDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK ParticleSysDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	
	WinCreateParticleSystem(HWND &main_hWnd, ParticleSystem **partSys = nullptr, 
		DrawableObjectsPresenter2 *mapDOPresenter = nullptr, std::string partName = "");

	int DrawGLScene();

	~WinCreateParticleSystem();
};

#endif WIN_CREATE_PARTICLE_SYSTEM_H