#ifndef MODEL_CREATOR_CLASS_ITEM_EDIT_DLG_PROC_H
#define MODEL_CREATOR_CLASS_ITEM_EDIT_DLG_PROC_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>

#include "structs.h"
#include "DlgHelper.h"
#include "ProgramDefines.h"

#include "Vect3f.h"
#include "Logger.h"
#include "MoveBtnsController.h"
#include "DrawableObjectPresenter2.h"

#include "rotationDlg.h"

#include <strsafe.h>

class MCClassItemEditDlgProc
{
public:
	enum FuncsId{ E_initDlg, E_checkVertData, E_checkObjsData, E_applyRot, E_applyPos, E_applyScal, E_applyVpos,
		E_setDefaultRot, E_setDefaultPos, E_setDefaultScal, E_setDefaultVpos, E_switchVertPos,
		E_checkMoveVert, E_checkMulVert, E_handleMsgUpDownBnts };

	static void setNecessaryFuncs(std::vector<FuncsId> &vect);
	static void setNecessaryIdentificators(std::vector<std::string> &vect);
private:
	static std::set<FuncsId> funcsSet;
	static std::set<std::string> identificators;

	static bool check(FuncsId id);
	static bool checkId(std::string identificator);


	//Нужно взять из родителя
	static Logger *logger;
	static MoveBtnsController *moveBtnsControl;

	static AddVars *addVars;

	static bool *isMulVertexes;
	static bool *isMoveVertexes;
	static bool *alreadyPressedE;

public:
	static void setNecessaryDatas(Logger *logger, MoveBtnsController *moveBtnsControl,
		AddVars *addVars, bool *isMulVertexes, bool *isMoveVertexes, bool *alreadyPressedE);

private:
	//Нужно установить из родителя
	static std::map<std::string, DrawableObjectsPresenter2*> DOPresenterMAP;
	static std::map<std::string, std::map<std::string, std::map<int, int>>*> curObjsMap;
	static std::map<std::string, std::map<int, std::map<Vect3f*, vector3ub>>*> currentVertexesMap;

	static std::map<std::string, std::map<int, AreaForParticleSys>> *fromKoefsForParticleSys;
	static std::map<std::string, std::map<int, AreaForParticleSys>> *toKoefsForParticleSys;
	static std::map<
		DrawableObjectsPresenter2*,
		std::map<std::string, std::map<int, AreaForParticleSys>>> *mapKoefsForPartSys;

public:
	static void addPresenter(std::string name, DrawableObjectsPresenter2* presenter);

	static void addCurObjsMap(std::string name,
		std::map<std::string, std::map<int, int>>* currentObjs);

	static void addCurVertexesMap(std::string name, 
		std::map<int, std::map<Vect3f*, vector3ub>>* currentVerts);

	static void setKoefsForPartSys(
		std::map<std::string, std::map<int, AreaForParticleSys>> *_fromKoefsForParticleSys,
		std::map<std::string, std::map<int, AreaForParticleSys>> *_toKoefsForParticleSys,
		std::map<
			DrawableObjectsPresenter2*,
			std::map<std::string, std::map<int, AreaForParticleSys>>> *_mapKoefsForPartSys
	);

private:
	//Не нужно задавать
	static Vect3f pos, rot, vpos;
	static bool isOnlyOneObj;
	static bool isOnlyOneVert;

	static std::map<DrawableObjectsPresenter2*, std::map<int, Vect3f>> objsRotationsMap;
	static std::map<DrawableObjectsPresenter2*, std::map<int, Vect3f>> objsPositionsMap;
	static std::map<Vect3f*, Vect3f> vertsPositions;
	static std::map<DrawableObject2*, Vect3f> objsScales;


	static bool initDlg(HWND hDlg, UINT message);

	static bool checkVertData(HWND hDlg, UINT message);

	static bool checkObjsData(HWND hDlg, UINT message);

	static bool applyRot(HWND hDlg, UINT message, WPARAM wParam);

	static bool applyPos(HWND hDlg, UINT message, WPARAM wParam);

	static bool applyScal(HWND hDlg, UINT message, WPARAM wParam);

	static bool applyVpos(HWND hDlg, UINT message, WPARAM wParam);

	static bool setDefaultRot(HWND hDlg, UINT message, WPARAM wParam);

	static bool setDefaultPos(HWND hDlg, UINT message, WPARAM wParam);

	static bool setDefaultScal(HWND hDlg, UINT message, WPARAM wParam);

	static bool setDefaultVpos(HWND hDlg, UINT message, WPARAM wParam);

	static bool switchVertPos(HWND hDlg, UINT message, WPARAM wParam);

	static bool checkMulVert(HWND hDlg, UINT message, WPARAM wParam);

	static bool checkMoveVert(HWND hDlg, UINT message, WPARAM wParam);

	static bool handleMsgUpDownBnts(HWND hDlg, UINT message, WPARAM wParam);




	static void changeData(HWND hDlg, WPARAM wParam, 
		int editVal, int editStep, int btnDown, int idApply);
	static void setDefault(HWND hDlg, Vect3f &vect, 
		bool isOnlyOneSelected, int e1, int e2, int e3, int step);
	static void switchVertTrans(HWND hDlg, int val);

	static void updateCurObjsData(bool &isOnlyOneObj,
		std::vector<DrawableObjectsPresenter2*> &presentersMap,
		std::vector<std::map<std::string, std::map<int, int>>*> &currentObjsMap);

	static void updateCurVertData(bool & isOnlyOneVert,
		std::vector<DrawableObjectsPresenter2*>& presenters,
		std::vector<std::map<int, std::map<Vect3f*, vector3ub>>*> &currentVertexesMap);

	static void updateCurObjsScales(
		bool &isOnlyOneObj,
		std::vector<DrawableObjectsPresenter2*> &presentersMap,
		std::vector<std::map<std::string, std::map<int, int>>*> &currentObjsMap);

	
public:
	static INT_PTR CALLBACK ItemEditDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif MODEL_CREATOR_CLASS_ITEM_EDIT_DLG_PROC_H