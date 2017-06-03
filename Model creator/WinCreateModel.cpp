#include "WinCreateModel.h"

using namespace std::chrono_literals;

AddVars WinCreateModel::addVars;
std::string WinCreateModel::newObjName = "";
bool WinCreateModel::haveSmthgNew = false;
std::map<std::string, std::map<int, int>> WinCreateModel::currentObjs;
HINSTANCE WinCreateModel::ModelCreator_hInstance = NULL;
HWND WinCreateModel::ModelCreator_hWnd = NULL;
HDC WinCreateModel::ModelCreator_hDC = NULL;
std::map<std::string, HWND> WinCreateModel::elementsMap;
std::map<std::string, GLuint> WinCreateModel::modelsMap;
std::map<std::string, DrawableObject2*> WinCreateModel::drawableObjsMap;
MoveBtnsController WinCreateModel::moveBtnsControl;
std::map<int, vector3ub> WinCreateModel::сolorMapForObjects;
std::map<int, std::map<Vect3f*, vector3ub>> WinCreateModel::colorMapForVertexes;
std::map<int, std::map<Vect3f*, vector3ub>> WinCreateModel::currentVertexes;
std::map<int, std::set<OpenGL_Base*>> WinCreateModel::OGLB_ForMappedTexture;
std::pair<std::string, GLuint*> WinCreateModel::pairNameTexForObjs;
DrawableObjectsPresenter2* WinCreateModel::DOPresenter = new DrawableObjectsPresenter2();
Logger WinCreateModel::logger;

bool WinCreateModel::isMulVertexes = false;
bool WinCreateModel::isMoveVertexes = false;
bool WinCreateModel::isOnlyForOGJB = false;
bool WinCreateModel::isDrawModeOn = false;
bool WinCreateModel::isMapTextureModeOn = false;
bool WinCreateModel::isOnlyOneVertex = false;
bool WinCreateModel::canMapTexture = true;
COLORREF WinCreateModel::color = RGB(255, 255, 255);

bool WinCreateModel::alreadyPressedE = false;
bool WinCreateModel::isVertexesEnable = false;

int WinCreateModel::DrawGLScene()
{
	static std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now();
	static std::chrono::time_point<std::chrono::system_clock> timeForCheckFPS = std::chrono::system_clock::now();
	static GLfloat fps = 0;
	auto end = std::chrono::system_clock::now();
	auto res = std::chrono::duration_cast<std::chrono::microseconds> (end - time).count();

	if (res < 16000)
		std::this_thread::sleep_for(std::chrono::microseconds(16000 - res));

	int res2 = std::chrono::duration_cast<std::chrono::milliseconds> (end - timeForCheckFPS).count();
	if (res2 >= 1000) {
		fps = 1000000.0f / res;
		timeForCheckFPS = std::chrono::system_clock::now();
	}
		//Sleep(16 - res);


	time = std::chrono::system_clock::now();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();

	std::vector<Vect3f> vect;
	vect.push_back(Vect3f(-1, -3, 0));
	//vect.push_back(Vect3f(-1, -1.5, -1.5));
	vect.push_back(Vect3f(-1, 0, -3));
	//vect.push_back(Vect3f(-1, 1.5, 1.5));
	vect.push_back(Vect3f(-1, 3, 0));
	vect.push_back(Vect3f(-1, 0, 3));
	static std::vector<OpenGL_Base*> * asd = genOGLBVectByPointVect(vect, -1, 0, 1, 1, 1, 5);

	
	
	

	
	const CameraLocation& cam = moveBtnsControl.getCamera();
	glRotatef(cam.getYAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(cam.getXAngle(), 0.0f, 1.0f, 0.0f);

	glTranslatef(cam.getX(), cam.getY(), cam.getZ());      // Перенос В/Вне экрана по z
														   //glOrtho(-200, 200, -200, 200, -200, 200);
	glColor3ub(0, 130, 0);
	glCallList(modelsMap["grid"]);

	glPushMatrix();

	DOPresenter->show(cam, currentObjs, isVertexesEnable, currentVertexes, vertForLink, OGLB_ForMappedTexture);

	glPopMatrix();

	//
	glPushMatrix();

	Vect3f v1(-1, 0, -5);
	Vect3f v2(1, 0, -5);
	Vect3f v3(0, 1, -5);

	//v1 = v1.minus(Vect3f(cam.getX(), cam.getY(), cam.getZ()));
	//v2 = v2.minus(Vect3f(cam.getX(), cam.getY(), cam.getZ()));
	//v3 = v3.minus(Vect3f(cam.getX(), cam.getY(), cam.getZ()));

	/*
	glBegin(4);

	glVertex3f(v1);
	glVertex3f(v2);
	glVertex3f(v3);

	glEnd();
	*/
	//glTranslatef(0, 0, -10);
	/*
	glBegin(4);

	glColor3ub(255, 0, 0);

	glVertex3f(-1,0,0);
	glVertex3f(1,0,0);
	glVertex3f(0,1,0);

	glEnd();
	*/

	///часть элипсоида
	//for (auto it = asd->begin(); it != asd->end(); ++it)
		//(*it)->draw(false, std::map<Vect3f*, vector3ub>(), std::pair<int, Vect3f*>(-1, nullptr), false);

	///диск
	//static DrawableObject2* obj = genDisc(Vect3f(0, 0, -5), 4, 1, 1, 100);
	//obj->draw(cam, false, std::map<Vect3f*, vector3ub>(), std::pair<int, Vect3f*>(-1, nullptr), std::set<OpenGL_Base*>());

	///конус
	//static DrawableObject2* obj = genCone(Vect3f(0, 0, -5), 4, Vect3f(0, 3, -5), 3, 1, 1, 100);
	//obj->draw(cam, false, std::map<Vect3f*, vector3ub>(), std::pair<int, Vect3f*>(-1, nullptr), std::set<OpenGL_Base*>());

	///цилиндр
	//static DrawableObject2* obj = genCylinder(Vect3f(0, 0, -5), 4, Vect3f(0, 3, -5), 1, 1, 100);
	//obj->draw(cam, false, std::map<Vect3f*, vector3ub>(), std::pair<int, Vect3f*>(-1, nullptr), std::set<OpenGL_Base*>());

	///сфера
	//static DrawableObject2* obj = genEllipsoid(Vect3f(0,0,0), 1, 1, 1, 1, 10);
	//obj->draw(cam, false, std::map<Vect3f*, vector3ub>(), std::pair<int, Vect3f*>(-1, nullptr), std::set<OpenGL_Base*>());

	glPopMatrix();

	glLoadIdentity();
	glColor3ub(0, 200, 200);

	glTranslatef(0,0, -1.0f);
	glRasterPos2f(0.38, -0.38);
	
	glPrint(base, "%3.2f", fps);

	//glRotatef(cam.getYAngle(), 1.0f, 0.0f, 0.0f);
	//glRotatef(cam.getXAngle(), 0.0f, 1.0f, 0.0f);
	/*
	glBegin(GL_TRIANGLES);

	glVertex3f(0, 0, -1);
	glVertex3f(2, 0, -1);
	glVertex3f(1, 1, -1);

	glEnd();
	*/
	//

	return TRUE;										// Keep Going
}

void WinCreateModel::Init()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	std::string fileName;
	fileName.resize(100);

	std::fstream fout("C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Model creator\\test.txt", std::ios_base::app);
	fout.close();

	hf = FindFirstFile("C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Model creator\\Data\\Models\\\*", &FindFileData);

	

	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			int addIndex = 0;
			for (int i = 0; i < strlen(FindFileData.cFileName); ++i)
				if (FindFileData.cFileName[i] == '\\')
					addIndex = i;
			for (int i = 0; i < strlen(FindFileData.cFileName) - addIndex; ++i)
				fileName[i] = FindFileData.cFileName[addIndex + i];
			//cout << FindFileData.cFileName << endl;
			char* index = strchr(FindFileData.cFileName, '.');
			if (index != NULL)
			{
				if (strlen(index) == 4)
				{
					if (strcmp(index, ".3dr") == 0) {//  index == ".3dr") {
						auto name = "Data/Models/" + std::string(FindFileData.cFileName);
						try {
							auto objName = DrawableObjectsPresenter2::loadFromFile(name, drawableObjsMap);
							drawableObjsMap[objName]->setPosition(Vect3f(0, 0, 0));
							drawableObjsMap[objName]->setRotation(Vect3f(0, 0, 0));
						}
						catch (...) {
							MessageBox(ModelCreator_hWnd, "File was damaged", (std::string("Can't load object ") + fileName).c_str(), MB_OK);
						}
					}
				}
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}

	CreateModelBuildLists(modelsMap);

	BuildFont(ModelCreator_hDC, base);
}

void WinCreateModel::saveModel()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));
	
	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = ModelCreator_hWnd;
	ofn.lpstrFilter = "Model Creator Files (*.3dr)\0*.3dr\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//ofn.lpstrDefExt = "3dr";
	ofn.lpstrInitialDir = "C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Model creator\\Data\\Models";

	if (GetSaveFileName(&ofn))
	{
		auto saveToFile = [](OPENFILENAME &ofn, DrawableObjectsPresenter2 * DOPresenter) {
			//std::fstream fout(ofn.lpstrFile, std::ios_base::trunc);

			std::vector<OpenGL_Base*>* vectOGLB = new std::vector<OpenGL_Base*>;
			std::vector<Vect3f*>* vectVertPos = new std::vector<Vect3f*>;
			std::vector<Vect3f*>* vectVertRot = new std::vector<Vect3f*>;
			for (int i = 0; i < DOPresenter->getobjMapSize(); ++i){

				auto OGLB = DOPresenter->getObj(i)->getVertexes();
				for (auto it = OGLB->begin(); it != OGLB->end(); ++it) 
					vectOGLB->push_back(*it);

				auto vertPos = DOPresenter->getObj(i)->getVertPosition();
				for (auto it = vertPos->begin(); it != vertPos->end(); ++it)
					vectVertPos->push_back(new Vect3f((*it)->sum(*DOPresenter->getObj(i)->getPosition())));

				auto vertRot = DOPresenter->getObj(i)->getVertRotation();
				for (auto it = vertRot->begin(); it != vertRot->end(); ++it)
					vectVertRot->push_back(*it);
			}

			DrawableObject2* newDrObjForSave = new 
				DrawableObject2(vectOGLB, vectVertPos, vectVertRot, Vect3f(0,0,0), Vect3f(0,0,0));

			DOPresenter->saveToFile(ofn.lpstrFile, newDrObjForSave);

			delete newDrObjForSave;

			//fout.close();
		};

		std::ifstream isFileExist(ofn.lpstrFile, std::ios_base::in | std::ios_base::_Nocreate);
		if (isFileExist.is_open())
		{
			isFileExist.close();
			if (MessageBox(ModelCreator_hWnd, "Model with such name already exist. Do you want to overwrite it?", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				//WindowHelper::KillGLWindow(ModelCreator_hWnd, ModelCreator_hInstance, ModelCreator_hDC, ModelCreator_hRC, "ModelCreator");
				//DestroyWindow(elementsMap["commonObjList"]);
				//EnableWindow(main_hWnd, TRUE);

				saveToFile(ofn, DOPresenter);
			}
		}
		else
		{
			std::fstream f(ofn.lpstrFile);
			if (f.is_open())
				f.close();

			saveToFile(ofn, DOPresenter);
		}
		// Do something usefull with the filename stored in szFileName 
	}
}

void WinCreateModel::saveAsAMap()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = ModelCreator_hWnd;
	ofn.lpstrFilter = "Model Creator Files (*.3dm)\0*.3dm\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//ofn.lpstrDefExt = "3dr";
	ofn.lpstrInitialDir = "C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Model creator\\Data\\Models";

	if (GetSaveFileName(&ofn))
	{
		std::ifstream isFileExist(ofn.lpstrFile, std::ios_base::in | std::ios_base::_Nocreate);
		if (isFileExist.is_open())
		{
			isFileExist.close();
			if (MessageBox(ModelCreator_hWnd, "Model with such name already exist. Do you want to overwrite it?", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				//WindowHelper::KillGLWindow(ModelCreator_hWnd, ModelCreator_hInstance, ModelCreator_hDC, ModelCreator_hRC, "ModelCreator");
				//DestroyWindow(elementsMap["commonObjList"]);
				//EnableWindow(main_hWnd, TRUE);
				ParticleSystemPresenter *PSPresenter = new ParticleSystemPresenter();

				Saver::saveMap(ofn.lpstrFile, DOPresenter, PSPresenter, &moveBtnsControl);

				delete PSPresenter;
			}
		}
		else
		{
			std::fstream f(ofn.lpstrFile);
			if (f.is_open())
				f.close();

			ParticleSystemPresenter *PSPresenter = new ParticleSystemPresenter();

			Saver::saveMap(ofn.lpstrFile, DOPresenter, PSPresenter, &moveBtnsControl);

			delete PSPresenter;
		}
		// Do something usefull with the filename stored in szFileName 
	}
}

void WinCreateModel::addObjToCurObjs(const char * objName, int intNum, int objId)
{
	if (currentObjs.find(objName) == currentObjs.end())
		currentObjs[objName] = {};

	if (currentObjs[objName].find(intNum) != currentObjs[objName].end())
		currentObjs[objName][intNum] = objId;
	else
	{
		currentObjs[objName][intNum] = {};
		currentObjs[objName][intNum] = objId;
	}
}

void WinCreateModel::addObjToCurObjs(HWND hDlg, int id)
{
	auto pair = DOPresenter->getNameIndexById(id);

	if (pair.second == -1)
		return;

	addObjToCurObjs(pair.first.c_str(), pair.second, id);

	std::string text = pair.first + "#" + std::to_string(pair.second);

	int idInList = SendDlgItemMessage(hDlg, IDC_LIST1, LB_FINDSTRING, (WPARAM)-1, (LPARAM)text.c_str());
	SendDlgItemMessageA(hDlg, IDC_LIST1, LB_SETSEL, (WPARAM)(BOOL)TRUE, idInList);
}

void WinCreateModel::addVertToCurVerts(int id, Vect3f * ptr)
{
	vector3ub color = colorMapForVertexes[id][ptr];
	if (currentVertexes.find(id) == currentVertexes.end())
		currentVertexes[id] = {};

	if (currentVertexes[id].find(ptr) != currentVertexes[id].end())
		currentVertexes[id][ptr] = color;
	else
	{
		currentVertexes[id][ptr] = {};
		currentVertexes[id][ptr] = color;
	}
}

bool WinCreateModel::isThisVertSelect(std::pair<int, Vect3f*> &pair)
{
	if (currentVertexes.find(pair.first) != currentVertexes.end())
		if (currentVertexes[pair.first].find(pair.second) != currentVertexes[pair.first].end())
			return true;
	return false;
}

bool WinCreateModel::isThisObjSelect(NameIndexId nii)
{
	if (currentObjs.find(nii.name) != currentObjs.end())
		if (currentObjs[nii.name].find(nii.index) != currentObjs[nii.name].end())
			return true;
	return false;
}

void WinCreateModel::applyChangesForMultList(HWND hDlg, HWND list)
{
	int curObjsSize = currentObjs.size();
	int numOfItems = SendMessage(list, LB_GETSELCOUNT, 0, 0);
	LPTSTR LBString = new TCHAR[80];

	char* buf = nullptr;
	char* objName = nullptr;

	char* num = nullptr;
	int intNum;

	int objId;

	auto initFunc = [&]() {
		objName = strtok_s(LBString, "#", &buf);

		num = strtok_s(NULL, "#", &buf);
		intNum = atoi(num);

		objId = DOPresenter->getObjId(objName, intNum);
	};

	if (numOfItems > curObjsSize) //add Object
	{
		int id = SendMessage(list, LB_GETCURSEL, 0, 0);

		SendMessage(list, LB_GETTEXT, (WPARAM)id, (LPARAM)LBString);

		initFunc();

		addObjToCurObjs(objName, intNum, objId);
		
	}
	else //del Object
	{
		objId = (int)SendMessage(list, LB_GETCURSEL, 0, 0);

		SendMessage(list, LB_GETTEXT, (WPARAM)objId, (LPARAM)LBString);

		initFunc();
		
		//Удалили из currentObjs
		currentObjs[objName].erase(currentObjs[objName].find(intNum));
		if (currentObjs[objName].size() == 0)
			currentObjs.erase(currentObjs.find(objName));
		if (currentVertexes.find(objId) != currentVertexes.end())
			currentVertexes.erase(currentVertexes.find(objId));
	}
	
	delete LBString;
}

void WinCreateModel::applyChangesForSingleList(HWND hDlg, HWND list)
{
	LPTSTR LBString = new TCHAR[80];

	int idCurSel = SendMessage(list, LB_GETCURSEL, 0, 0);
	int val = SendMessage(list, LB_GETSEL, (WPARAM)idCurSel, 0);
	SendMessage(list, LB_GETTEXT, (WPARAM)idCurSel, (LPARAM)LBString);

	char* buf = nullptr;
	char* objName = strtok_s(LBString, "#", &buf);

	char* num = strtok_s(NULL, "#", &buf);
	int intNum = atoi(num);

	int objId = DOPresenter->getObjId(objName, intNum);


	SendDlgItemMessage(hDlg, IDC_LIST1, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);
	clearCurrentObjs();
	if (currentVertexes.find(objId) != currentVertexes.end())
		currentVertexes.erase(currentVertexes.find(objId));
	if (val > 0) {
		SendDlgItemMessage(hDlg, IDC_LIST1, LB_SETSEL, (WPARAM)(BOOL)TRUE, idCurSel);
		addObjToCurObjs(objName, intNum, objId);
	}

	delete LBString;
}

void WinCreateModel::checkEqualityBetweenCurObjsAndList(HWND hDlg, HWND list)
{
	int curObjsSize = currentObjs.size();

	int numOfItems = SendMessage(list, LB_GETSELCOUNT, 0, 0);

	bool isMultiple = IsDlgButtonChecked(hDlg, IDC_CHECK1);

	if (isMultiple) 
	{
		DWORD style = (DWORD)GetWindowLong(list, GWL_STYLE);
		if ((style & LBS_MULTIPLESEL) == 0)
		{
			style |= LBS_MULTIPLESEL;
			SetWindowLong(list, GWL_STYLE, style);
		}
	}
	else 
	{
		DWORD style = (DWORD)GetWindowLong(list, GWL_STYLE);
		if ((style & LBS_MULTIPLESEL) != 0)
		{
			style &= (~LBS_MULTIPLESEL);
			int asd = SetWindowLong(list, GWL_STYLE, style);
			style = (DWORD)GetWindowLong(list, GWL_STYLE);
		}
	}

	if (isMultiple)
		applyChangesForMultList(hDlg, list);
	else
		applyChangesForSingleList(hDlg, list);
}

void WinCreateModel::clearCurrentObjs()
{
	currentObjs.clear();
}

void WinCreateModel::clearCurrentVertexes()
{
	for (auto it_id = currentVertexes.begin(); it_id != currentVertexes.end(); ++it_id) 
		currentVertexes[it_id->first] = {};
	currentVertexes.clear();
	
}

std::vector<std::string> WinCreateModel::getDrObjKeys(std::map<std::string, DrawableObject2*> map)
{
	std::vector<std::string> keys;

	for (auto it = map.begin(); it != map.end(); ++it)
		keys.push_back(it->first);

	return keys;
}

Vect3f WinCreateModel::getDifBetweenVerts(std::pair<int, Vect3f*>& staticVert, std::pair<int, Vect3f*>& connectedVert)
{
	auto staticDrObj = DOPresenter->getObj(staticVert.first);
	auto connectedDrObj = DOPresenter->getObj(connectedVert.first);

	auto staticOJLB = staticDrObj->getVertexes();
	auto connectedOJLB = connectedDrObj->getVertexes();

	auto getGlobalCoord = [](Vect3f objRotation, Vect3f objPosition,
		Vect3f OJLB_rot, Vect3f OJLB_pos,
		std::pair<int, Vect3f*> &vect) {
		auto cam = moveBtnsControl.getCamera();

		Vect3f cam_pos(cam.getX(), cam.getY(), cam.getZ());

		//Получаем матрицу перехода из B1 в global
		Matrix basisObj;
		basisObj.rotate(objRotation);
		//basisObj.getInvetMatrix(); // обратная матрица перехода

		//Получаем матрицу перехода из B2 в B1																	     
		Matrix basisOpenGL_baseObj;
		basisOpenGL_baseObj.rotate(OJLB_rot);

		//Получаем матрицу перехода из B2 в global
		Matrix fromB2ToGlob = basisObj * basisOpenGL_baseObj;

		//Получаем вектор, который надо добавить к координатам (получаемым ниже), чтобы получить координаты в global
		Vect3f additionToGlobal = Matrix::multMatOnVect(basisObj, OJLB_pos);
		additionToGlobal = additionToGlobal.sum(objPosition).sum(cam_pos);

		//Получаем координату точки объекта OpenGL_Base (из которых состоит сам DrawableObject) в global
		return Matrix::multMatOnVect(fromB2ToGlob, *vect.second).sum(additionToGlobal);
	};


	Vect3f staticVertGlobCoord;
	Vect3f connectedVertGlobCoord;


	auto it_staticOJLBPos = staticDrObj->getVertPosition()->begin();
	auto it_staticOJLBRot = staticDrObj->getVertRotation()->begin();
	for (auto it_OJLBs = staticOJLB->begin(); it_OJLBs != staticOJLB->end(); ++it_OJLBs)
	{
		for (auto its = (*it_OJLBs)->getVertexes()->begin(); its != (*it_OJLBs)->getVertexes()->end(); ++its)
		{
			if ((*its) == staticVert.second)
			{
				staticVertGlobCoord = getGlobalCoord(
					staticDrObj->getRotation(),
					*staticDrObj->getPosition(),
					**it_staticOJLBPos,
					**it_staticOJLBRot,
					staticVert);
				goto nextStep1;
			}
		}
		it_staticOJLBPos++;
	}
nextStep1:

	auto it_connectedOJLBPos = staticDrObj->getVertPosition()->begin();
	auto it_connectedOJLBRot = staticDrObj->getVertRotation()->begin();
	for (auto it_OJLBc = staticOJLB->begin(); it_OJLBc != staticOJLB->end(); ++it_OJLBc)
	{
		for (auto itc = (*it_OJLBc)->getVertexes()->begin(); itc != (*it_OJLBc)->getVertexes()->end(); ++itc)
		{
			if ((*itc) == staticVert.second)
			{
				connectedVertGlobCoord = getGlobalCoord(
					connectedDrObj->getRotation(),
					*connectedDrObj->getPosition(),
					**it_connectedOJLBPos,
					**it_connectedOJLBRot,
					connectedVert);
				goto nextStep2;
			}
		}
		it_staticOJLBPos++;
	}
nextStep2:

	return staticVertGlobCoord.minus(connectedVertGlobCoord);
}

void WinCreateModel::disableTextureMode()
{
	OGLB_ForMappedTexture.clear();
	isDrawModeOn = false;
}

void WinCreateModel::updateEditDlgForLog()
{
	for (auto it_name = currentObjs.begin(); it_name != currentObjs.end(); ++it_name)
		for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it) {

		}

}

LRESULT CALLBACK WinCreateModel::ModelCreatorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR drawCursor;
	static HCURSOR texCursor;
	static HCURSOR defaultCursor;

	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CREATE:
	{
		//drawCursor = LoadCursor(NULL, IDC_HAND);
		drawCursor = LoadCursorFromFile("zalivka2.cur");
		texCursor = LoadCursorFromFile("Arrow.ani");
		defaultCursor = GetCursor();
		//DOPresenter->objMapIndex
		
		return TRUE;
	}

	case WM_SETCURSOR:
	{
		if (isDrawModeOn)
			SetCursor(drawCursor);
		else if (isMapTextureModeOn)
			SetCursor(texCursor);
		else
			SetCursor(defaultCursor);


		return TRUE;
	}
		//CreateWindow("button", "Добавить запись", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,

		//5, 5, 150, 20, hWnd, (HMENU)(0001), NULL, NULL);
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))					// Check Minimization State
		{
			addVars.active = TRUE;						// Program Is Active
		}
		else
		{
			addVars.active = FALSE;						// Program Is No Longer Active
		}

		return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
		switch (wParam)							// Check System Calls
		{
		case SC_SCREENSAVE:					// Screensaver Trying To Start?
		case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		}
		break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		addVars.keys[wParam] = TRUE;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}

	case WM_MOUSEWHEEL:
	{
		addVars.wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		addVars.keys[1] = TRUE;
		return 0;
	}

	case WM_LBUTTONUP:
	{
		addVars.keys[1] = FALSE;
		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		addVars.keys[2] = true;
		return 0;
	}

	case WM_RBUTTONUP:
	{
		addVars.keys[2] = false;
		return 0;
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
		addVars.keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}

	case WM_GETMINMAXINFO: //Получили сообщение от Винды
	{
		MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
		POINT Min = { 640, 480 };
		POINT  Max = { 1000, 800 };
		pInfo->ptMinTrackSize = Min; // Установили минимальный размер
		pInfo->ptMaxTrackSize = Max; // Установили максимальный размер
		return 0;
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
		WindowHelper::ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK WinCreateModel::ItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND list;
	CameraLocation cam;
	Vect3f direct;
	int selectedItem;
	char* text2 = nullptr;
	int lbItem, i;
	LPTSTR LBString = new TCHAR[128];
	std::vector<OpenGL_Base*> *obj;
	StructForCreateObj _struct;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND list = GetDlgItem(hDlg, IDC_LIST_ALL_ITEMS);
		//SendMessage(list, LB_SETITEMDATA, 0, (LPARAM)"asd");
		//int pos = (int)SendMessage(list, LB_ADDSTRING, 0,
			//(LPARAM)"item");


		//
		std::vector<std::string> keys = WinCreateModel::getDrObjKeys(drawableObjsMap);
		for (auto key : keys)
			SendMessage(list, LB_ADDSTRING, 0,	(LPARAM)key.c_str());




		// Set the array index of the player as item data.
		// This enables us to retrieve the item from the array
		// even after the items are sorted by the list box.
		//SendMessage(list, LB_SETITEMDATA, pos, (LPARAM)0);

		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ADD:
		{
			//list = GetDlgItem(elementsMap["commonObjList"], IDC_LIST1);
			list = GetDlgItem(hDlg, IDC_LIST_ALL_ITEMS);
			//SendMessage(list, LB_SETITEMDATA, 0, (LPARAM)"asd");


			// Get selected index.
			lbItem = (int)SendMessage(list, LB_GETCURSEL, 0, 0);

			if (lbItem == -1)
				return TRUE;

			// Get item data.
			//i = (int)SendMessage(list, LB_GETITEMDATA, lbItem, 0);

			//std::string text;
			
			SendMessage(list, LB_GETTEXT, (WPARAM)lbItem, (LPARAM)LBString);

			(int)SendMessage(list, LB_ADDSTRING, 0,
				(LPARAM)LBString);
			//color map for objects
			i = DOPresenter->addObj(drawableObjsMap[LBString], LBString, moveBtnsControl.getCamera());

			сolorMapForObjects[i] = getColorForSimpleDraw(сolorMapForObjects);

			direct = Vect3f(0, 0, -1);
			cam = moveBtnsControl.getCamera();

			direct = Matrix::rotationMatrix_X(direct, cam.getYAngle());
			direct = Matrix::rotationMatrix_Y(direct, cam.getXAngle());
			direct = direct.mul(5.0f);

			DOPresenter->getObj(i)->move(direct);

			//color map for vertexes
			obj = DOPresenter->getObj(i)->getVertexes();
			
			for (auto it_baseObj = obj->begin(); it_baseObj != obj->end(); ++it_baseObj) {
				auto vertexes = (*it_baseObj)->getVertexes();
				for (auto it = vertexes->begin(); it != vertexes->end(); ++it) {
					VertexKey vertexKey = { i, *it };
					if (colorMapForVertexes.find(i) == colorMapForVertexes.end())
						colorMapForVertexes[i] = {};

					if (colorMapForVertexes[i].find(*it) == colorMapForVertexes[i].end())
						colorMapForVertexes[i][*it] = getColorForVertColorMap(colorMapForVertexes);
				}
			}

			haveSmthgNew = true;

			
			newObjName = std::string(LBString) + "#" + std::to_string(i);
			//currentObjs[]

			
			_struct.cam = moveBtnsControl.getCamera();
			_struct.index = i;
			_struct.objName = LBString;
			_struct.presenterName = "global";

			logger.log(drawableObjsMap[LBString], &_struct, Logger::Create_Obj);
			//HWND dlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, ListBoxExampleProc);
			//ShowWindow(dlg, nCmdShow);

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
		}
		return TRUE;

		case IDC_LIST_ALL_ITEMS:
		{
			switch (HIWORD(wParam))
			{

			case LBN_SELCHANGE:
			{
				list = GetDlgItem(hDlg, IDC_LIST1);
				//SendMessage(list, LB_SETITEMDATA, 0, (LPARAM)"asd");


				// Get selected index.
				lbItem = (int)SendMessage(list, LB_GETCURSEL, 0, 0);

				// Get item data.
				i = (int)SendMessage(list, LB_GETITEMDATA, lbItem, 0);

				//std::string text;

				SendMessage(list, LB_GETTEXT, (WPARAM)i, (LPARAM)LBString);

			}
			}
		}
		return TRUE;
		}
	}
	return FALSE;
}

INT_PTR WinCreateModel::RoundItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_INITDIALOG:
	{
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_A_KOEF, 1);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_B_KOEF, 1);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_C_KOEF, 1);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_BOT_POS_X, 0);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_BOT_POS_Y, 0);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_BOT_POS_Z, 0);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TOP_POS_X, 0);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TOP_POS_Y, 1);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TOP_POS_Z, 0);

		return TRUE;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_CREATE_DISK:
		{
			Vect3f botPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_BOT_POS_X, IDC_EDIT_BOT_POS_Y, IDC_EDIT_BOT_POS_Z);

			GLfloat a_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_A_KOEF);
			GLfloat c_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_C_KOEF);
			int size = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_TRIANGLES);
			//size = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_TRIANGLES);

			
			auto disk = genDisc(botPos, a_koef, c_koef, size);

			disk->setVertSize(0.05);
			//
			int i = DOPresenter->addObj(disk, "Disk", moveBtnsControl.getCamera());

			сolorMapForObjects[i] = getColorForSimpleDraw(сolorMapForObjects);

			newObjName = std::string("Disk") + "#" + std::to_string(i);

			SendDlgItemMessage(elementsMap["ObjDlg"], IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl.getCamera();
			_struct.index = i;
			_struct.objName = "Disk";
			_struct.presenterName = "global";

			logger.log(disk, &_struct, Logger::Create_Obj);

			EndDialog(hDlg, LOWORD(wParam));

			return TRUE;
		}
		
		case ID_CREATE_CONE:
		{
			Vect3f botPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_BOT_POS_X, IDC_EDIT_BOT_POS_Y, IDC_EDIT_BOT_POS_Z);

			Vect3f topPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_TOP_POS_X, IDC_EDIT_TOP_POS_Y, IDC_EDIT_TOP_POS_Z);

			GLfloat a_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_A_KOEF);
			GLfloat c_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_C_KOEF);
			GLfloat size = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_TRIANGLES);

			auto cone = genCone(botPos, topPos, a_koef, c_koef, size);

			cone->setVertSize(0.05);
			//
			int i = DOPresenter->addObj(cone, "Cone", moveBtnsControl.getCamera());

			сolorMapForObjects[i] = getColorForSimpleDraw(сolorMapForObjects);

			newObjName = std::string("Cone") + "#" + std::to_string(i);

			SendDlgItemMessage(elementsMap["ObjDlg"], IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl.getCamera();
			_struct.index = i;
			_struct.objName = "Cone";
			_struct.presenterName = "global";

			logger.log(cone, &_struct, Logger::Create_Obj);

			return TRUE;
		}

		case ID_CREATE_CYLINDER:
		{
			Vect3f botPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_BOT_POS_X, IDC_EDIT_BOT_POS_Y, IDC_EDIT_BOT_POS_Z);

			Vect3f topPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_TOP_POS_X, IDC_EDIT_TOP_POS_Y, IDC_EDIT_TOP_POS_Z);

			GLfloat a_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_A_KOEF);
			GLfloat c_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_C_KOEF);
			GLfloat size = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_TRIANGLES);

			auto cylinder = genCylinder(botPos, topPos, a_koef, c_koef, size);

			cylinder->setVertSize(0.05);
			//
			int i = DOPresenter->addObj(cylinder, "Cylinder", moveBtnsControl.getCamera());

			сolorMapForObjects[i] = getColorForSimpleDraw(сolorMapForObjects);

			newObjName = std::string("Cylinder") + "#" + std::to_string(i);

			SendDlgItemMessage(elementsMap["ObjDlg"], IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl.getCamera();
			_struct.index = i;
			_struct.objName = "Cylinder";
			_struct.presenterName = "global";

			logger.log(cylinder, &_struct, Logger::Create_Obj);

			return TRUE;
		}

		case ID_CREATE_ELLIPSOID:
		{
			Vect3f botPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_BOT_POS_X, IDC_EDIT_BOT_POS_Y, IDC_EDIT_BOT_POS_Z);

			GLfloat a_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_A_KOEF);
			GLfloat b_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_B_KOEF);
			GLfloat c_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_C_KOEF);
			GLfloat size = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_TRIANGLES);

			auto ellipsoid = genEllipsoid(botPos, a_koef, b_koef, c_koef, size);

			ellipsoid->setVertSize(0.05);
			//
			int i = DOPresenter->addObj(ellipsoid, "Ellipsoid", moveBtnsControl.getCamera());

			сolorMapForObjects[i] = getColorForSimpleDraw(сolorMapForObjects);

			newObjName = std::string("Ellipsoid") + "#" + std::to_string(i);

			SendDlgItemMessage(elementsMap["ObjDlg"], IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl.getCamera();
			_struct.index = i;
			_struct.objName = "Ellipsoid";
			_struct.presenterName = "global";

			logger.log(ellipsoid, &_struct, Logger::Create_Obj);

			return TRUE;
		}

		}



		return TRUE;
	}

	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
	}


	return FALSE;
}

INT_PTR WinCreateModel::ItemRotationDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static std::map<int, Vect3f> objsRotations;
	static std::map<int, Vect3f> objsPositions;
	static std::map<Vect3f*, Vect3f> vertsPositions;
	static std::map<DrawableObject2*, Vect3f> objsScales;
	static Vect3f pos;
	static Vect3f rot;
	static Vect3f vpos;
	static bool isOnlyOneObj;
	static bool isOnlyOneVert;
	int selectedItem;
	char* text2 = nullptr;
	int lbItem, i;
	LPTSTR text = new TCHAR[10];

	static auto setDefault = [](HWND hDlg, Vect3f &vect, bool isOnlyOneSelected, int e1, int e2, int e3, int step) {
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
		//SendDlgItemMessage(hDlg, IDC_EDIT_POS_STEP, WM_SETTEXT, 0, (LPARAM)"1.0");
		//SendDlgItemMessage(hDlg, IDC_EDIT_VPOS_STEP, WM_SETTEXT, 0, (LPARAM)"1.0");

	};

	static auto switchVertTrans = [](HWND hDlg, int val) {

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
	};

	static auto changeData = [](HWND hDlg, WPARAM wParam, int editVal, int editStep, int btnDown, int idApply) {
		GLfloat val = DlgHelper::getValFromEdit(hDlg, editVal);
		GLfloat step = DlgHelper::getValFromEdit(hDlg, editStep);
		if (LOWORD(wParam) == btnDown)
			step *= -1;

		val += step;
		DlgHelper::setValForEdit(hDlg, editVal, val);
		SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(idApply, 0), 0);
	};

	static auto updateCurObjsData = [](std::map<int, Vect3f> &objsRotations,
		std::map<int, Vect3f> &objsPositions, bool &isOnlyOneObj, std::map<std::string, std::map<int, int>> &currentObjs) {
		int i = 0;
		objsPositions.clear();
		objsRotations.clear();
		for (auto it_name = currentObjs.begin(); it_name != currentObjs.end(); ++it_name)
			for (auto it = currentObjs[it_name->first].begin(); it != currentObjs[it_name->first].end(); ++it) {
				i++;
				objsRotations[it->second] = DOPresenter->getObj(it->second)->getRotation();
				objsPositions[it->second] = *DOPresenter->getObj(it->second)->getPosition();
			}

		if (i == 1) {
			pos = objsPositions.begin()->second;
			rot = objsRotations.begin()->second;
			isOnlyOneObj = true;
		}
		else
			isOnlyOneObj = false;
	};

	static auto updateCurVertData = [](bool &isOnlyOneVert,
		std::map<int, std::map<Vect3f*, vector3ub>> &currentVertexes) {


		for (auto it_id = currentVertexes.begin(); it_id != currentVertexes.end(); ++it_id)
			for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it)
			{
				vertsPositions[it->first] = *(it->first);
			}

		if (currentVertexes.size() != 0 && currentVertexes.begin()->second.size() == 1) {
			vpos = *(currentVertexes.begin()->second.begin()->first);
			isOnlyOneVert = true;
		}
		else
			isOnlyOneVert = false;
	};

	static auto updateCurObjsScale = [](bool &isOnlyOneObj, DrawableObjectsPresenter2* DOPresenter,
		std::map<std::string, std::map<int, int>> &currentObjs) {

		objsScales.clear();
		for (auto it_id = currentObjs.begin(); it_id != currentObjs.end(); ++it_id)
			for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it)
			{
				auto obj = DOPresenter->getObj(it->second);
				objsScales[obj] = obj->getScal();
			}

		if (objsScales.size() == 1)
			isOnlyOneObj = true;
		else
			isOnlyOneObj = false;
	};

	switch (message)
	{
	case WM_INITDIALOG:
	{
		addVars.keys['E'] = FALSE;

		switchVertTrans(hDlg, FALSE);

		updateCurObjsData(objsRotations, objsPositions, isOnlyOneObj, currentObjs);
		updateCurVertData(isOnlyOneVert, currentVertexes);
		updateCurObjsScale(isOnlyOneObj, DOPresenter, currentObjs);

		setDefault(hDlg, pos, isOnlyOneObj, IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, IDC_EDIT_POS_STEP);
		setDefault(hDlg, rot, isOnlyOneObj, IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, IDC_EDIT_ROT_STEP);
		setDefault(hDlg, vpos, isOnlyOneVert, IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z, IDC_EDIT_VPOS_STEP);

		if (isOnlyOneObj)
			setDefault(hDlg, objsScales.begin()->second, true, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
		else
			setDefault(hDlg, Vect3f(1, 1, 1), true, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);


		return TRUE;
	}

	case PD::WM_CHECKVERTDATA:
	{
		updateCurVertData(isOnlyOneVert, currentVertexes);
		setDefault(hDlg, vpos, isOnlyOneVert, IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z, IDC_EDIT_VPOS_STEP);

		return TRUE;
	}

	case PD::WM_CHECHOBJDATA:
	{
		updateCurObjsData(objsRotations, objsPositions, isOnlyOneObj, currentObjs);
		updateCurObjsScale(isOnlyOneObj, DOPresenter, currentObjs);
		setDefault(hDlg, pos, isOnlyOneObj, IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, IDC_EDIT_POS_STEP);
		setDefault(hDlg, rot, isOnlyOneObj, IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, IDC_EDIT_ROT_STEP);

		if (isOnlyOneObj)
			setDefault(hDlg, objsScales.begin()->second, true, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
		else
			setDefault(hDlg, Vect3f(1, 1, 1), true, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);

		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_APPLY_ROT:
		{
			Vect3f rotVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z);

			std::vector<DrawableObject2*> *drObjVectForLog = new std::vector<DrawableObject2*>();
			for (auto it_name = currentObjs.begin(); it_name != currentObjs.end(); ++it_name)
				for (auto it = currentObjs[it_name->first].begin(); it != currentObjs[it_name->first].end(); ++it) {
					auto drObj = DOPresenter->getObj(it->second);

					if (isOnlyOneObj) {
						drObj->setRotation(rotVect);
						drObj->updateCoords();
						drObj->updateCube();
					}
					else {
						drObj->setRotation(objsRotations[it->second].sum(rotVect));
						drObj->updateCoords();
						drObj->updateCube();
					}

					drObjVectForLog->push_back(drObj);
				}

			logger.log(drObjVectForLog, &rotVect, Logger::Rot_Objs);

			return TRUE;
		}

		case ID_APPLY_POS:
		{
			Vect3f posVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z);

			if (posVect.getX() == 0)
				std::cout << 1;

			for (auto it_name = currentObjs.begin(); it_name != currentObjs.end(); ++it_name)
				for (auto it = currentObjs[it_name->first].begin(); it != currentObjs[it_name->first].end(); ++it) {
					if (isOnlyOneObj) {
						auto drObj = DOPresenter->getObj(it->second);
						drObj->setPosition(posVect);
						drObj->updateCoords();
						drObj->updateCube();
					}
					else {
						auto drObj = DOPresenter->getObj(it->second);
						drObj->setPosition(objsPositions[it->second].sum(posVect));
						drObj->updateCoords();
						drObj->updateCube();
					}
				}

			return TRUE;
		}

		case ID_APPLY_SCAL:
		{
			Vect3f newScalForLog = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z);
			Vect3f oldScal;

			if (currentObjs.size() != 0 && currentObjs.begin()->second.size() != 0) {

			}
			else
				return TRUE;

			auto cam = moveBtnsControl.getCamera();
			std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>
				*globMap = new std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>();

			bool isScalDet = false;
			for (auto it_name = currentObjs.begin(); it_name != currentObjs.end(); ++it_name)
				for (auto it_pair = it_name->second.begin(); it_pair != it_name->second.end(); ++it_pair)
				{
					auto drObj = DOPresenter->getObj(it_pair->second);

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

				it_map->first->updateCoords();
				it_map->first->updateCube();
			}

			logger.log(globMap, &MapOldNewScal(shifts, oldScal, newScalForLog), Logger::Scal_Objs);

			return TRUE;
		}

		case ID_APPLY_VPOS:
		{
			Vect3f vposVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z);;

			if (currentVertexes.size() == 1)
				if (currentVertexes.begin()->second.size() == 1) {
					currentVertexes.begin()->second.begin()->first->set(vposVect);
					auto vect = new std::vector<Vect3f*>();
					auto curPos = currentVertexes.begin()->second.begin()->first;

					vect->push_back(curPos);

					auto drObj = DOPresenter->getObj(currentVertexes.begin()->first);
					drObj->updateCoords();
					drObj->updateCube();

					logger.log(vect, &(vertsPositions[curPos].sum(vposVect).minus(*curPos)), Logger::Move_Verts);
					return TRUE;
				}

			std::vector<Vect3f*> *vect = new std::vector<Vect3f*>();
			Vect3f vposVectForLog;
			bool isFirst = true;
			for (auto it_id = currentVertexes.begin(); it_id != currentVertexes.end(); ++it_id) {
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

				auto drObj = DOPresenter->getObj(currentVertexes.begin()->first);
				drObj->updateCoords();
				drObj->updateCube();
			}
			//if (isOnlyOneVert)
			logger.log(vect, &vposVectForLog, Logger::Move_Verts);

			return TRUE;
		}

		case ID_SET_DEFAULT_ROT:
		{
			setDefault(hDlg, rot, isOnlyOneObj,
				IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, IDC_EDIT_ROT_STEP);

			return TRUE;
		}

		case ID_SET_DEFAULT_POS:
		{
			setDefault(hDlg, pos, isOnlyOneObj,
				IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, IDC_EDIT_POS_STEP);

			return TRUE;
		}

		case ID_SET_DEFAULT_SCAL:
		{
			if (isOnlyOneObj)
				setDefault(hDlg, objsScales.begin()->second, true,
					IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
			else
				setDefault(hDlg, Vect3f(1, 1, 1), true,
					IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);


			return TRUE;
		}

		case ID_SET_DEFAULT_VPOS:
		{
			setDefault(hDlg, vpos, isOnlyOneVert,
				IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z, IDC_EDIT_VPOS_STEP);

			return TRUE;
		}

		case ID_SWITCHER_VERT_POS:
		{
			auto el = GetDlgItem(hDlg, ID_SWITCHER_VERT_POS);

			SendMessage(el, WM_GETTEXT, (WPARAM)10, (LPARAM)text);

			if (std::string(text) == "Enable") {
				SendMessage(el, WM_SETTEXT, (WPARAM)10, (LPARAM)"Disable");
				switchVertTrans(hDlg, TRUE);
			}
			else {
				SendMessage(el, WM_SETTEXT, (WPARAM)10, (LPARAM)"Enable");
				switchVertTrans(hDlg, FALSE);
			}

			return TRUE;
		}

		case IDC_CHECK_MUL_VERT:
		{
			isMulVertexes = IsDlgButtonChecked(hDlg, IDC_CHECK_MUL_VERT);

			return TRUE;
		}

		case IDC_CHECK_MOVE_VERT:
		{
			isMoveVertexes = IsDlgButtonChecked(hDlg, IDC_CHECK_MOVE_VERT);

			return TRUE;
		}

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

			return TRUE;
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

			return TRUE;
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

			return TRUE;
		}


		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			alreadyPressedE = false;
		}
		return TRUE;

	}
	//return DefWindowProc(hDlg, message, wParam, lParam);
	return FALSE;
}

INT_PTR WinCreateModel::MappedTextureDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_INITDIALOG:
	{
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TEXPOS_X_R, 0);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TEXPOS_Y_R, 0);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TEXPOS_X_G, 1);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TEXPOS_Y_G, 0);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TEXPOS_X_B, 0.5);
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_TEXPOS_Y_B, 0.5);

		return TRUE;
	}
	
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_APPLY_TEXTURE:
		{
			GLfloat r_val_x = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_TEXPOS_X_R);
			GLfloat r_val_y = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_TEXPOS_Y_R);
			GLfloat g_val_x = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_TEXPOS_X_G);
			GLfloat g_val_y = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_TEXPOS_Y_G);
			GLfloat b_val_x = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_TEXPOS_X_B);
			GLfloat b_val_y = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_TEXPOS_Y_B);

			std::vector<OpenGL_Base*> * vectForLog = new std::vector<OpenGL_Base*>();
			std::vector<ColorTexture*> *oldStruct = new std::vector<ColorTexture*>();

			for (auto it_id = OGLB_ForMappedTexture.begin(); it_id != OGLB_ForMappedTexture.end(); ++it_id) {
				auto map = it_id->second;

				for (auto it_OGLB = map.begin(); it_OGLB != map.end(); ++it_OGLB) {


					auto objColors = (*it_OGLB)->getColors();
					std::vector<Vect4f*> * colorsForLog = genColorVect4f(objColors);
					
					auto objTexCoord = (*it_OGLB)->getTextureCoord();
					std::vector<Vect2f*> * texCoordForLog = genColorVect2f(objTexCoord);

					oldStruct->push_back(
						new ColorTexture(
							colorsForLog,
							(*it_OGLB)->getPathToTexture(), 
							(*it_OGLB)->getTexture(),
							texCoordForLog)
					);


					std::vector<Vect2f*>* texCoord = new std::vector<Vect2f*>();

					texCoord->push_back(new Vect2f(r_val_x, r_val_y));
					texCoord->push_back(new Vect2f(g_val_x, g_val_y));
					texCoord->push_back(new Vect2f(b_val_x, b_val_y));


					(*it_OGLB)->setTexture(pairNameTexForObjs.second, texCoord, pairNameTexForObjs.first);

					vectForLog->push_back(*it_OGLB);
				}
			}
			std::vector<Vect2f*>* texCoord = new std::vector<Vect2f*>();

			texCoord->push_back(new Vect2f(r_val_x, r_val_y));
			texCoord->push_back(new Vect2f(g_val_x, g_val_y));
			texCoord->push_back(new Vect2f(b_val_x, b_val_y));

			ColorTexture * newStruct = new ColorTexture(nullptr, pairNameTexForObjs.first,
				pairNameTexForObjs.second, texCoord);

			std::pair<ColorTexture*, std::vector<ColorTexture*>*> pair;
			pair.first = newStruct;
			pair.second = oldStruct;

			std::pair<std::vector<OpenGL_Base*>*, bool> curObj;
			curObj.first = vectForLog;
			curObj.second = true;

			logger.log(&curObj, &pair, Logger::Set_Texture);

			EndDialog(hDlg, LOWORD(wParam));
			disableTextureMode();
			canMapTexture = true;
			return TRUE;
		}
		}

		return TRUE;
	}

	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		disableTextureMode();
		canMapTexture = true;
	}


	return FALSE;
}

INT_PTR CALLBACK WinCreateModel::ItemsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	HDC testDC;
	PAINTSTRUCT ps;
	static LPTSTR LBString = new TCHAR[80];
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	char* buf = nullptr;
	char* objName = nullptr;
	char* num = nullptr;
	int intNum;
	int objId;
	//int idBuf[50];

	static COLORREF acrCustClr[16];	// массив пользовательских цветов
	static DWORD rgbCurrent;		// выбор начального цвета
	static HBRUSH hBrush;
	
	if (haveSmthgNew == true)
	{
		haveSmthgNew = false;
		HWND list = GetDlgItem(hDlg, IDC_LIST1);

		SendMessage(list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
	}

	switch (message)
	{
	case WM_PAINT:
	{
		/*
		testDC = BeginPaint(hDlg, &ps);
		SelectObject(testDC, font);
		
		::TextOut(testDC, 150, 150, "asd", strlen("asd"));
		EndPaint(hDlg, &ps);
		*/

		testDC = BeginPaint(hDlg, &ps); //начали рисование
		hBrush = CreateSolidBrush(color);
		SelectObject(testDC, hBrush);

		//SetBkMode(hdc, TRANSPARENT);
		//SetBkColor(hdc, color);

		//Rectangle(testDC, rect.right + 5, rect.top, rect.right + 5 + rect.bottom - rect.top, rect.bottom);
		Rectangle(testDC, 220, 255, 240, 275);
		//TextOut(hdc, 20, 10, "Выберите цвет главного окна:", 28);

		EndPaint(hDlg, &ps); //закончили рисование


		return TRUE;
	}
	case WM_INITDIALOG:
	{
		//drawCursor = LoadCursorFromFile("C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Data\\zalivka2.cur");
		//drawCursor = (HCURSOR)LoadImage(NULL, "C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Data\\zalivka2.png",
			//IMAGE_CURSOR, 225, 225, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
		//drawCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_DRAW_CURSOR));
		/*
		HWND list = GetDlgItem(hDlg, IDC_LIST1);
		//SendMessage(list, LB_SETITEMDATA, 0, (LPARAM)"asd");
		int pos = (int)SendMessage(list, LB_ADDSTRING, 0,
		(LPARAM)"test");
		*/
		// Set the array index of the player as item data.
		// This enables us to retrieve the item from the array
		// even after the items are sorted by the list box.
		//SendMessage(list, LB_SETITEMDATA, pos, (LPARAM)0);
		/*
		HWND list = GetDlgItem(hDlg, IDC_LIST1);

		std::vector<std::string> names = DOPresenter->getObjNames();
		auto objMapIndex = DOPresenter->getObjMapIndex();
		for (auto name : names) {
			for (auto it = (*objMapIndex)[name].begin(); it != (*objMapIndex)[name].end(); ++it) {

				std::string str = name + "#" + std::to_string(it->first);

				SendMessage(list, LB_ADDSTRING, 0, (LPARAM)str.c_str());
			}
		}
		//SendMessage(list, LB_ADDSTRING, 0, (LPARAM)key.c_str());

		*/
		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			//HWND dlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, addItemProc, NULL);
			//HWND dlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, ListBoxExampleProc);
			//ShowWindow(dlg, 0);
			DialogBox(ModelCreator_hInstance,
				MAKEINTRESOURCE(IDD_ADD_ITEM_DLG),
				hDlg, (DLGPROC)ItemsAddDlgProc);

			return TRUE;
		}

		case IDC_BUTTON_DELETE:
		{
			if (!(currentObjs.size() == 1 && currentObjs.begin()->second.size() == 1)) {
				MessageBox(ModelCreator_hWnd, "Can't delete object", "Please, choose only one object for delete", MB_OK);
				return TRUE;
			}

			auto obj = DOPresenter->getObj(currentObjs.begin()->second.begin()->second);

			auto indexesMap = DOPresenter->getObjMapIndex();

			auto objName = currentObjs.begin()->first;
			auto objIndex = currentObjs.begin()->second.begin()->first;
			auto objId = currentObjs.begin()->second.begin()->second;

			(*indexesMap)[objName].erase(objIndex);

			if ((*indexesMap)[objName].size() == 0)
				indexesMap->erase(objName);

			DOPresenter->eraseObjFromObjMap(objId);
			//DOPresenter

			int itemIndexInList = SendDlgItemMessage(elementsMap["ObjDlg"], IDC_LIST1, LB_FINDSTRING, 0,
				(LPARAM)(std::string(objName) + "#" + std::to_string(objIndex)).c_str());

			SendDlgItemMessage(elementsMap["ObjDlg"], IDC_LIST1, LB_DELETESTRING, itemIndexInList, 0);


			//
			LPTSTR LBString = new TCHAR[128];

			for (int i = 0; i < objName.length(); ++i)
				LBString[i] = objName[i];

			LBString[objName.length()] = '\0';

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl.getCamera();
			_struct.index = objIndex;
			_struct.objName = LBString;
			_struct.presenterName = "global";

			logger.log(obj, &_struct, Logger::Delete_Obj);

			return TRUE;
		}

		case ID_BUTTON_ADD_ROUND_OBJ:
		{
			DialogBox(ModelCreator_hInstance,
				MAKEINTRESOURCE(IDD_ADD_ROUND_OBJS_DLG),
				hDlg, (DLGPROC)RoundItemsAddDlgProc);



			return TRUE;
		}

		case IDC_CHOOSE_COLOR:
		{
			CHOOSECOLOR cc;
											// Инициализация CHOOSECOLOR
			ZeroMemory(&cc, sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = ModelCreator_hWnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = rgbCurrent;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc))
			{
				hBrush = CreateSolidBrush(cc.rgbResult);
				color = rgbCurrent = cc.rgbResult;
			}


			InvalidateRect(hDlg, NULL, TRUE);
			UpdateWindow(hDlg);
			return TRUE;
		}

		case IDC_DRAW:
		{
			isMapTextureModeOn = false;
			//SendMessage(ModelCreator_hWnd, PD::WM_CHANGECURSOR, 0, 0);
			isDrawModeOn = !isDrawModeOn;

			return TRUE;
		}

		case IDC_MAP_TEXTURE:
		{
			isDrawModeOn = false;
			isMapTextureModeOn = !isMapTextureModeOn;

			return TRUE;
		}

		case IDC_LIST1:
		{
			switch (HIWORD(wParam))
			{

			case LBN_SELCHANGE:
			{
				char* str = "asdasd";

				HWND list = GetDlgItem(hDlg, IDC_LIST1);

				checkEqualityBetweenCurObjsAndList(hDlg, list);

				InvalidateRect(hDlg, NULL, TRUE);
				UpdateWindow(hDlg);

				if (elementsMap["EditDlg"]) {
					SendMessage(elementsMap["EditDlg"], PD::WM_CHECHOBJDATA, 0, 0);
					SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
				}

				return TRUE;
			}
			}
			return TRUE;
		}

		case IDC_BUTTON_SAVE:
		{
			saveModel();

			return TRUE;
		}

		case IDC_BUTTON_SAVE_AS_A_MAP:
		{
			saveAsAMap();

			return TRUE;
		}

		case IDC_CHECK_ONLY_ONE_VERT:
		{
			isOnlyOneVertex = IsDlgButtonChecked(hDlg, IDC_CHECK_ONLY_ONE_VERT);
			if (isOnlyOneVertex == true) {
				auto hWnd = GetDlgItem(hDlg, IDC_CHECK_ONLY_FOR_OGLB);
				isOnlyForOGJB = false;
				SendDlgItemMessage(hDlg, IDC_CHECK_ONLY_FOR_OGLB, BM_SETCHECK, FALSE, 0);
			}
				
			return TRUE;
		}

		case IDC_CHECK_ONLY_FOR_OGLB:
		{
			isOnlyForOGJB = IsDlgButtonChecked(hDlg, IDC_CHECK_ONLY_FOR_OGLB);
			if (isOnlyForOGJB == true) {
				auto hWnd = GetDlgItem(hDlg, IDC_CHECK_ONLY_ONE_VERT);
				isOnlyOneVertex = false;
				SendDlgItemMessage(hDlg, IDC_CHECK_ONLY_ONE_VERT, BM_SETCHECK, FALSE, 0);
			}

			return TRUE;
		}

		return TRUE;
		}
	}
	return FALSE;
}

WinCreateModel::WinCreateModel(HWND &main_hWnd)
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	HGLRC		ModelCreator_hRC = NULL;		// Permanent Rendering Context

	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.f };

	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, .0f }; // Значения диффузного света ( НОВОЕ )

	GLfloat LightPosition[] = { 0.0f, .0f, 0.0f, .0f };     // Позиция света ( НОВОЕ )

	std::vector<LightGL> lights_vect = { LightGL(LightAmbient, LightDiffuse, LightPosition) };

	logger.setElementsMap(&elementsMap);
	logger.addDrObjPresenter("global", DOPresenter);

	if (!WindowHelper::CreateGLWindow(ModelCreator_hWnd, ModelCreator_hInstance, ModelCreatorWndProc,
		&main_hWnd, ModelCreator_hDC, ModelCreator_hRC,
		InitGL, lights_vect, "ModelCreator", 0, 0, "NeHe's Texture Mapping Tutorial", 640, 480, 16))
	{
		return;									// Quit If Window Was Not Created
	}
	Init();
	InitModelCreatorWindow(modelsMap);

	auto dlg = CreateDialogParam(ModelCreator_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), ModelCreator_hWnd, ItemsDlgProc, NULL);
	elementsMap["ObjDlg"] = dlg;
	//HWND dlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, ListBoxExampleProc);
	ShowWindow(dlg, TRUE);

	while (!done)									// Loop That Runs While done=FALSE
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				if (MessageBox(ModelCreator_hWnd, "Вы уверены, что хотите выйти?\nВсе несохранённые изменения будут потеряны", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					done = TRUE;							// If So done=TRUE
					WindowHelper::KillGLWindow(ModelCreator_hWnd, ModelCreator_hInstance, ModelCreator_hDC, ModelCreator_hRC, "ModelCreator");
					DestroyWindow(elementsMap["commonObjList"]);
					EnableWindow(main_hWnd, TRUE);
				}
				//ShowWindow(main_hWnd, TRUE);
				//wglMakeCurrent();
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			//Draw mode
			if (addVars.keys[VK_LBUTTON] == TRUE && isDrawModeOn && !isMapTextureModeOn && !addVars.keys[VK_SHIFT] && !addVars.keys[VK_SPACE])
			{
				addVars.keys[VK_LBUTTON] = FALSE;
				auto res = GraphicHelper::getSelectedObj(ModelCreator_hWnd,
					moveBtnsControl.getCamera(), DOPresenter, сolorMapForObjects);

				if (isThisObjSelect(res))
				{
					auto getColorVect = [](DrawableObjectsPresenter2 * DOPresenter,
						std::map<int, std::map<Vect3f*, vector3ub>>& currentVertexes,
						NameIndexId &res) -> std::pair<int, OpenGL_Base*> {
						if (currentVertexes.size() == 1) {
							if (currentVertexes.begin()->second.size() == 1)
							{
								auto *vectPtr = currentVertexes.begin()->second.begin()->first;
								std::pair<int, Vect3f*> params = { res.id, vectPtr };
								auto pair = DOPresenter->getOJLBByIdAndPosVertPtr(params);
								if (pair.first == -1)
									return {-1, nullptr};
								auto vertPos = pair.second->getVertexes();

								int pos = -1;
								for (int i = 0; i < vertPos->size(); ++i)
									if ((*vertPos)[i] == vectPtr) {
										pos = i;
										break;
									}

								return { pos, pair.second };//(*(pair.second->getColors()))[pos];
							}
						}
						return { -1, nullptr };
					};

					if (isOnlyForOGJB) //Тут что-то не так
					{
						auto idForVert = GraphicHelper::getSelectedOpenGL_Base(ModelCreator_hWnd,
							moveBtnsControl.getCamera(), DOPresenter->getObj(res.id));

						auto OGLB = (*DOPresenter->getObj(res.id)->getVertexes())[idForVert];
						auto colors = new std::vector<Vect4f*>();

						for (int i = 0; i < 3; ++i) {
							colors->push_back(new Vect4f());
							(*colors)[i]->setX(GetRValue(color));
							(*colors)[i]->setY(GetGValue(color));
							(*colors)[i]->setZ(GetBValue(color));
						}
						OGLB->setColors(colors);
						auto colorsForLog = genColorVect4f(colors);

						std::vector<ColorTexture*> *oldStruct = new std::vector<ColorTexture*>();
						oldStruct->push_back(
							new ColorTexture(
								genColorVect4f(OGLB->getColors()),
								OGLB->getPathToTexture(),
								OGLB->getTexture(),
								genColorVect2f(OGLB->getTextureCoord())
							)
						);

						ColorTexture * newStruct = new ColorTexture(colorsForLog, "", nullptr, nullptr);

						std::pair<ColorTexture*, std::vector<ColorTexture*>*> pair;
						pair.first = newStruct;
						pair.second = oldStruct;

						auto vect = new std::vector<OpenGL_Base*>();
						vect->push_back(OGLB);

						std::pair<std::vector<OpenGL_Base*>*, bool> curObj;
						curObj.first = vect;
						curObj.second = true;

						logger.log(&curObj, &pair, Logger::Set_Color);
					}
					else if (isOnlyOneVertex)//Работает
					{
						auto pair = getColorVect(DOPresenter, currentVertexes, res);

						if (pair.first != -1) {

							auto colors = pair.second->getColors();

							auto colorsForObj = genColorVect4f(colors);

							std::vector<ColorTexture*> *oldStruct = new std::vector<ColorTexture*>();
							oldStruct->push_back(
								new ColorTexture(
									genColorVect4f(pair.second->getColors()),
									pair.second->getPathToTexture(),
									pair.second->getTexture(),
									genColorVect2f(pair.second->getTextureCoord())
								)
							);

							if (colorsForObj == nullptr) {
								colorsForObj = new std::vector<Vect4f*>;
								for (int i = 0; i < 3; ++i)
									colorsForObj->push_back(new Vect4f(255,255,255));
							}

							(*colorsForObj)[pair.first]->setX(GetRValue(color));
							(*colorsForObj)[pair.first]->setY(GetGValue(color));
							(*colorsForObj)[pair.first]->setZ(GetBValue(color));

							pair.second->setColors(colorsForObj);
							//logger.log()
							auto colorsForLog = genColorVect4f(colorsForObj);

							ColorTexture * newStruct = new ColorTexture(colorsForLog, "", nullptr, nullptr);

							std::pair<ColorTexture*, std::vector<ColorTexture*>*> pairForLog;
							pairForLog.first = newStruct;
							pairForLog.second = oldStruct;

							auto vect = new std::vector<OpenGL_Base*>();
							vect->push_back(pair.second);

							std::pair<std::vector<OpenGL_Base*>*, bool> curObj;
							curObj.first = vect;
							curObj.second = true;

							logger.log(&curObj, &pairForLog, Logger::Set_Color);
						}
						else
							MessageBox(ModelCreator_hWnd, "You choose more than 1 vertex.\nChange the mode or select only one vertex", "Warning", MB_YESNO | MB_ICONQUESTION);
					}
					else
					{
						//Работает
						auto OGLB_vect = DOPresenter->getObj(res.id)->getVertexes();
						std::vector<ColorTexture*> *oldStruct = new std::vector<ColorTexture*>();
						std::vector<Vect4f*>* colorsForLog = nullptr;
						bool isFirst = true;

						for (auto it = OGLB_vect->begin(); it != OGLB_vect->end(); ++it) {
							auto colors = (*it)->getColors();
								oldStruct->push_back(
										new ColorTexture(
											genColorVect4f(colors),
											(*it)->getPathToTexture(),
											(*it)->getTexture(),
											genColorVect2f((*it)->getTextureCoord())
										)
									);

							auto colorsForObj = genColorVect4f(colors);
							
							if (colorsForObj == nullptr) {
								colorsForObj = new std::vector<Vect4f*>;
								for (int i = 0; i < 3; ++i)
									colorsForObj->push_back(new Vect4f());
							}

							for (int i = 0; i < 3; ++i) {
								(*colorsForObj)[i]->setX(GetRValue(color));
								(*colorsForObj)[i]->setY(GetGValue(color));
								(*colorsForObj)[i]->setZ(GetBValue(color));
							}

							if (isFirst) {
								colorsForLog = genColorVect4f(colorsForObj);
								isFirst = false;
							}

							(*it)->setColors(colorsForObj);
						}

						ColorTexture * newStruct = new ColorTexture(colorsForLog, "", nullptr, nullptr);

						std::pair<ColorTexture*, std::vector<ColorTexture*>*> pair;
						pair.first = newStruct;
						pair.second = oldStruct;

						std::pair<std::vector<OpenGL_Base*>*, bool> curObj;
						curObj.first = OGLB_vect;
						curObj.second = false;

						logger.log(&curObj, &pair, Logger::Set_Color);
					}
				}
			}

			//Map texture mode
			if (addVars.keys[VK_LBUTTON] == TRUE && isMapTextureModeOn && !isDrawModeOn &&
				canMapTexture && !addVars.keys[VK_SHIFT] && !addVars.keys[VK_SPACE])
			{
				addVars.keys[VK_LBUTTON] = FALSE;
				auto res = GraphicHelper::getSelectedObj(ModelCreator_hWnd,
					moveBtnsControl.getCamera(), DOPresenter, сolorMapForObjects);

				if (isThisObjSelect(res))
				{
					auto getColorVect = [](DrawableObjectsPresenter2 * DOPresenter,
						std::map<int, std::map<Vect3f*, vector3ub>>& currentVertexes,
						NameIndexId &res) -> Vect3f* {
						if (currentVertexes.size() == 1) {
							if (currentVertexes.begin()->second.size() == 1)
							{
								auto *vectPtr = currentVertexes.begin()->second.begin()->first;
								std::pair<int, Vect3f*> params = { res.id, vectPtr };
								auto pair = DOPresenter->getOJLBByIdAndPosVertPtr(params);
								auto vertPos = pair.second->getVertexes();

								int pos;
								for (int i = 0; i < vertPos->size(); ++i)
									if ((*vertPos)[i] == vectPtr) {
										pos = i;
										break;
									}

								return (*(pair.second->getColors()))[pos];
							}
						}
						return nullptr;
					};

					if (isOnlyForOGJB)
					{
						auto idForVert = GraphicHelper::getSelectedOpenGL_Base(ModelCreator_hWnd,
							moveBtnsControl.getCamera(), DOPresenter->getObj(res.id));

						auto OGLB = (*DOPresenter->getObj(res.id)->getVertexes())[idForVert];

						std::string filename;
						filename.resize(255);
						auto pair = getOFN(ModelCreator_hWnd, "All (*.*)\0*.*\0");

						if (pair.first != -1)
						{
							for (int i = 0; i < strlen(pair.second.lpstrFile); ++i) {
								if (pair.second.lpstrFile[i] == '\\')
									filename[i] = '/';
								else
									filename[i] = pair.second.lpstrFile[i];
							}

							GLuint *texture = new GLuint;
							if (LoadGLTextures(filename, texture) == 0)
							{
								isMapTextureModeOn = true;
								pairNameTexForObjs.first = filename;
								pairNameTexForObjs.second = texture;
								OGLB_ForMappedTexture.clear();
								
								OGLB_ForMappedTexture[res.id] = { {OGLB} };
								
								auto mapTexDlg = CreateDialogParam(ModelCreator_hInstance,
									MAKEINTRESOURCE(IDD_MAP_TEX_DLG),
									ModelCreator_hWnd, MappedTextureDlgProc, NULL);
								ShowWindow(mapTexDlg, TRUE);

								canMapTexture = false;
								isVertexesEnable = false;
							}
						}
						
					}
					else
					{
						std::string filename;
						filename.resize(255);
						auto pair = getOFN(ModelCreator_hWnd, "All (*.*)\0*.*\0");

						if (pair.first != -1)
						{
							for (int i = 0; i < strlen(pair.second.lpstrFile); ++i) {
								if (pair.second.lpstrFile[i] == '\\')
									filename[i] = '/';
								else
									filename[i] = pair.second.lpstrFile[i];
							}

							//GLuint texture[1];
							GLuint *texture = new GLuint;
							//char tempFilename[255]; 
							
							//strncpy_s(tempFilename, strlen(tempFilename), pair.second.lpstrFile, strlen(pair.second.lpstrFile));

							if (LoadGLTextures(filename, texture) == 0)		
							{
								isMapTextureModeOn = true;
								pairNameTexForObjs.first = filename;
								pairNameTexForObjs.second = texture;
								OGLB_ForMappedTexture.clear();

								for (auto it_name = currentObjs.begin(); it_name != currentObjs.end(); ++it_name)
									for (auto it_index = it_name->second.begin(); it_index != it_name->second.end(); ++it_index) {
										auto obj = DOPresenter->getObj(it_index->second);
										OGLB_ForMappedTexture[it_index->second] = {};

										for (auto it = obj->getVertexes()->begin(); it != obj->getVertexes()->end(); ++it)
											OGLB_ForMappedTexture[it_index->second].insert(*it);
									}
								auto mapTexDlg = CreateDialogParam(ModelCreator_hInstance,
									MAKEINTRESOURCE(IDD_MAP_TEX_DLG),
									ModelCreator_hWnd, MappedTextureDlgProc, NULL);
								ShowWindow(mapTexDlg, TRUE);

								canMapTexture = false;
								isVertexesEnable = false;
							}
						}
					}
				}
			}



			//при изменении позиции использую клавиатуру не обновляются значения в EditDlg

			//Select Object
			if (addVars.keys[VK_LBUTTON] == FALSE || addVars.keys[VK_SHIFT] == FALSE)
				canSelObj = true;
			if (addVars.keys[VK_LBUTTON] && addVars.keys[VK_SHIFT] && canSelObj)
			{
				canSelObj = false;
				auto res = GraphicHelper::getSelectedObj(ModelCreator_hWnd,
					moveBtnsControl.getCamera(), DOPresenter, сolorMapForObjects);

				if (isThisObjSelect(res))
				{
					currentObjs[res.name].erase(currentObjs[res.name].find(res.index));
					if (currentObjs[res.name].size() == 0)
						currentObjs.erase(res.name);

					if (currentVertexes.find(res.id) != currentVertexes.end())
						currentVertexes.erase(currentVertexes.find(res.id));

					int itemIndexInList = SendDlgItemMessage(dlg, IDC_LIST1, LB_FINDSTRING, 0, 
						(LPARAM)(std::string(res.name) + "#" + std::to_string(res.index)).c_str());

					SendDlgItemMessage(dlg, IDC_LIST1, LB_SETSEL, 
						(WPARAM)(BOOL)FALSE, (LPARAM)(UINT)itemIndexInList);
					
					//SendMessage(dlg, WM_COMMAND, MAKEWPARAM(IDC_LIST1, LBN_SELCHANGE), 0);

					//if (auto hDlg = GetWindow(ModelCreator_hWnd, IDD_DIALOG1))
						//SendMessage(hDlg, PD::WM_CHECHOBJDATA, 0, 0);

					if (elementsMap["EditDlg"]) {
						SendMessage(elementsMap["EditDlg"], PD::WM_CHECHOBJDATA, 0, 0);
						SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
					}
				} 
				else if (res.id != -1) 
				{
					if (!IsDlgButtonChecked(dlg, IDC_CHECK1)) {
						SendDlgItemMessage(dlg, IDC_LIST1, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);
						clearCurrentObjs();
						clearCurrentVertexes();
						/*
						if (currentVertexes.find(res.id) != currentVertexes.end())
							currentVertexes.erase(currentVertexes.find(res.id));
							*/
					}

					addObjToCurObjs(dlg, res.id);
					
					if (elementsMap["EditDlg"]) {
						SendMessage(elementsMap["EditDlg"], PD::WM_CHECHOBJDATA, 0, 0);
						SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
					}
				}
			}


			//Select Vertexes
			if (addVars.keys[VK_LBUTTON] == FALSE || addVars.keys[VK_SPACE] == FALSE)
				canSelVert = true;
			if (addVars.keys[VK_LBUTTON] && addVars.keys[VK_SPACE] && canSelVert)//addVars.keys[VK_LMENU])
			{
				canSelVert = false;
				auto pair = GraphicHelper::getSelectedVertex(ModelCreator_hWnd, 
					moveBtnsControl.getCamera(), DOPresenter, currentObjs, colorMapForVertexes);
				if (isThisVertSelect(pair))
				{
					if (currentVertexes[pair.first].find(pair.second) != currentVertexes[pair.first].end()) {
						currentVertexes[pair.first].erase(currentVertexes[pair.first].find(pair.second));
						if (currentVertexes[pair.first].size() == 0)
							currentVertexes.erase(currentVertexes.find(pair.first));
					}
					if (elementsMap.find("EditDlg") != elementsMap.end())
						SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
				}
				else if (pair.first != -1) 
				{
					if (!isMulVertexes) {
						//SendDlgItemMessage(dlg, IDC_LIST1, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);
						clearCurrentVertexes();
					}
					addVertToCurVerts(pair.first, pair.second);
					if (elementsMap.find("EditDlg") != elementsMap.end())
						SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
				}
			}


			//Select vertexes for link vertexes or objects
			if (addVars.keys[VK_RBUTTON] == FALSE || addVars.keys[VK_SHIFT] == FALSE)
				canLinkObjsThrowVert = true;
			if (addVars.keys[VK_RBUTTON] && addVars.keys[VK_SHIFT] && canLinkObjsThrowVert)//addVars.keys[VK_LMENU])
			{
				canLinkObjsThrowVert = false;
				auto pair = GraphicHelper::getSelectedVertex(ModelCreator_hWnd,
					moveBtnsControl.getCamera(), DOPresenter, currentObjs, colorMapForVertexes);
				if (!isThisVertSelect(pair) && pair.first != -1)
				{
					if (vertForLink.first == -1) 
					{
						vertForLink.first = pair.first;
						vertForLink.second = pair.second;
					}
					else 
					{
						auto delta = getDifBetweenVerts(pair, vertForLink);

						if (isMoveVertexes == true)
						{
							std::vector<Vect3f*> *vect = new std::vector<Vect3f*>();
							vect->push_back(vertForLink.second);
							logger.log(vect, &delta, Logger::Move_Verts);

							vertForLink.second->set(vertForLink.second->sum(delta));

							DOPresenter->getObj(vertForLink.first)->updateCoords();
							DOPresenter->getObj(vertForLink.first)->updateCube();

							vertForLink.first = -1;
							vertForLink.second = nullptr;
						}
						else
						{
							std::vector<DrawableObject2*> *vect = new std::vector<DrawableObject2*>();
							vect->push_back(DOPresenter->getObj(vertForLink.first));
							logger.log(vect, &delta, Logger::Move_Objs);

							DOPresenter->getObj(vertForLink.first)->updateCoords();
							DOPresenter->getObj(vertForLink.first)->updateCube();

							auto pos = DOPresenter->getObj(vertForLink.first)->getPosition();
							DOPresenter->getObj(vertForLink.first)->setPosition(pos->sum(delta));
							vertForLink.first = -1;
							vertForLink.second = nullptr;
						}

						//link mode в rotDlg
							//проверить, нужно ли делать invertedMatrix  (повертеть объект и почекать его координаты)
							
					}



					/*
					currentVertexes[pair.first].erase(currentVertexes[pair.first].find(pair.second));
					if (elementsMap.find("EditDlg") != elementsMap.end())
						SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
						*/
				}
			}



			if (addVars.keys['E'])
			{
				if (alreadyPressedE == false) {

					/*
					MCClassItemEditDlgProc::setNecessaryDatas(&logger, &moveBtnsControl,
						&addVars, &isMulVertexes, &isMoveVertexes, &alreadyPressedE);

					MCClassItemEditDlgProc::addPresenter("global", DOPresenter);
					MCClassItemEditDlgProc::addCurObjsMap("global", &currentObjs);
					MCClassItemEditDlgProc::addCurVertexesMap("global", &currentVertexes);

					std::vector<std::string> vect = {"ModelCreator"};
					MCClassItemEditDlgProc::setNecessaryIdentificators(vect);
					//
					*/
					auto rotDlg = CreateDialogParam(ModelCreator_hInstance,
						MAKEINTRESOURCE(IDD_ROTATION_DIALOG),
						
						ModelCreator_hWnd, ItemRotationDlgProc, NULL); //ItemRotationDlgProc //MCClassItemEditDlgProc::ItemEditDlgProc
					ShowWindow(rotDlg, TRUE);

					elementsMap["EditDlg"] = rotDlg;
					alreadyPressedE = true;
				}
			}


			if (addVars.keys['P'] == FALSE)
				pp = false;
			if (addVars.keys['P'] && currentObjs.size() != 0 && !pp) //Points
			{
				pp = true;
				isVertexesEnable = !isVertexesEnable;
			}
			


			if (addVars.keys['Z'] == FALSE)
				zp = false;
			if (addVars.keys[VK_CONTROL] && addVars.keys['Z'] && !zp)
			{
				zp = true;
				logger.backward();

			}

			if (addVars.keys['Y'] == FALSE)
				yp = false;
			if (addVars.keys[VK_CONTROL] && addVars.keys['Y'] && !yp)
			{
				yp = true;
				logger.forward();

			}

				
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((addVars.active && !DrawGLScene()) || addVars.keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done = TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(ModelCreator_hDC);					// Swap Buffers (Double Buffering)
				if (addVars.keys['L'] && !lp) // Клавиша 'L' нажата и не удерживается?
				{
					lp = TRUE;      // lp присвоили TRUE
					light = !light; // Переключение света TRUE/FALSE
				}
				if (!light)               // Если не свет
				{
					glDisable(GL_LIGHTING);  // Запрет освещения
				}
				else                      // В противном случае
				{
					glEnable(GL_LIGHTING);   // Разрешить освещение
				}
			}
			if (!addVars.keys['L']) // Клавиша 'L' Отжата?
			{
				lp = FALSE;      // Если так, то lp равно FALSE
			}

			objectsControl(moveBtnsControl, DOPresenter, currentObjs, &addVars.keys,
				addVars.wheelDelta, elementsMap, logger, IDC_EDIT_CAM_SPEED);//, currentObj, DOPresenter);
			cameraControl(moveBtnsControl, &addVars.keys, addVars.wheelDelta,
				elementsMap, IDC_EDIT_CAM_SPEED);



			//Удалить
			if (addVars.keys['O'])
			{
				POINT p;
				RECT r;
				GetClientRect(main_hWnd, &r);
				p.x = r.left;
				p.y = r.top;
				ClientToScreen(main_hWnd, &p);

				POINT pt;
				GetCursorPos(&pt);

				Vect2f mouseVect(pt.x, pt.y); // 444 213


				auto asd = GetCursor();

				const CameraLocation& cam = moveBtnsControl.getCamera(); // 545, 103
				//bool a = drawableObject->isSelected(cam, mouseVect, ModelCreator_hWnd);
			}



			//-------------------------------------
			


			if (addVars.keys['B'] && !bp)
			{
				bp = TRUE;
				blend = !blend;                  // Инвертируем blend 
				if (blend)                        // blend TRUE?
				{
					glEnable(GL_BLEND);        // Включаем смешивание
					glDisable(GL_DEPTH_TEST);  // Выключаем тест глубины
				}
				else
				{
					glDisable(GL_BLEND);        // Выключаем смешивание
					glEnable(GL_DEPTH_TEST);    // Включаем тест глубины
				}
			}
			if (!addVars.keys['B'])                    // ’B’ отжата?
			{
				bp = FALSE;                         // тогда bp возвращает ложь
			}

		}
	}
}
