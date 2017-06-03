#ifndef WIN_CREATE_MODEL_H
#define WIN_CREATE_MODEL_H

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
#include "Saver.h"

#include "DrawableObject2.h"
#include "DrawableObjectPresenter2.h"
#include "MoveBtnsController.h"
#include "gloabalVars.h"
#include "structs.h"

#include "Logger.h"

#include "resource.h"
#include "rotationDlg.h"
#include "addItemsDlg.h"
#include "mapTexDlg.h"
#include "AddRoundObjsDlg.h"
#include "ParticleSystemsDlg.h"

#include "MCClassItemEditDlgProc.h"



#include "WinCreateParticleSystem.h"

class WinCreateModel {
	static DrawableObjectsPresenter2* DOPresenter;// = new DrawableObjectsPresenter2();
	static MoveBtnsController moveBtnsControl;
	static std::map<std::string, std::map<int, int>> currentObjs;
	static std::map<std::string, DrawableObject2*> drawableObjsMap;
	static std::map<std::string, GLuint> modelsMap;
	static std::map<int, vector3ub> ñolorMapForObjects; // id - color
	static std::map<int, std::map<Vect3f*, vector3ub>> colorMapForVertexes;
	static std::map<int, std::map<Vect3f*, vector3ub>> currentVertexes;
	static std::map<int, std::set<OpenGL_Base*>> OGLB_ForMappedTexture;// <drawableObjId, <OpenGL_Base_IndexForVert, <color, vertex>>>
	static std::pair<std::string, GLuint*> pairNameTexForObjs;
	static Logger logger;

	BOOL blend, light, lp, bp, pp, zp, yp, canSelVert = true, canSelObj = true, canLinkObjsThrowVert = true;
	GLuint base;

	std::pair<int, Vect3f*> vertForLink = { -1, nullptr };
	static bool isMulVertexes;
	static bool isMoveVertexes;
	static bool isOnlyForOGJB;
	static bool canMapTexture;

	static COLORREF color;

	static bool haveSmthgNew;
	static std::string newObjName;
	static AddVars addVars;
	static HINSTANCE ModelCreator_hInstance;
	static HWND ModelCreator_hWnd;
	static HDC ModelCreator_hDC;
	static std::map<std::string, HWND> elementsMap;

	void Init();

	static void saveModel();

	static void saveAsAMap();

	static void addObjToCurObjs(const char* objName, int intNum, int objId);
	static void addObjToCurObjs(HWND hDlg, int id);
	static void addVertToCurVerts(int id, Vect3f* ptr);

	static bool isThisVertSelect(std::pair<int, Vect3f*> &pair);
	static bool isThisObjSelect(NameIndexId nii);

	static void applyChangesForMultList(HWND hDlg, HWND list);
	static void applyChangesForSingleList(HWND hDlg, HWND list);
	static void checkEqualityBetweenCurObjsAndList(HWND hDlg, HWND list);
	static void clearCurrentObjs();
	static void clearCurrentVertexes();
	static std::vector<std::string> getDrObjKeys(std::map<std::string, DrawableObject2*> map);

	Vect3f getDifBetweenVerts(std::pair<int, Vect3f*> &staticVert, std::pair<int, Vect3f*> &connectedVert);

	static bool alreadyPressedE;
	static bool isVertexesEnable;
	static bool isDrawModeOn;
	static bool isMapTextureModeOn;
	static bool isOnlyOneVertex;

	static void disableTextureMode();
	static void updateEditDlgForLog();
public:
	static LRESULT CALLBACK ModelCreatorWndProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM	lParam);

	static INT_PTR CALLBACK ItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK RoundItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK ItemRotationDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static INT_PTR CALLBACK MappedTextureDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK ItemsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	WinCreateModel(HWND &main_hWnd);

	int DrawGLScene();
};


#endif WIN_CREATE_MODEL_H
