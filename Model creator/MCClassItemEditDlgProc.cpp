#include "MCClassItemEditDlgProc.h"


std::set<MCClassItemEditDlgProc::FuncsId> MCClassItemEditDlgProc::funcsSet;
std::set<std::string> MCClassItemEditDlgProc::identificators;
Logger *MCClassItemEditDlgProc::logger;
MoveBtnsController *MCClassItemEditDlgProc::moveBtnsControl;

AddVars *MCClassItemEditDlgProc::addVars;

bool *MCClassItemEditDlgProc::isMulVertexes;
bool *MCClassItemEditDlgProc::isMoveVertexes;
bool *MCClassItemEditDlgProc::alreadyPressedE;

std::map<std::string, DrawableObjectsPresenter2*> MCClassItemEditDlgProc::DOPresenterMAP;
std::map<std::string, std::map<std::string, std::map<int, int>>*> MCClassItemEditDlgProc::curObjsMap;
std::map<std::string, std::map<int, std::map<Vect3f*, vector3ub>>*> MCClassItemEditDlgProc::currentVertexesMap;

std::map<std::string, std::map<int, AreaForParticleSys>> *MCClassItemEditDlgProc::fromKoefsForParticleSys;
std::map<std::string, std::map<int, AreaForParticleSys>> *MCClassItemEditDlgProc::toKoefsForParticleSys;
std::map<
	DrawableObjectsPresenter2*,
	std::map<std::string, std::map<int, AreaForParticleSys>>> *MCClassItemEditDlgProc::mapKoefsForPartSys;

Vect3f MCClassItemEditDlgProc::pos;
Vect3f MCClassItemEditDlgProc::rot;
Vect3f MCClassItemEditDlgProc::vpos;
bool MCClassItemEditDlgProc::isOnlyOneObj;
bool MCClassItemEditDlgProc::isOnlyOneVert;

std::map<DrawableObjectsPresenter2*, std::map<int, Vect3f>> MCClassItemEditDlgProc::objsRotationsMap;
std::map<DrawableObjectsPresenter2*, std::map<int, Vect3f>> MCClassItemEditDlgProc::objsPositionsMap;
std::map<Vect3f*, Vect3f> MCClassItemEditDlgProc::vertsPositions;
std::map<DrawableObject2*, Vect3f> MCClassItemEditDlgProc::objsScales;


void MCClassItemEditDlgProc::setNecessaryFuncs(std::vector<FuncsId>& vect)
{
	for (auto it = vect.begin(); it != vect.end(); ++it)
		funcsSet.insert(*it);
}

void MCClassItemEditDlgProc::setNecessaryIdentificators(std::vector<std::string>& vect)
{
	for (auto it = vect.begin(); it != vect.end(); ++it)
		identificators.insert(*it);

	if (checkId("ModelCreator")) {
		std::vector<FuncsId> funcId = { E_initDlg, E_checkObjsData, E_applyRot, E_applyPos, E_applyScal, E_applyVpos,
			E_setDefaultRot, E_setDefaultPos, E_setDefaultScal, E_setDefaultVpos, E_switchVertPos,
			E_checkMoveVert, E_checkMulVert, E_handleMsgUpDownBnts };

		for (auto it = funcId.begin(); it != funcId.end(); ++it)
			funcsSet.insert(*it);
	}

	if (checkId("ParticleSysCreator")) {
		std::vector<FuncsId> funcId = { E_initDlg, E_checkObjsData, E_checkVertData, E_applyRot, E_applyPos, E_applyScal,
			E_setDefaultRot, E_setDefaultPos, E_setDefaultScal,
			E_handleMsgUpDownBnts };

		for (auto it = funcId.begin(); it != funcId.end(); ++it)
			funcsSet.insert(*it);
	}
}

bool MCClassItemEditDlgProc::check(FuncsId id)
{
	if (funcsSet.find(id) != funcsSet.end())
		return true;
	return false;
}

bool MCClassItemEditDlgProc::checkId(std::string identificator)
{
	if (identificators.find(identificator) != identificators.end())
		return true;
	return false;
}

void MCClassItemEditDlgProc::setNecessaryDatas(Logger * _logger, 
	MoveBtnsController * _moveBtnsControl, AddVars *_addVars, 
	bool *_isMulVertexes, bool *_isMoveVertexes, bool *_alreadyPressedE)
{
	logger = _logger;
	moveBtnsControl = _moveBtnsControl;
	addVars = _addVars;
	isMulVertexes = _isMulVertexes;
	isMoveVertexes = _isMoveVertexes;
	alreadyPressedE = _alreadyPressedE;
}

void MCClassItemEditDlgProc::addPresenter(std::string name, DrawableObjectsPresenter2 * presenter)
{
	DOPresenterMAP[name] = presenter;
}

void MCClassItemEditDlgProc::addCurObjsMap(std::string name, std::map<std::string, std::map<int, int>> *currentObjs)
{
	curObjsMap[name] = currentObjs;
}

void MCClassItemEditDlgProc::addCurVertexesMap(std::string name, std::map<int, std::map<Vect3f*, vector3ub>>* currentVerts)
{
	currentVertexesMap[name] = currentVerts;
}

void MCClassItemEditDlgProc::setKoefsForPartSys(
	std::map<std::string, std::map<int, AreaForParticleSys>> *_fromKoefsForParticleSys,
	std::map<std::string, std::map<int, AreaForParticleSys>> *_toKoefsForParticleSys,
	std::map<
	DrawableObjectsPresenter2*,
	std::map<std::string, std::map<int, AreaForParticleSys>>> *_mapKoefsForPartSys)
{
	fromKoefsForParticleSys = _fromKoefsForParticleSys;
	toKoefsForParticleSys = _toKoefsForParticleSys;
	mapKoefsForPartSys = _mapKoefsForPartSys;
}

bool MCClassItemEditDlgProc::initDlg(HWND hDlg, UINT message)
{
	if (message != WM_INITDIALOG)
		return false;

	addVars->keys['E'] = FALSE;

	std::vector<std::map<std::string, std::map<int, int>>*> currentObjs;
	std::vector<DrawableObjectsPresenter2*> presenters;
	std::vector<std::map<int, std::map<Vect3f*, vector3ub>>*> currentVertexes;

	if (checkId("ModelCreator")) {
		switchVertTrans(hDlg, FALSE);

		currentObjs = { curObjsMap["global"] };
		presenters = { DOPresenterMAP["global"] };
		currentVertexes = { currentVertexesMap["global"] };

		setDefault(hDlg, vpos, isOnlyOneVert, IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z, IDC_EDIT_VPOS_STEP);
	}

	if (checkId("ParticleSysCreator")) {
		RECT rect;
		GetWindowRect(hDlg, &rect);
		SetWindowPos(hDlg, HWND_TOP, 0, 0, rect.right - rect.left, 230, SWP_SHOWWINDOW | SWP_NOMOVE);

		currentObjs = {
			curObjsMap["from"],
			curObjsMap["to"]
		};
		presenters = {
			DOPresenterMAP["from"],
			DOPresenterMAP["to"]
		};
	}

	updateCurObjsData(isOnlyOneObj, presenters, currentObjs);
	updateCurVertData(isOnlyOneVert, presenters, currentVertexes);
	updateCurObjsScales(isOnlyOneObj, presenters, currentObjs);

	setDefault(hDlg, pos, isOnlyOneObj, 
		IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, IDC_EDIT_POS_STEP);
	setDefault(hDlg, rot, isOnlyOneObj,
		IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, IDC_EDIT_ROT_STEP);

	if (isOnlyOneObj)
		setDefault(hDlg, objsScales.begin()->second, true,
			IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
	else
		setDefault(hDlg, Vect3f(1, 1, 1), true,
			IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);

	return true;
}

bool MCClassItemEditDlgProc::checkVertData(HWND hDlg, UINT message)
{
	if (message != PD::WM_CHECKVERTDATA)
		return false;

	std::vector<DrawableObjectsPresenter2*> presenters;
	std::vector<std::map<int, std::map<Vect3f*, vector3ub>>*> currentVertexes;

	presenters = { DOPresenterMAP["global"] };
	currentVertexes = { currentVertexesMap["global"] };

	updateCurVertData(isOnlyOneVert, presenters, currentVertexes);
	setDefault(hDlg, vpos, isOnlyOneVert, IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z, IDC_EDIT_VPOS_STEP);

	return true;
}

bool MCClassItemEditDlgProc::checkObjsData(HWND hDlg, UINT message)
{
	if (message != PD::WM_CHECHOBJDATA)
		return false;

	std::vector<std::map<std::string, std::map<int, int>>*> currentObjs;
	std::vector<DrawableObjectsPresenter2*> presenters;

	if (checkId("ModelCreator")) {
		currentObjs = { curObjsMap["global"] };
		presenters = { DOPresenterMAP["global"] };
	}

	if (checkId("ParticleSysCreator")) {
		currentObjs = {
			curObjsMap["from"],
			curObjsMap["to"]
		};
		presenters = {
			DOPresenterMAP["from"],
			DOPresenterMAP["to"]
		};
	}

	updateCurObjsData(isOnlyOneObj, presenters, currentObjs);
	updateCurObjsScales(isOnlyOneObj, presenters, currentObjs);

	
	setDefault(hDlg, pos, isOnlyOneObj,
		IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, IDC_EDIT_POS_STEP);
	setDefault(hDlg, rot, isOnlyOneObj,
		IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, IDC_EDIT_ROT_STEP);

	if (isOnlyOneObj)
		setDefault(hDlg, objsScales.begin()->second, true,
			IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
	else
		setDefault(hDlg, Vect3f(1, 1, 1), true,
			IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);

	return true;
}

bool MCClassItemEditDlgProc::applyRot(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_APPLY_ROT)
		return false;

	std::vector<std::map<std::string, std::map<int, int>>*> _currentObjsMap;
	std::vector<DrawableObjectsPresenter2*> _presentersMap;


	if (checkId("ModelCreator")) {
		_currentObjsMap = { curObjsMap["global"] };
		_presentersMap = { DOPresenterMAP["global"] };
	}

	if (checkId("ParticleSysCreator")) {
		_currentObjsMap = {
			curObjsMap["from"],
			curObjsMap["to"]
		};
		_presentersMap = {
			DOPresenterMAP["from"],
			DOPresenterMAP["to"]
		};
	}

	Vect3f rotVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z);
	std::vector<DrawableObject2*> *drObjVectForLog = new std::vector<DrawableObject2*>();

	auto currentObjs = _currentObjsMap.begin();
	auto presenter = _presentersMap.begin();
	for (int k = 0; k < _presentersMap.size(); ++k) {
		for (auto it_name = (*currentObjs)->begin(); it_name != (*currentObjs)->end(); ++it_name) {
			auto pairMap = &(**currentObjs)[it_name->first];
			for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {

				auto drObj = (*presenter)->getObj(it->second);
				
				if (isOnlyOneObj)
					drObj->setRotation(rotVect);
				else
					drObj->setRotation(objsPositionsMap[*presenter][it->second].sum(rotVect));

				drObjVectForLog->push_back(drObj);


				drObj->updateCoords();
				drObj->updateCube();
			}
		}
	}

	logger->log(drObjVectForLog, &rotVect, Logger::Rot_Objs);

	return true;
}

bool MCClassItemEditDlgProc::applyPos(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_APPLY_POS)
		return false;

	std::vector<std::map<std::string, std::map<int, int>>*> _currentObjsMap;
	std::vector<DrawableObjectsPresenter2*> _presentersMap;


	if (checkId("ModelCreator")) {
		_currentObjsMap = { curObjsMap["global"] };
		_presentersMap = { DOPresenterMAP["global"] };
	}

	if (checkId("ParticleSysCreator")) {
		_currentObjsMap = {
			curObjsMap["from"],
			curObjsMap["to"]
		};
		_presentersMap = {
			DOPresenterMAP["from"],
			DOPresenterMAP["to"]
		};
	}

	Vect3f posVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z);
	std::vector<DrawableObject2*> *vectForLog = new std::vector<DrawableObject2*>();
	Vect3f shiftForLog;
	bool isShiftDet = false;

	auto currentObjs = _currentObjsMap.begin();
	auto presenter = _presentersMap.begin();
	for (int k = 0; k < _presentersMap.size(); ++k) {
		for (auto it_name = (*currentObjs)->begin(); it_name != (*currentObjs)->end(); ++it_name) {
			auto pairMap = &(**currentObjs)[it_name->first];
			for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {

				auto drObj = (*presenter)->getObj(it->second);

				if (!isShiftDet) {
					isShiftDet = true;

					if (isOnlyOneObj)
						shiftForLog = posVect.minus(*drObj->getPosition());
					else
						shiftForLog = (objsPositionsMap[*presenter][it->second].sum(posVect)).minus(*drObj->getPosition());
				}

				if (isOnlyOneObj)
					drObj->setPosition(posVect);
				else
					drObj->setPosition(objsPositionsMap[*presenter][it->second].sum(posVect));

				drObj->updateCoords();
				drObj->updateCube();

				vectForLog->push_back(drObj);
			}
		}
	}

	logger->log(vectForLog, &shiftForLog, Logger::Move_Objs);

	return TRUE;
}

bool MCClassItemEditDlgProc::applyScal(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_APPLY_SCAL)
		return false;

	Vect3f newScalForLog = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z);
	Vect3f oldScal;


	std::vector<std::map<std::string, std::map<int, int>>*> _currentObjsMap;
	std::vector<DrawableObjectsPresenter2*> _presentersMap;


	if (checkId("ModelCreator")) {
		_currentObjsMap = { curObjsMap["global"] };
		_presentersMap = { DOPresenterMAP["global"] };
	}

	if (checkId("ParticleSysCreator")) {
		_currentObjsMap = {
			curObjsMap["from"],
			curObjsMap["to"]
		};
		_presentersMap = {
			DOPresenterMAP["from"],
			DOPresenterMAP["to"]
		};
	}

	auto cam = moveBtnsControl->getCamera();
	std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>
		*globMap = new std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>();

	bool isScalDet = false;

	auto currentObjs = _currentObjsMap.begin();
	auto presenter = _presentersMap.begin();
	for (int k = 0; k < _presentersMap.size(); ++k) {
		for (auto it_name = (*currentObjs)->begin(); it_name != (*currentObjs)->end(); ++it_name) {
			auto pairMap = &(**currentObjs)[it_name->first];
			for (auto it = pairMap->begin(); it != pairMap->end(); ++it)
			{
				auto drObj = (*presenter)->getObj(it->second);

				auto map = drObj->getAllGlobalCoord(cam);
				globMap->push_back({ drObj, map });


				if (!isScalDet) {
					isScalDet = true;

					if (isOnlyOneObj) {
						oldScal = drObj->getScal();
					}
					else {
						oldScal = drObj->getScal();
						oldScal.setX(oldScal.getX() / objsScales[drObj].getX());
						oldScal.setY(oldScal.getY() / objsScales[drObj].getY());
						oldScal.setZ(oldScal.getZ() / objsScales[drObj].getZ());
					}
				}
			}
		}
		currentObjs++;
		presenter++;
	}

	Vect3f left, right, up, down, toward, back;
	bool check = false;
	for (auto it_map = globMap->begin(); it_map != globMap->end(); ++it_map)
		for (auto it = it_map->second->begin(); it != it_map->second->end(); ++it) {
			if (check == false) {
				check = true;
				left = it->second;
				right = it->second;
				up = it->second;
				down = it->second;
				toward = it->second;
				back = it->second;
			}

			auto checkFunc = [](Vect3f &point, Vect3f &low, Vect3f &high, char key) {
				if (point.get(key) < low.get(key))
					low = point;
				if (point.get(key) > high.get(key))
					high = point;
			};


			checkFunc(it->second, left, right, 'x');
			checkFunc(it->second, down, up, 'y');
			checkFunc(it->second, back, toward, 'z');
		}


	auto func = [&cam](Vect3f vect) -> Vect3f {
		return vect.minus(Vect3f(cam.getX(), cam.getY(), cam.getZ()));
	};


	SelectedCube cube1(left, right, down, up, back, toward);
	SelectedCube cube2(func(left), func(right), func(down), func(up), func(back), func(toward));

	std::map<Vect3f*, Vect3f> *shifts = new std::map<Vect3f*, Vect3f>();

	Vect3f newScal = newScalForLog;
	{
		Vect3f baseScale = objsScales[globMap->begin()->first];
		Vect3f newScaleForObj = baseScale.mul(newScal);
		Vect3f curScaleForObj = globMap->begin()->first->getScal();

		newScal = newScaleForObj;
		newScal.setX(newScal.getX() / curScaleForObj.getX());
		newScal.setY(newScal.getY() / curScaleForObj.getY());
		newScal.setZ(newScal.getZ() / curScaleForObj.getZ());
	}

	for (auto it_map = globMap->begin(); it_map != globMap->end(); ++it_map) {
		it_map->first->setScal(it_map->first->getScal().mul(newScal));
		//
		GLfloat fromLBToVert = it_map->first->getPosition()->getX() - cube2.getLeft();//Left Board Of Cube (x)
		GLfloat fromDBToVert = it_map->first->getPosition()->getY() - cube2.getDown();//Down Board Of Cube (y)
		GLfloat fromBBToVert = it_map->first->getPosition()->getZ() - cube2.getBack();//Back Board Of Cube (z)

		Vect3f curPosInCube(fromLBToVert, fromDBToVert, fromBBToVert);

		Vect3f shift = curPosInCube.mul(newScal);
		shift = shift.minus(curPosInCube); // final shift

		auto vert = it_map->first;
		vert->setPosition(vert->getPosition()->sum(shift));
		(*shifts)[vert->getPosition()] = shift;

		//->setPosition(it_map->first->getPosition());
		for (auto it = it_map->second->begin(); it != it_map->second->end(); ++it) {
			GLfloat fromLBToVert = it->second.getX() - cube1.getLeft();//Left Board Of Cube (x)
			GLfloat fromDBToVert = it->second.getY() - cube1.getDown();//Down Board Of Cube (y)
			GLfloat fromBBToVert = it->second.getZ() - cube1.getBack();//Back Board Of Cube (z)

																	   //Vect3f curPosInCube(fromLBToVert, fromDBToVert, fromBBToVert);
			Vect3f curPosInCube(it->first->getX(), it->first->getY(), it->first->getZ());


			Vect3f shift = curPosInCube.mul(newScal);
			shift = shift.minus(curPosInCube); // final shift


			auto vert = it->first;
			//vert->set(vert->sum(shift).minus(*it_map->first->getPosition()));
			vert->set(vert->sum(shift));
			(*shifts)[vert] = shift;
		}
	}

	if (checkId("ParticleSysCreator"))
	{
		currentObjs = _currentObjsMap.begin();
		presenter = _presentersMap.begin();
		for (int k = 0; k < _presentersMap.size(); ++k) {
			for (auto it_name = (*currentObjs)->begin(); it_name != (*currentObjs)->end(); ++it_name) {
				auto pairMap = &(**currentObjs)[it_name->first];
				for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {

					auto area = &(*fromKoefsForParticleSys)[it_name->first][it->first];
					auto baseArea = &(*mapKoefsForPartSys)[*presenter][it_name->first][it->first];

					if (k != 0) {
						area = &(*toKoefsForParticleSys)[it_name->first][it->first];
						baseArea = &(*mapKoefsForPartSys)[*presenter][it_name->first][it->first];
					}
					/*
					area->a = baseArea->a * newScal.getX();
					area->b = baseArea->b * newScal.getY();
					area->c = baseArea->c * newScal.getZ();
					*/
					area->a *= newScal.getX();
					area->b *= newScal.getY();
					area->c *= newScal.getZ();


				}
			}
			currentObjs++;
			presenter++;
		}
	}

	for (auto it_map = globMap->begin(); it_map != globMap->end(); ++it_map) {
		auto drObj = it_map->first;

		drObj->updateCoords();
		drObj->updateCube();
	}

	logger->log(globMap, &MapOldNewScal(shifts, oldScal, newScalForLog), Logger::Scal_Objs);

	return true;
}

bool MCClassItemEditDlgProc::applyVpos(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_APPLY_VPOS)
		return false;

	DrawableObjectsPresenter2* presenters;

	presenters = DOPresenterMAP["global"];
	std::map<int, std::map<Vect3f*, vector3ub>>* currentVertexes = currentVertexesMap["global"];


	Vect3f vposVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z);;

	if (currentVertexes->size() == 1)
		if (currentVertexes->begin()->second.size() == 1) {
			currentVertexes->begin()->second.begin()->first->set(vposVect);
			auto vect = new std::vector<Vect3f*>();
			auto curPos = currentVertexes->begin()->second.begin()->first;

			vect->push_back(curPos);

			auto drObj = presenters->getObj(currentVertexes->begin()->first);
			drObj->updateCoords();
			drObj->updateCube();

			logger->log(vect, &(vertsPositions[curPos].sum(vposVect).minus(*curPos)), Logger::Move_Verts);
			return true;
		}

	std::vector<Vect3f*> *vect = new std::vector<Vect3f*>();
	Vect3f vposVectForLog;
	bool isFirst = true;
	for (auto it_id = currentVertexes->begin(); it_id != currentVertexes->end(); ++it_id) {
		for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it) {
			auto curPos = it->first;

			if (isFirst) {
				isFirst = false;
				vposVectForLog = vertsPositions[curPos].sum(vposVect).minus(*curPos);
			}

			vect->push_back(curPos);
			if (isOnlyOneVert)
				curPos->set(vposVect);
			else
				curPos->set(vertsPositions[curPos].sum(vposVect));
		}

		auto drObj = presenters->getObj(currentVertexes->begin()->first);
		drObj->updateCoords();
		drObj->updateCube();
	}
	//if (isOnlyOneVert)
	logger->log(vect, &vposVectForLog, Logger::Move_Verts);

	return true;
}

bool MCClassItemEditDlgProc::setDefaultRot(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_SET_DEFAULT_ROT)
		return false;

	setDefault(hDlg, rot, isOnlyOneObj,
		IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, IDC_EDIT_ROT_STEP);

	return true;
}

bool MCClassItemEditDlgProc::setDefaultPos(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_SET_DEFAULT_POS)
		return false;

	setDefault(hDlg, pos, isOnlyOneObj,
		IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, IDC_EDIT_POS_STEP);

	return true;
}

bool MCClassItemEditDlgProc::setDefaultScal(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_SET_DEFAULT_SCAL)
		return false;

	if (isOnlyOneObj)
		setDefault(hDlg, objsScales.begin()->second, true,
			IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
	else
		setDefault(hDlg, Vect3f(1, 1, 1), true,
			IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);

	return true;
}

bool MCClassItemEditDlgProc::setDefaultVpos(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_SET_DEFAULT_VPOS)
		return false;

	setDefault(hDlg, vpos, isOnlyOneVert,
		IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z, IDC_EDIT_VPOS_STEP);

	return true;
}

bool MCClassItemEditDlgProc::switchVertPos(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_SWITCHER_VERT_POS)
		return false;

	auto el = GetDlgItem(hDlg, ID_SWITCHER_VERT_POS);

	TCHAR text[20];

	SendMessage(el, WM_GETTEXT, (WPARAM)10, (LPARAM)text);

	if (std::string(text) == "Enable") {
		SendMessage(el, WM_SETTEXT, (WPARAM)10, (LPARAM)"Disable");
		switchVertTrans(hDlg, TRUE);
	}
	else {
		SendMessage(el, WM_SETTEXT, (WPARAM)10, (LPARAM)"Enable");
		switchVertTrans(hDlg, FALSE);
	}

	return true;
}

bool MCClassItemEditDlgProc::checkMulVert(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_SWITCHER_VERT_POS)
		return false;

	*isMulVertexes = IsDlgButtonChecked(hDlg, IDC_CHECK_MUL_VERT);

	return true;
}

bool MCClassItemEditDlgProc::checkMoveVert(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND || LOWORD(wParam) != ID_SWITCHER_VERT_POS)
		return false;

	*isMoveVertexes = IsDlgButtonChecked(hDlg, IDC_CHECK_MOVE_VERT);

	return true;
}

bool MCClassItemEditDlgProc::handleMsgUpDownBnts(HWND hDlg, UINT message, WPARAM wParam)
{
	if (message != WM_COMMAND)
		return false;

	switch (LOWORD(wParam))
	{
	case IDC_BUTTON_POS_UP_X:
	case IDC_BUTTON_POS_DOWN_X:
	case IDC_BUTTON_POS_UP_Y:
	case IDC_BUTTON_POS_DOWN_Y:
	case IDC_BUTTON_POS_UP_Z:
	case IDC_BUTTON_POS_DOWN_Z:
	{
		int e = LOWORD(wParam);

		if (e == IDC_BUTTON_POS_UP_X || e == IDC_BUTTON_POS_DOWN_X)
			changeData(hDlg, wParam, IDC_EDIT_POS_X, IDC_EDIT_POS_STEP, IDC_BUTTON_POS_DOWN_X, ID_APPLY_POS);
		if (e == IDC_BUTTON_POS_UP_Y || e == IDC_BUTTON_POS_DOWN_Y)
			changeData(hDlg, wParam, IDC_EDIT_POS_Y, IDC_EDIT_POS_STEP, IDC_BUTTON_POS_DOWN_Y, ID_APPLY_POS);
		if (e == IDC_BUTTON_POS_UP_Z || e == IDC_BUTTON_POS_DOWN_Z)
			changeData(hDlg, wParam, IDC_EDIT_POS_Z, IDC_EDIT_POS_STEP, IDC_BUTTON_POS_DOWN_Z, ID_APPLY_POS);

		return true;
	}

	case IDC_BUTTON_ROT_UP_X:
	case IDC_BUTTON_ROT_DOWN_X:
	case IDC_BUTTON_ROT_UP_Y:
	case IDC_BUTTON_ROT_DOWN_Y:
	case IDC_BUTTON_ROT_UP_Z:
	case IDC_BUTTON_ROT_DOWN_Z:
	{
		int e = LOWORD(wParam);

		if (e == IDC_BUTTON_ROT_UP_X || e == IDC_BUTTON_ROT_DOWN_X)
			changeData(hDlg, wParam, IDC_EDIT_ROT_X, IDC_EDIT_ROT_STEP, IDC_BUTTON_ROT_DOWN_X, ID_APPLY_ROT);
		if (e == IDC_BUTTON_ROT_UP_Y || e == IDC_BUTTON_ROT_DOWN_Y)
			changeData(hDlg, wParam, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_STEP, IDC_BUTTON_ROT_DOWN_Y, ID_APPLY_ROT);
		if (e == IDC_BUTTON_ROT_UP_Z || e == IDC_BUTTON_ROT_DOWN_Z)
			changeData(hDlg, wParam, IDC_EDIT_ROT_Z, IDC_EDIT_ROT_STEP, IDC_BUTTON_ROT_DOWN_Z, ID_APPLY_ROT);

		return true;
	}

	case IDC_BUTTON_VPOS_UP_X:
	case IDC_BUTTON_VPOS_DOWN_X:
	case IDC_BUTTON_VPOS_UP_Y:
	case IDC_BUTTON_VPOS_DOWN_Y:
	case IDC_BUTTON_VPOS_UP_Z:
	case IDC_BUTTON_VPOS_DOWN_Z:
	{
		int e = LOWORD(wParam);

		if (e == IDC_BUTTON_VPOS_UP_X || e == IDC_BUTTON_VPOS_DOWN_X)
			changeData(hDlg, wParam, IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_STEP, IDC_BUTTON_VPOS_DOWN_X, ID_APPLY_VPOS);
		if (e == IDC_BUTTON_VPOS_UP_Y || e == IDC_BUTTON_VPOS_DOWN_Y)
			changeData(hDlg, wParam, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_STEP, IDC_BUTTON_VPOS_DOWN_Y, ID_APPLY_VPOS);
		if (e == IDC_BUTTON_VPOS_UP_Z || e == IDC_BUTTON_VPOS_DOWN_Z)
			changeData(hDlg, wParam, IDC_EDIT_VPOS_Z, IDC_EDIT_VPOS_STEP, IDC_BUTTON_VPOS_DOWN_Z, ID_APPLY_VPOS);

		return true;
	}

	}

	return false;
}

void MCClassItemEditDlgProc::changeData(HWND hDlg, WPARAM wParam, 
	int editVal, int editStep, int btnDown, int idApply)
{
	GLfloat val = DlgHelper::getValFromEdit(hDlg, editVal);
	GLfloat step = DlgHelper::getValFromEdit(hDlg, editStep);
	if (LOWORD(wParam) == btnDown)
		step *= -1;

	val += step;
	DlgHelper::setValForEdit(hDlg, editVal, val);
	SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(idApply, 0), 0);
}

void MCClassItemEditDlgProc::setDefault(HWND hDlg, Vect3f & vect,
	bool isOnlyOneSelected, int e1, int e2, int e3, int step)
{
	if (isOnlyOneSelected) {
		SendDlgItemMessage(hDlg, e1, WM_SETTEXT, 0, (LPARAM)std::to_string(vect.get('x')).c_str());
		SendDlgItemMessage(hDlg, e2, WM_SETTEXT, 0, (LPARAM)std::to_string(vect.get('y')).c_str());
		SendDlgItemMessage(hDlg, e3, WM_SETTEXT, 0, (LPARAM)std::to_string(vect.get('z')).c_str());
	}
	else {
		SendDlgItemMessage(hDlg, e1, WM_SETTEXT, 0, (LPARAM)"0.0");
		SendDlgItemMessage(hDlg, e2, WM_SETTEXT, 0, (LPARAM)"0.0");
		SendDlgItemMessage(hDlg, e3, WM_SETTEXT, 0, (LPARAM)"0.0");
	}

	SendDlgItemMessage(hDlg, step, WM_SETTEXT, 0, (LPARAM)"1.0");
}

void MCClassItemEditDlgProc::switchVertTrans(HWND hDlg, int val)
{
	auto el = GetDlgItem(hDlg, IDC_EDIT_VPOS_X);
	ShowWindow(el, val);
	el = GetDlgItem(hDlg, IDC_EDIT_VPOS_Y);
	ShowWindow(el, val);
	el = GetDlgItem(hDlg, IDC_EDIT_VPOS_Z);
	ShowWindow(el, val);
	el = GetDlgItem(hDlg, ID_APPLY_VPOS);
	ShowWindow(el, val);
	el = GetDlgItem(hDlg, ID_SET_DEFAULT_VPOS);
	ShowWindow(el, val);
	el = GetDlgItem(hDlg, IDC_STATIC_VPOS_X);
	ShowWindow(el, val);
	el = GetDlgItem(hDlg, IDC_STATIC_VPOS_Y);
	ShowWindow(el, val);
	el = GetDlgItem(hDlg, IDC_STATIC_VPOS_Z);
	ShowWindow(el, val);

	RECT rect;
	GetWindowRect(hDlg, &rect);
	if (val == FALSE)
		SetWindowPos(hDlg, HWND_TOP, 0, 0, rect.right - rect.left, 250, SWP_SHOWWINDOW | SWP_NOMOVE);
	else
		SetWindowPos(hDlg, HWND_TOP, 0, 0, rect.right - rect.left, 325, SWP_SHOWWINDOW | SWP_NOMOVE);
}

void MCClassItemEditDlgProc::updateCurObjsData(
	bool &isOnlyOneObj,
	std::vector<DrawableObjectsPresenter2*> &presentersMap,
	std::vector<std::map<std::string, std::map<int, int>>*> &currentObjsMap)
{
	int i = 0;
	for (auto it = objsRotationsMap.begin(); it != objsRotationsMap.end(); ++it)
		it->second.clear();

	for (auto it = objsPositionsMap.begin(); it != objsPositionsMap.end(); ++it)
		it->second.clear();

	objsRotationsMap.clear();
	objsPositionsMap.clear();

	auto currentObjs = currentObjsMap.begin();
	auto presenter = presentersMap.begin();
	for (int k = 0; k < presentersMap.size(); ++k) {
		for (auto it_name = (*currentObjs)->begin(); it_name != (*currentObjs)->end(); ++it_name) {
			auto pairMap = &(**currentObjs)[it_name->first];
			for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {
				i++;

				auto drObj = (*presenter)->getObj(it->second);

				objsRotationsMap[*presenter][it->second] = drObj->getRotation();
				objsPositionsMap[*presenter][it->second] = *drObj->getPosition();
			}
		}
		currentObjs++;
		presenter++;
	}

	if (i == 1) {
		pos = objsPositionsMap[*presentersMap.begin()].begin()->second;
		rot = objsRotationsMap[*presentersMap.begin()].begin()->second;
		isOnlyOneObj = true;
	}
	else
		isOnlyOneObj = false;
}

void MCClassItemEditDlgProc::updateCurVertData(bool & isOnlyOneVert, 
	std::vector<DrawableObjectsPresenter2*>& presenters, 
	std::vector<std::map<int, std::map<Vect3f*, vector3ub>>*> &currentVertexesMap)
{
	auto currentVertexes = currentVertexesMap.begin();

	int i = 0;
	for (int k = 0; k < currentVertexesMap.size(); ++k) {
		for (auto it_id = (*currentVertexes)->begin(); it_id != (*currentVertexes)->end(); ++it_id)
			for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it) {
				i++;
				vertsPositions[it->first] = *(it->first);
			}
		currentVertexes++;
	}

	if (i == 1) {
		vpos = *((*currentVertexesMap.begin())->begin()->second.begin()->first);
		isOnlyOneVert = true;
	}
	else
		isOnlyOneVert = false;
}

void MCClassItemEditDlgProc::updateCurObjsScales(
	bool &isOnlyOneObj,
	std::vector<DrawableObjectsPresenter2*> &presentersMap,
	std::vector<std::map<std::string, std::map<int, int>>*> &currentObjsMap)
{
	objsScales.clear();
	auto currentObjs = currentObjsMap.begin();
	auto presenter = presentersMap.begin();

	for (int k = 0; k < presentersMap.size(); ++k)
		for (auto it_id = (*currentObjs)->begin(); it_id != (*currentObjs)->end(); ++it_id)
			for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it)
			{
				auto drObj = (*presenter)->getObj(it->second);
				
				objsScales[drObj] = drObj->getScal();
			}

	if (objsScales.size() == 1)
		isOnlyOneObj = true;
	else
		isOnlyOneObj = false;
}

INT_PTR MCClassItemEditDlgProc::ItemEditDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (check(FuncsId::E_initDlg) &&
		initDlg(hDlg, message) != false)
		return TRUE;

	if (check(FuncsId::E_checkVertData) &&
		checkVertData(hDlg, message) != false)
		return TRUE;

	if (check(FuncsId::E_checkObjsData) &&
		checkObjsData(hDlg, message) != false)
		return TRUE;

	if (check(FuncsId::E_applyRot) &&
		applyRot(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_applyPos) &&
		applyPos(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_applyScal) &&
		applyScal(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_applyVpos) &&
		applyVpos(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_setDefaultPos) &&
		setDefaultPos(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_setDefaultRot) &&
		setDefaultRot(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_setDefaultScal) &&
		setDefaultScal(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_setDefaultVpos) &&
		setDefaultVpos(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_switchVertPos) &&
		switchVertPos(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_checkMoveVert) &&
		checkMoveVert(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_checkMulVert) &&
		checkMulVert(hDlg, message, wParam) != false)
		return TRUE;

	if (check(FuncsId::E_handleMsgUpDownBnts) &&
		handleMsgUpDownBnts(hDlg, message, wParam) != false)
		return TRUE;

	if (message == WM_COMMAND && LOWORD(wParam) == IDCANCEL) {
		int asd = EndDialog(hDlg, LOWORD(wParam));

		*alreadyPressedE = false;
		return TRUE;
	}

	return FALSE;
	//return DefWindowProc(hDlg, message, wParam, lParam);
}
