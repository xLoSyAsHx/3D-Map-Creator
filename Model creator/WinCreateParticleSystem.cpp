#include "WinCreateParticleSystem.h"

AddVars WinCreateParticleSystem::addVars;
Logger WinCreateParticleSystem::logger;
ParticleInfo *WinCreateParticleSystem::particleInfo = new ParticleInfo;

HINSTANCE WinCreateParticleSystem::ParticleSysCreator_hInstance = NULL;
HWND WinCreateParticleSystem::ParticleSysCreator_hWnd = NULL;
HDC WinCreateParticleSystem::ParticleSysCreator_hDC = NULL;

MoveBtnsController *WinCreateParticleSystem::moveBtnsControl = new MoveBtnsController();
std::map<std::string, GLuint> WinCreateParticleSystem::modelsMap;
std::map<std::string, DrawableObject2*> WinCreateParticleSystem::drawableObjsMap;

DrawableObjectsPresenter2* WinCreateParticleSystem::ToDOPresenter = new DrawableObjectsPresenter2();
DrawableObjectsPresenter2* WinCreateParticleSystem::FromDOPresenter = new DrawableObjectsPresenter2();
ParticleSystem* WinCreateParticleSystem::particleSystem = nullptr;

std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> WinCreateParticleSystem::ñolorMapForObjects; // id - color
std::map<std::string, HWND> WinCreateParticleSystem::elementsMap;
std::map<std::string, std::map<int, int>> WinCreateParticleSystem::fromCurrentObjs;
std::map<std::string, std::map<int, int>> WinCreateParticleSystem::toCurrentObjs;
std::map<std::string, std::map<int, AreaForParticleSys>> *WinCreateParticleSystem::fromKoefsForParticleSys = 
	new std::map<std::string, std::map<int, AreaForParticleSys>>;
std::map<std::string, std::map<int, AreaForParticleSys>> *WinCreateParticleSystem::toKoefsForParticleSys = 
	new std::map<std::string, std::map<int, AreaForParticleSys>>;
std::map<
	DrawableObjectsPresenter2*,
	std::map<std::string, std::map<int, AreaForParticleSys>>> WinCreateParticleSystem::mapKoefsForPartSys;

bool WinCreateParticleSystem::haveSmthgNew = false;
std::string WinCreateParticleSystem::newObjName = "";
bool WinCreateParticleSystem::isAddToFrom = false;
bool WinCreateParticleSystem::alreadyPressedE = false;
bool WinCreateParticleSystem::alreadyPressedP = false;
GLfloat WinCreateParticleSystem::probability = 0.0f;
std::string WinCreateParticleSystem::particleName = "no particle";
bool WinCreateParticleSystem::isEditFromMapCreator = false;
ParticleSystem** WinCreateParticleSystem::particleSystemFromMapCreator;

std::vector<std::string> WinCreateParticleSystem::getDrObjKeys(std::map<std::string, DrawableObject2*> map)
{
	std::vector<std::string> keys;

	for (auto it = map.begin(); it != map.end(); ++it)
		keys.push_back(it->first);

	return keys;
}

bool WinCreateParticleSystem::isThisObjSelect(
	std::map<std::string, std::map<int, int>> &currentObjs, NameIndexId nii)
{
	if (currentObjs.find(nii.name) != currentObjs.end())
		if (currentObjs[nii.name].find(nii.index) != currentObjs[nii.name].end())
			return true;
	return false;
}

bool WinCreateParticleSystem::canCreateParticleSystem(ParticleInfo & particleInfo)
{
	if (particleInfo.particle == nullptr) {
		MessageBox(elementsMap["ObjDlg"], "You didn't choose particle", "Can't create particle system", MB_OK);
		return false;
	}

	if (particleInfo.speedOfOccur == 0) {
		MessageBox(elementsMap["ObjDlg"], "You didn't choose speed of particle occurence", "Can't create particle system", MB_OK);
		return false;
	}

	if (particleInfo.particleSpeed == 0) {
		MessageBox(elementsMap["ObjDlg"], "You didn't choose particle speed", "Can't create particle system", MB_OK);
		return false;
	}

	if (particleInfo.numOfParticles == 0) {
		MessageBox(elementsMap["ObjDlg"], "You didn't choose number of particles", "Can't create particle system", MB_OK);
		return false;
	}

	return true;
}

void WinCreateParticleSystem::saveModel()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = ParticleSysCreator_hWnd;
	ofn.lpstrFilter = "Model Creator Files (*.3drps)\0*.3drps\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrInitialDir = "C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Êóðñ 2\\OpenGL\\êóðñà÷\\Model creator\\Model creator\\Data\\Models";

	if (GetSaveFileName(&ofn))
	{
		std::ifstream isFileExist(ofn.lpstrFile, std::ios_base::in | std::ios_base::_Nocreate);
		if (isFileExist.is_open())
		{
			isFileExist.close();
			if (MessageBox(ParticleSysCreator_hWnd, "Model with such name already exist. Do you want to overwrite it?", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				//WindowHelper::KillGLWindow(ModelCreator_hWnd, ModelCreator_hInstance, ModelCreator_hDC, ModelCreator_hRC, "ModelCreator");
				//DestroyWindow(elementsMap["commonObjList"]);
				//EnableWindow(main_hWnd, TRUE);

				Saver::saveParticleSystem(ofn.lpstrFile, FromDOPresenter, ToDOPresenter,
					fromKoefsForParticleSys,
					toKoefsForParticleSys,
					*particleInfo, particleName);
			}
		}
		else
		{
			auto hFile = CreateFile(ofn.lpstrFile, FILE_ALL_ACCESS, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
				CREATE_NEW, 0, NULL);

			Saver::saveParticleSystem(ofn.lpstrFile, FromDOPresenter, ToDOPresenter, 
				fromKoefsForParticleSys,
				toKoefsForParticleSys,
				*particleInfo, particleName);
		}
		// Do something usefull with the filename stored in szFileName 
	}
	
}

void WinCreateParticleSystem::loadModel()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = ParticleSysCreator_hWnd;
	ofn.lpstrFilter = "Model Creator Files (*.3drps)\0*.3drps\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrInitialDir = "C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Êóðñ 2\\OpenGL\\êóðñà÷\\Model creator\\Model creator\\Data\\Models";

	if (GetOpenFileName(&ofn))
	{
		clearAllParticleSystemCreator();

		HWND fromList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
		HWND toList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
		HWND fromListProb = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM_PROB);
		HWND toListProb = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO_PROB);
		HWND editNumOfParts = GetDlgItem(elementsMap["ObjDlg"], IDC_EDIT_NUM_OF_PARTICLES);

		Loader::loadParticleSystem(ofn.lpstrFile,
			&logger,
			FromDOPresenter, ToDOPresenter,
			fromKoefsForParticleSys, toKoefsForParticleSys,
			*particleInfo, moveBtnsControl->getCamera(),
			ñolorMapForObjects, 
			fromList, toList, fromListProb, toListProb,
			editNumOfParts, particleName);
	}
	else
		MessageBox(ParticleSysCreator_hWnd, "Something went wrong", "Can't load particle system", MB_OK);
}

void WinCreateParticleSystem::Init()

{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	std::string fileName;
	fileName.resize(100);

	std::fstream fout("C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Êóðñ 2\\OpenGL\\êóðñà÷\\Model creator\\Model creator\\test.txt", std::ios_base::app);
	fout.close();

	hf = FindFirstFile("C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Êóðñ 2\\OpenGL\\êóðñà÷\\Model creator\\Model creator\\Data\\Models\\\*", &FindFileData);
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
							MessageBox(ParticleSysCreator_hWnd, "File was damaged", (std::string("Can't load object ") + fileName).c_str(), MB_OK);
						}
					}
				}
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}

	CreateModelBuildLists(modelsMap);

	BuildFont(ParticleSysCreator_hDC, base);
}

void WinCreateParticleSystem::clearAllParticleSystemCreator()
{
	HWND list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
	SendMessage(list, LB_RESETCONTENT, 0, 0);
	list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM_PROB);
	SendMessage(list, LB_RESETCONTENT, 0, 0);
	list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
	SendMessage(list, LB_RESETCONTENT, 0, 0);
	list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO_PROB);
	SendMessage(list, LB_RESETCONTENT, 0, 0);

	logger.clear();


	for (auto obj : ñolorMapForObjects)
		obj.second.clear();
	ñolorMapForObjects.clear();

	for (auto obj : fromCurrentObjs)
		obj.second.clear();
	fromCurrentObjs.clear();

	for (auto obj : toCurrentObjs)
		obj.second.clear();
	toCurrentObjs.clear();

	if (particleSystemFromMapCreator != nullptr)
	{
		for (auto obj : *fromKoefsForParticleSys)
			obj.second.clear();
		fromKoefsForParticleSys->clear();

		for (auto obj : *toKoefsForParticleSys)
			obj.second.clear();
		toKoefsForParticleSys->clear();

		for (auto map : mapKoefsForPartSys) {
			for (auto obj : map.second)
				obj.second.clear();
			map.second.clear();
		}
		mapKoefsForPartSys.clear();
	}

	for (int i = 0; i < 255; ++i)
		addVars.keys[i] = FALSE;

	haveSmthgNew = false;
	newObjName = "";
	probability = 0.0f;
	particleName = "no particle";

	drawableObjsMap.clear();

	if (particleSystem != nullptr && particleSystemFromMapCreator == nullptr) {
		delete particleSystem;
		particleSystem = nullptr;

		ToDOPresenter->clear();
		FromDOPresenter->clear();
	}
}

LRESULT WinCreateParticleSystem::ParticleSysCreatorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)									// Check For Windows Messages
	{

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

	case WM_GETMINMAXINFO: //Ïîëó÷èëè ñîîáùåíèå îò Âèíäû
	{
		MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
		POINT Min = { 640, 480 };
		POINT  Max = { 1000, 800 };
		pInfo->ptMinTrackSize = Min; // Óñòàíîâèëè ìèíèìàëüíûé ðàçìåð
		pInfo->ptMaxTrackSize = Max; // Óñòàíîâèëè ìàêñèìàëüíûé ðàçìåð
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

INT_PTR WinCreateParticleSystem::ItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND list;
	int selectedItem;
	char* text2 = nullptr;
	int lbItem, i;
	TCHAR LBString[128];
	std::vector<OpenGL_Base*> *obj;
	StructForCreateObj _struct;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND list = GetDlgItem(hDlg, IDC_LIST_ALL_ITEMS);

		std::vector<std::string> keys = getDrObjKeys(drawableObjsMap);
		for (auto key : keys)
			SendMessage(list, LB_ADDSTRING, 0, (LPARAM)key.c_str());

		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ADD:
		{
			list = GetDlgItem(hDlg, IDC_LIST_ALL_ITEMS);

			// Get selected index.
			lbItem = (int)SendMessage(list, LB_GETCURSEL, 0, 0);

			if (lbItem == -1)
				return TRUE;

			// Get item data.
			SendMessage(list, LB_GETTEXT, (WPARAM)lbItem, (LPARAM)LBString);

			if (particleInfo->particle != nullptr)
				delete particleInfo->particle;

			particleInfo->particle = DrawableObjectsPresenter2::genegateSimilarDrObj(
				drawableObjsMap[LBString]);

			particleName = LBString;

			InvalidateRect(elementsMap["EditParticleSysDlg"], NULL, TRUE);
			UpdateWindow(elementsMap["EditParticleSysDlg"]);

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
				list = GetDlgItem(hDlg, IDC_LIST_ALL_ITEMS);

				// Get selected index.
				i = (int)SendMessage(list, LB_GETCURSEL, 0, 0);

				SendMessage(list, LB_GETTEXT, (WPARAM)i, (LPARAM)LBString);
			}
			}
		}
		return TRUE;
		}
	}
	return FALSE;
}

INT_PTR WinCreateParticleSystem::RoundItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
			HWND list;
			HWND probList;
			auto koefsForParticleSys = fromKoefsForParticleSys;

			if (isAddToFrom) {
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM_PROB);
			}
			else
			{
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO_PROB);
				koefsForParticleSys = toKoefsForParticleSys;
			}

			Vect3f botPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_BOT_POS_X, IDC_EDIT_BOT_POS_Y, IDC_EDIT_BOT_POS_Z);

			GLfloat a_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_A_KOEF);
			GLfloat c_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_C_KOEF);
			int size = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_TRIANGLES);

			auto disk = genDisc(botPos, a_koef, c_koef, size);

			disk->setVertSize(0.05);
			//
			int i = 0;
			if (isAddToFrom) {
				i = FromDOPresenter->addObj(disk, "Disk", moveBtnsControl->getCamera());
				ñolorMapForObjects[FromDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}
			else {
				i = ToDOPresenter->addObj(disk, "Disk", moveBtnsControl->getCamera());
				ñolorMapForObjects[ToDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}

			//ñolorMapForObjects[][i] = getColorForSimpleDraw(ñolorMapForObjects);

			newObjName = std::string("Disk") + "#" + std::to_string(i);

			SendMessage(list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
			SendMessage(probList, LB_ADDSTRING, 0, (LPARAM)"0.00");

			if (koefsForParticleSys->find("Disk") == koefsForParticleSys->end())
				(*koefsForParticleSys)["Disk"] = {};

			std::pair<std::string, int> nameIndex;

			if (isAddToFrom)
				nameIndex = FromDOPresenter->getNameIndexById(i);
			else
				nameIndex = ToDOPresenter->getNameIndexById(i);

			AreaForParticleSys area;
			area.type = "Disk";
			area.a = a_koef;
			area.b = 0;
			area.c = c_koef;
			area.areaPos = *disk->getPosition();
			area.areaRot = disk->getRotation();

			if (isAddToFrom)
				mapKoefsForPartSys[FromDOPresenter]["Disk"][nameIndex.second] = area;
			else
				mapKoefsForPartSys[ToDOPresenter]["Disk"][nameIndex.second] = area;

			(*koefsForParticleSys)["Disk"][nameIndex.second] = area;

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl->getCamera();
			_struct.index = i;
			_struct.objName = "Disk";
			_struct.presenterName = isAddToFrom ? "from" : "to";

			logger.log(disk, &_struct, Logger::Create_Obj);

			return TRUE;
		}

		case ID_CREATE_CONE:
		{
			HWND list;
			HWND probList;
			auto koefsForParticleSys = fromKoefsForParticleSys;

			if (isAddToFrom) {
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM_PROB);
			}
			else
			{
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO_PROB);
				koefsForParticleSys = toKoefsForParticleSys;
			}

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
			int i = 0;
			if (isAddToFrom) {
				i = FromDOPresenter->addObj(cone, "Cone", moveBtnsControl->getCamera());
				ñolorMapForObjects[FromDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}
			else {
				i = ToDOPresenter->addObj(cone, "Cone", moveBtnsControl->getCamera());
				ñolorMapForObjects[ToDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}

			if (koefsForParticleSys->find("Cone") == koefsForParticleSys->end())
				(*koefsForParticleSys)["Cone"] = {};

			std::pair<std::string, int> nameIndex;

			if (isAddToFrom)
				nameIndex = FromDOPresenter->getNameIndexById(i);
			else
				nameIndex = ToDOPresenter->getNameIndexById(i);

			AreaForParticleSys area;
			area.type = "Cone";
			area.a = a_koef;
			area.b = topPos.getY() - botPos.getY();
			area.c = c_koef;
			area.areaPos = *cone->getPosition();
			area.areaRot = cone->getRotation();


			(*koefsForParticleSys)["Cone"][nameIndex.second] = area;

			newObjName = std::string("Cone") + "#" + std::to_string(i);

			SendMessage(list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
			SendMessage(probList, LB_ADDSTRING, 0, (LPARAM)"0.00");

			if (koefsForParticleSys->find("Disk") == koefsForParticleSys->end())
				(*koefsForParticleSys)["Disk"] = {};

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl->getCamera();
			_struct.index = i;
			_struct.objName = "Cone";
			_struct.presenterName = isAddToFrom ? "from" : "to";

			logger.log(cone, &_struct, Logger::Create_Obj);

			return TRUE;
		}

		case ID_CREATE_CYLINDER:
		{
			HWND list;
			HWND probList;
			auto koefsForParticleSys = fromKoefsForParticleSys;

			if (isAddToFrom) {
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM_PROB);
			}
			else
			{
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO_PROB);
				koefsForParticleSys = toKoefsForParticleSys;
			}

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
			int i = 0;
			if (isAddToFrom) {
				i = FromDOPresenter->addObj(cylinder, "Cylinder", moveBtnsControl->getCamera());
				ñolorMapForObjects[FromDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}
			else {
				i = ToDOPresenter->addObj(cylinder, "Cylinder", moveBtnsControl->getCamera());
				ñolorMapForObjects[ToDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}

			if (koefsForParticleSys->find("Cylinder") == koefsForParticleSys->end())
				(*koefsForParticleSys)["Cylinder"] = {};

			std::pair<std::string, int> nameIndex;

			if (isAddToFrom)
				nameIndex = FromDOPresenter->getNameIndexById(i);
			else
				nameIndex = ToDOPresenter->getNameIndexById(i);

			AreaForParticleSys area;
			area.type = "Cylinder";
			area.a = a_koef;
			area.b = topPos.getY() - botPos.getY();
			area.c = c_koef;
			area.areaPos = *cylinder->getPosition();
			area.areaRot = cylinder->getRotation();


			(*koefsForParticleSys)["Cylinder"][nameIndex.second] = area;

			newObjName = std::string("Cylinder") + "#" + std::to_string(i);

			SendMessage(list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
			SendMessage(probList, LB_ADDSTRING, 0, (LPARAM)"0.00");

			if (koefsForParticleSys->find("Disk") == koefsForParticleSys->end())
				(*koefsForParticleSys)["Disk"] = {};

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl->getCamera();
			_struct.index = i;
			_struct.objName = "Cylinder";
			_struct.presenterName = isAddToFrom ? "from" : "to";

			logger.log(cylinder, &_struct, Logger::Create_Obj);

			return TRUE;
		}

		case ID_CREATE_ELLIPSOID:
		{
			HWND list;
			HWND probList;
			auto koefsForParticleSys = fromKoefsForParticleSys;

			if (isAddToFrom) {
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM_PROB);
			}
			else
			{
				list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
				probList = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO_PROB);
				koefsForParticleSys = toKoefsForParticleSys;
			}

			Vect3f botPos = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_BOT_POS_X, IDC_EDIT_BOT_POS_Y, IDC_EDIT_BOT_POS_Z);

			GLfloat a_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_A_KOEF);
			GLfloat b_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_B_KOEF);
			GLfloat c_koef = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_C_KOEF);
			GLfloat size = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_TRIANGLES);

			auto ellipsoid = genEllipsoid(botPos, a_koef, b_koef, c_koef, size);

			ellipsoid->setVertSize(0.05);
			//
			int i = 0;
			if (isAddToFrom) {
				i = FromDOPresenter->addObj(ellipsoid, "Ellipsoid", moveBtnsControl->getCamera());
				ñolorMapForObjects[FromDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}
			else {
				i = ToDOPresenter->addObj(ellipsoid, "Ellipsoid", moveBtnsControl->getCamera());
				ñolorMapForObjects[ToDOPresenter][i] = DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);
			}

			if (koefsForParticleSys->find("Ellipsoid") == koefsForParticleSys->end())
				(*koefsForParticleSys)["Ellipsoid"] = {};

			std::pair<std::string, int> nameIndex;

			if (isAddToFrom)
				nameIndex = FromDOPresenter->getNameIndexById(i);
			else
				nameIndex = ToDOPresenter->getNameIndexById(i);

			AreaForParticleSys area;
			area.type = "Ellipsoid";
			area.a = a_koef;
			area.b = b_koef;
			area.c = c_koef;
			area.areaPos = *ellipsoid->getPosition();
			area.areaRot = ellipsoid->getRotation();


			(*koefsForParticleSys)["Ellipsoid"][nameIndex.second] = area;

			newObjName = std::string("Ellipsoid") + "#" + std::to_string(i);

			SendMessage(list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
			SendMessage(probList, LB_ADDSTRING, 0, (LPARAM)"0.00");

			if (koefsForParticleSys->find("Disk") == koefsForParticleSys->end())
				(*koefsForParticleSys)["Disk"] = {};

			StructForCreateObj _struct;
			_struct.cam = moveBtnsControl->getCamera();
			_struct.index = i;
			_struct.objName = "Ellipsoid";
			_struct.presenterName = isAddToFrom ? "from" : "to";

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

INT_PTR WinCreateParticleSystem::ItemRotationDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static std::map<int, Vect3f> objsRotations;
	static std::map<int, Vect3f> objsPositions;
	static std::map<DrawableObject2*, Vect3f> objsScales;
	static Vect3f pos;
	static Vect3f rot;
	static bool isOnlyOneObj;
	int selectedItem;
	char* text2 = nullptr;
	int lbItem, i;
	LPTSTR text = new TCHAR[20];

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
		std::map<int, Vect3f> &objsPositions, bool &isOnlyOneObj,
		std::map<std::string, std::map<int, int>> &fromCurrentObjs,
		std::map<std::string, std::map<int, int>> &toCurrentObjs) {
		int i = 0;
		objsPositions.clear();
		objsRotations.clear();


		std::vector<std::map<std::string, std::map<int, int>>> currentObjs = { 
			fromCurrentObjs,
			toCurrentObjs 
		};
		for (auto it_obj = currentObjs.begin(); it_obj != currentObjs.end(); ++it_obj)
			for (auto it_name = it_obj->begin(); it_name != it_obj->end(); ++it_name)
				for (auto it = (*it_obj)[it_name->first].begin(); it != (*it_obj)[it_name->first].end(); ++it) {
					i++;

					DrawableObject2 *drObj;
					if (it_obj - currentObjs.begin() == 0)
						drObj = FromDOPresenter->getObj(it->second);
					else
						drObj = ToDOPresenter->getObj(it->second);

					objsRotations[it->second] = drObj->getRotation();
					objsPositions[it->second] = *drObj->getPosition();
				}

		if (i == 1) {
			pos = objsPositions.begin()->second;
			rot = objsRotations.begin()->second;
			isOnlyOneObj = true;
		}
		else
			isOnlyOneObj = false;
	};

	static auto updateCurObjsScale = [](bool &isOnlyOneObj,
		DrawableObjectsPresenter2* FromDOPresenter,
		DrawableObjectsPresenter2* ToDOPresenter,
		std::map<std::string, std::map<int, int>> &fromCurrentObjs,
		std::map<std::string, std::map<int, int>> &toCurrentObjs) {

		objsScales.clear();

		std::vector<std::map<std::string, std::map<int, int>>> currentObjs = {
			fromCurrentObjs,
			toCurrentObjs
		};
		for (auto it_obj = currentObjs.begin(); it_obj != currentObjs.end(); ++it_obj)
			for (auto it_id = it_obj->begin(); it_id != it_obj->end(); ++it_id)
				for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it)
				{
					DrawableObject2 *obj;
					if (it_obj - currentObjs.begin() == 0)
						obj = FromDOPresenter->getObj(it->second);
					else
						obj = ToDOPresenter->getObj(it->second);

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

		RECT rect;
		GetWindowRect(hDlg, &rect);
		SetWindowPos(hDlg, HWND_TOP, 0, 0, rect.right - rect.left, 230, SWP_SHOWWINDOW | SWP_NOMOVE);


		updateCurObjsData(objsRotations, objsPositions, isOnlyOneObj, 
			fromCurrentObjs, toCurrentObjs);
		updateCurObjsScale(isOnlyOneObj, FromDOPresenter, ToDOPresenter,
			fromCurrentObjs, toCurrentObjs);


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


		return TRUE;
	}

	case PD::WM_CHECHOBJDATA:
	{
		updateCurObjsData(objsRotations, objsPositions, isOnlyOneObj,
			fromCurrentObjs, toCurrentObjs);
		updateCurObjsScale(isOnlyOneObj, FromDOPresenter, ToDOPresenter,
			fromCurrentObjs, toCurrentObjs);

		setDefault(hDlg, pos, isOnlyOneObj, 
			IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, ID_SET_DEFAULT_POS);
		setDefault(hDlg, rot, isOnlyOneObj, 
			IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, ID_SET_DEFAULT_ROT);

		if (isOnlyOneObj)
			setDefault(hDlg, objsScales.begin()->second, true, 
				IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
		else
			setDefault(hDlg, Vect3f(1, 1, 1), true, 
				IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);

		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_APPLY_ROT:
		{
			Vect3f rotVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z);

			std::vector<DrawableObject2*> *drObjVectForLog = new std::vector<DrawableObject2*>();

			std::vector<std::map<std::string, std::map<int, int>>> currentObjs = {
				fromCurrentObjs,
				toCurrentObjs
			};
			for (auto it_obj = currentObjs.begin(); it_obj != currentObjs.end(); ++it_obj)
				for (auto it_name = it_obj->begin(); it_name != it_obj->end(); ++it_name)
					for (auto it = (*it_obj)[it_name->first].begin(); it != (*it_obj)[it_name->first].end(); ++it) {
					
						DrawableObject2 *drObj;
						if (it_obj - currentObjs.begin() == 0)
							drObj = FromDOPresenter->getObj(it->second);
						else
							drObj = ToDOPresenter->getObj(it->second);

						if (isOnlyOneObj)
							drObj->setRotation(rotVect);
						else
							drObj->setRotation(objsRotations[it->second].sum(rotVect));

						drObjVectForLog->push_back(drObj);


						drObj->updateCoords();
						drObj->updateCube();
					}

			logger.log(drObjVectForLog, &rotVect, Logger::Rot_Objs);

			return TRUE;
		}

		case ID_APPLY_POS:
		{
			Vect3f posVect = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z);

			if (posVect.getX() == 0)
				std::cout << 1;

			std::vector<std::map<std::string, std::map<int, int>>> currentObjs = {
				fromCurrentObjs,
				toCurrentObjs
			};
			for (auto it_obj = currentObjs.begin(); it_obj != currentObjs.end(); ++it_obj)
				for (auto it_name = it_obj->begin(); it_name != it_obj->end(); ++it_name)
					for (auto it = (*it_obj)[it_name->first].begin(); it != (*it_obj)[it_name->first].end(); ++it) {

						DrawableObject2 *drObj;
						if (it_obj - currentObjs.begin() == 0)
							drObj = FromDOPresenter->getObj(it->second);
						else
							drObj = ToDOPresenter->getObj(it->second);

						if (isOnlyOneObj)
							drObj->setPosition(posVect);
						else
							drObj->setPosition(objsPositions[it->second].sum(posVect));

						drObj->updateCoords();
						drObj->updateCube();
					}

			return TRUE;
		}

		case ID_APPLY_SCAL:
		{
			Vect3f newScalForLog = DlgHelper::getValFromEdits(hDlg, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z);
			Vect3f oldScal;

			if (toCurrentObjs.size() != 0 && toCurrentObjs.begin()->second.size() != 0 ||
				fromCurrentObjs.size() != 0 && fromCurrentObjs.begin()->second.size() != 0) {

			}
			else
				return TRUE;

			auto cam = moveBtnsControl->getCamera();
			std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>
				*globMap = new std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>();

			bool isScalDet = false;

			std::vector<std::map<std::string, std::map<int, int>>> currentObjs = {
				fromCurrentObjs,
				toCurrentObjs
			};
			for (auto it_obj = currentObjs.begin(); it_obj != currentObjs.end(); ++it_obj)
				for (auto it_name = it_obj->begin(); it_name != it_obj->end(); ++it_name)
					for (auto it_pair = it_name->second.begin(); it_pair != it_name->second.end(); ++it_pair)
				{
					DrawableObject2 *drObj;
					if (it_obj - currentObjs.begin() == 0)
						drObj = FromDOPresenter->getObj(it_pair->second);
					else
						drObj = ToDOPresenter->getObj(it_pair->second);

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
			}

			for (auto it_obj = currentObjs.begin(); it_obj != currentObjs.end(); ++it_obj)
				for (auto it_name = it_obj->begin(); it_name != it_obj->end(); ++it_name)
					for (auto it_pair = it_name->second.begin(); it_pair != it_name->second.end(); ++it_pair) {

						auto area = &(*fromKoefsForParticleSys)[it_name->first][it_pair->first];
						auto baseArea = &mapKoefsForPartSys[FromDOPresenter][it_name->first][it_pair->first];

						if (it_obj - currentObjs.begin() != 0) {
							area = &(*toKoefsForParticleSys)[it_name->first][it_pair->first];
							baseArea = &mapKoefsForPartSys[ToDOPresenter][it_name->first][it_pair->first];
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

			for (auto it_map = globMap->begin(); it_map != globMap->end(); ++it_map) {
				auto drObj = it_map->first;

				drObj->updateCoords();
				drObj->updateCube();
			}

			logger.log(globMap, &MapOldNewScal(shifts, oldScal, newScalForLog), Logger::Scal_Objs);

			return TRUE;
		}

		case ID_SET_DEFAULT_ROT:
		{
			setDefault(hDlg, rot, isOnlyOneObj, 
				IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z, ID_SET_DEFAULT_ROT);

			return TRUE;
		}

		case ID_SET_DEFAULT_POS:
		{
			setDefault(hDlg, pos, isOnlyOneObj, IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z, ID_SET_DEFAULT_POS);

			return TRUE;
		}

		case ID_SET_DEFAULT_SCAL:
		{
			if (isOnlyOneObj)
				setDefault(hDlg, objsScales.begin()->second, true, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);
			else
				setDefault(hDlg, Vect3f(1, 1, 1), true, IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z, NULL);


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

INT_PTR WinCreateParticleSystem::EditParticleSysDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int selectedItem;
	char* text2 = nullptr;
	int lbItem, i;
	static bool isFirst = false;
	LPTSTR text = new TCHAR[10];
	HDC testDC;
	PAINTSTRUCT ps;
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	

	static auto setDefault = [](HWND hDlg, Vect3f &vect, bool isDefault, int e1, int e2, int e3, int step) {
		if (!isDefault) {
			SendDlgItemMessage(hDlg, e1, WM_SETTEXT, 0, (LPARAM)std::to_string(vect.get('x')).c_str());
			SendDlgItemMessage(hDlg, e2, WM_SETTEXT, 0, (LPARAM)std::to_string(vect.get('y')).c_str());
			SendDlgItemMessage(hDlg, e3, WM_SETTEXT, 0, (LPARAM)std::to_string(vect.get('z')).c_str());
		}
		else {
			SendDlgItemMessage(hDlg, e1, WM_SETTEXT, 0, (LPARAM)"0.0");
			SendDlgItemMessage(hDlg, e2, WM_SETTEXT, 0, (LPARAM)"0.0");
			SendDlgItemMessage(hDlg, e3, WM_SETTEXT, 0, (LPARAM)"0.0");
		}

		SendDlgItemMessage(hDlg, step, WM_SETTEXT, 0, (LPARAM)"0.01");
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

	switch (message)
	{
	case WM_INITDIALOG:
	{
		addVars.keys['P'] = FALSE;
		isFirst = true;

		setDefault(hDlg, particleInfo->particleFluct, false,
			IDC_EDIT_PART_FLUCT_X, IDC_EDIT_PART_FLUCT_Y, IDC_EDIT_PART_FLUCT_Z, 
			IDC_EDIT_PART_FLUCT_STEP);

		setDefault(hDlg, particleInfo->particleFluctLen, false,
			IDC_EDIT_PART_FLUCT_LEN_X, IDC_EDIT_PART_FLUCT_LEN_Y, IDC_EDIT_PART_FLUCT_LEN_Z,
			IDC_EDIT_PART_FLUCT_LEN_STEP);

		setDefault(hDlg, particleInfo->particleRot, false,
			IDC_EDIT_PART_ROT_X, IDC_EDIT_PART_ROT_Y, IDC_EDIT_PART_ROT_Z, 
			IDC_EDIT_PART_ROT_STEP);

		SendDlgItemMessage(hDlg, IDC_EDIT_PART_SPEED, WM_SETTEXT,
			0, (LPARAM)std::to_string(particleInfo->particleSpeed).c_str());

		SendDlgItemMessage(hDlg, IDC_EDIT_PART_GEN_SPEED, WM_SETTEXT,
			0, (LPARAM)std::to_string(particleInfo->speedOfOccur).c_str());

		SendDlgItemMessage(hDlg, IDC_EDIT_PART_FLUCT_LEN_STEP, WM_SETTEXT,
			0, (LPARAM)"0.1");

		return TRUE;
	}

	case WM_PAINT:
	{
		testDC = BeginPaint(hDlg, &ps);
		SelectObject(testDC, font);


		//215, 270    405, 270
		if (particleName.length() < 23)
			i = ((405 - 218) - particleName.length()  * ((405 - 218) / 32)) / 2;
		::TextOut(testDC, 218 + i, 270, particleName.c_str(), particleName.length());
		EndPaint(hDlg, &ps);

		if (isFirst) {
			isFirst = false;
			InvalidateRect(elementsMap["EditParticleSysDlg"], NULL, TRUE);
			UpdateWindow(elementsMap["EditParticleSysDlg"]);
		}

		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_APPLY_PART_FLUCT:
		{
			auto fluct = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_PART_FLUCT_X, IDC_EDIT_PART_FLUCT_Y, IDC_EDIT_PART_FLUCT_Z);

			particleInfo->particleFluct.set(fluct);

			return TRUE;
		}

		case ID_APPLY_PART_FLUCT_LEN:
		{
			auto fluctLen = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_PART_FLUCT_LEN_X, IDC_EDIT_PART_FLUCT_LEN_Y, IDC_EDIT_PART_FLUCT_LEN_Z);

			particleInfo->particleFluctLen.set(fluctLen);

			return TRUE;
		}

		case ID_APPLY_PART_ROT:
		{
			auto rot = DlgHelper::getValFromEdits(hDlg,
				IDC_EDIT_PART_ROT_X, IDC_EDIT_PART_ROT_Y, IDC_EDIT_PART_ROT_Z);

			particleInfo->particleRot.set(rot);

			return TRUE;
		}

		case ID_APPLY_PART_SPEED:
		{
			particleInfo->particleSpeed = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_PART_SPEED);

			return TRUE;
		}

		case ID_APPLY_PART_GEN_SPEED:
		{
			particleInfo->speedOfOccur = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_PART_GEN_SPEED);

			return TRUE;
		}

		case ID_SET_DEFAULT_PART_FLUCT:
		{
			setDefault(hDlg, Vect3f(), false, 
				IDC_EDIT_PART_FLUCT_X, IDC_EDIT_PART_FLUCT_Y, IDC_EDIT_PART_FLUCT_Z,
				IDC_EDIT_PART_FLUCT_STEP);

			return TRUE;
		}

		case ID_SET_DEFAULT_PART_FLUCT_LEN:
		{
			setDefault(hDlg, Vect3f(), false,
				IDC_EDIT_PART_FLUCT_LEN_X, IDC_EDIT_PART_FLUCT_LEN_Y, IDC_EDIT_PART_FLUCT_LEN_Z,
				IDC_EDIT_PART_FLUCT_LEN_STEP);

			return TRUE;
		}

		case ID_SET_DEFAULT_PART_ROT:
		{
			setDefault(hDlg, Vect3f(), false,
				IDC_EDIT_PART_ROT_X, IDC_EDIT_PART_ROT_Y, IDC_EDIT_PART_ROT_Z,
				IDC_EDIT_PART_ROT_STEP);

			return TRUE;
		}

		case IDC_B_CHOOSE_PARTICLE:
		{
			DialogBox(ParticleSysCreator_hInstance,
				MAKEINTRESOURCE(IDD_ADD_ITEM_DLG),
				hDlg, (DLGPROC)ItemsAddDlgProc);

			return TRUE;
		}

		case IDC_BUTTON_PART_FLUCT_UP_X:
		case IDC_BUTTON_PART_FLUCT_DOWN_X:
		case IDC_BUTTON_PART_FLUCT_UP_Y:
		case IDC_BUTTON_PART_FLUCT_DOWN_Y:
		case IDC_BUTTON_PART_FLUCT_UP_Z:
		case IDC_BUTTON_PART_FLUCT_DOWN_Z:
		{
			int e = LOWORD(wParam);

			if (e == IDC_BUTTON_PART_FLUCT_UP_X || e == IDC_BUTTON_PART_FLUCT_DOWN_X)
				changeData(hDlg, wParam, 
					IDC_EDIT_PART_FLUCT_X, IDC_EDIT_PART_FLUCT_STEP, IDC_BUTTON_PART_FLUCT_DOWN_X,
					ID_APPLY_PART_FLUCT);
			if (e == IDC_BUTTON_PART_FLUCT_UP_Y || e == IDC_BUTTON_PART_FLUCT_DOWN_Y)
				changeData(hDlg, wParam, 
					IDC_EDIT_PART_FLUCT_Y, IDC_EDIT_PART_FLUCT_STEP, IDC_BUTTON_PART_FLUCT_DOWN_Y,
					ID_APPLY_PART_FLUCT);
			if (e == IDC_BUTTON_PART_FLUCT_UP_Z || e == IDC_BUTTON_PART_FLUCT_DOWN_Z)
				changeData(hDlg, wParam, 
					IDC_EDIT_PART_FLUCT_Z, IDC_EDIT_PART_FLUCT_STEP, IDC_BUTTON_PART_FLUCT_DOWN_Z,
					ID_APPLY_PART_FLUCT);

			return TRUE;
		}

		case IDC_BUTTON_PART_FLUCT_LEN_UP_X:
		case IDC_BUTTON_PART_FLUCT_LEN_DOWN_X:
		case IDC_BUTTON_PART_FLUCT_LEN_UP_Y:
		case IDC_BUTTON_PART_FLUCT_LEN_DOWN_Y:
		case IDC_BUTTON_PART_FLUCT_LEN_UP_Z:
		case IDC_BUTTON_PART_FLUCT_LEN_DOWN_Z:
		{
			int e = LOWORD(wParam);

			if (e == IDC_BUTTON_PART_FLUCT_LEN_UP_X || e == IDC_BUTTON_PART_FLUCT_LEN_DOWN_X)
				changeData(hDlg, wParam,
					IDC_EDIT_PART_FLUCT_LEN_X, IDC_EDIT_PART_FLUCT_LEN_STEP, 
					IDC_BUTTON_PART_FLUCT_LEN_DOWN_X, ID_APPLY_PART_FLUCT_LEN);
			if (e == IDC_BUTTON_PART_FLUCT_LEN_UP_Y || e == IDC_BUTTON_PART_FLUCT_LEN_DOWN_Y)
				changeData(hDlg, wParam,
					IDC_EDIT_PART_FLUCT_LEN_Y, IDC_EDIT_PART_FLUCT_LEN_STEP,
					IDC_BUTTON_PART_FLUCT_LEN_DOWN_Y, ID_APPLY_PART_FLUCT_LEN);
			if (e == IDC_BUTTON_PART_FLUCT_LEN_UP_Z || e == IDC_BUTTON_PART_FLUCT_LEN_DOWN_Z)
				changeData(hDlg, wParam,
					IDC_EDIT_PART_FLUCT_LEN_Z, IDC_EDIT_PART_FLUCT_LEN_STEP, 
					IDC_BUTTON_PART_FLUCT_LEN_DOWN_Z, ID_APPLY_PART_FLUCT_LEN);

			return TRUE;
		}

		case IDC_BUTTON_PART_ROT_UP_X:
		case IDC_BUTTON_PART_ROT_DOWN_X:
		case IDC_BUTTON_PART_ROT_UP_Y:
		case IDC_BUTTON_PART_ROT_DOWN_Y:
		case IDC_BUTTON_PART_ROT_UP_Z:
		case IDC_BUTTON_PART_ROT_DOWN_Z:
		{
			int e = LOWORD(wParam);

			if (e == IDC_BUTTON_PART_ROT_UP_X || e == IDC_BUTTON_PART_ROT_DOWN_X)
				changeData(hDlg, wParam, 
					IDC_EDIT_PART_ROT_X, IDC_EDIT_PART_ROT_STEP, 
					IDC_BUTTON_PART_ROT_DOWN_X,	ID_APPLY_PART_ROT);
			if (e == IDC_BUTTON_PART_ROT_UP_Y || e == IDC_BUTTON_PART_ROT_DOWN_Y)
				changeData(hDlg, wParam, 
					IDC_EDIT_PART_ROT_Y, IDC_EDIT_PART_ROT_STEP, 
					IDC_BUTTON_PART_ROT_DOWN_Y, ID_APPLY_PART_ROT);
			if (e == IDC_BUTTON_PART_ROT_UP_Z || e == IDC_BUTTON_PART_ROT_DOWN_Z)
				changeData(hDlg, wParam, 
					IDC_EDIT_PART_ROT_Z, IDC_EDIT_PART_ROT_STEP,
					IDC_BUTTON_PART_ROT_DOWN_Z, ID_APPLY_PART_ROT);

			return TRUE;
		}


		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			alreadyPressedP = false;
		}
		return TRUE;

	}
	//return DefWindowProc(hDlg, message, wParam, lParam);
	return FALSE;
}

INT_PTR WinCreateParticleSystem::SetProbabilityDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND edit;

	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_APPLY_PROBABILITY: {
			probability = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_FOR_PROBABLITY);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		}
		return TRUE;
	}
	case WM_CLOSE: {
		probability = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_FOR_PROBABLITY);
		EndDialog(hDlg, LOWORD(wParam));
	}


	return TRUE;
	}

	return FALSE;
}

LRESULT WinCreateParticleSystem::ParticleSysDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	static LPTSTR LBString = new TCHAR[80];
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	char* buf = nullptr;
	char* objName = nullptr;
	char* num = nullptr;
	int intNum;
	int objId;

	if (haveSmthgNew == true)
	{
		haveSmthgNew = false;
		HWND list;
		if (isAddToFrom)
			list = GetDlgItem(hDlg, IDC_LIST_FROM);
		else
			list = GetDlgItem(hDlg, IDC_LIST_TO);

		SendMessage(list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
	}

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SendDlgItemMessage(hDlg, , WM_SETTEXT, 0, (LPARAM)"20");
		DlgHelper::setValForEdit(hDlg, IDC_EDIT_CAMERA_SPEED, 20);

		if (particleSystemFromMapCreator != nullptr) {
			HWND item = GetDlgItem(hDlg, IDC_BUTTON_LOAD_PART_SYS);
			ShowWindow(item, FALSE);
			item = GetDlgItem(hDlg, IDC_BUTTON_DELETE);
			ShowWindow(item, FALSE);
			item = GetDlgItem(hDlg, IDC_BUTTON_ADD_TO_TO);
			ShowWindow(item, FALSE);
			item = GetDlgItem(hDlg, IDC_BUTTON_ADD_TO_FROM);
			ShowWindow(item, FALSE);


			RECT rect;
			item = GetDlgItem(hDlg, IDC_BUTTON_SET_PROBABIL);
			GetWindowRect(item, &rect);
			SetWindowPos(item, HWND_TOP, rect.left - 100, rect.top - 60, rect.right - rect.left + 120, rect.bottom - rect.top, SWP_SHOWWINDOW);

			item = GetDlgItem(hDlg, IDC_BUTTON_SAVE_PART_SYS);
			SetWindowPos(item, HWND_TOP, 0, 0, rect.right - rect.left + 120, rect.bottom - rect.top, SWP_SHOWWINDOW | SWP_NOMOVE);

			item = GetDlgItem(hDlg, IDC_CHECK_MULTIPLE_SEL);
			SetWindowPos(item, HWND_TOP, 242, 82, rect.right - rect.left + 120, rect.bottom - rect.top, SWP_SHOWWINDOW);
		}

		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ADD_TO_FROM:
		{
			isAddToFrom = true;
			DialogBox(ParticleSysCreator_hInstance,
				MAKEINTRESOURCE(IDD_ADD_ROUND_OBJS_DLG),
				hDlg, (DLGPROC)RoundItemsAddDlgProc);

			return TRUE;
		}

		case IDC_BUTTON_ADD_TO_TO:
		{
			isAddToFrom = false;
			DialogBox(ParticleSysCreator_hInstance,
				MAKEINTRESOURCE(IDD_ADD_ROUND_OBJS_DLG),
				hDlg, (DLGPROC)RoundItemsAddDlgProc);

			return TRUE;
		}

		case IDC_LIST_FROM:
		case IDC_LIST_TO:
		{
			switch (HIWORD(wParam))
			{

			case LBN_SELCHANGE:
			{
				HWND list = GetDlgItem(hDlg, LOWORD(wParam));

				DrawableObjectsPresenter2 * presenter;
				std::map<std::string, std::map<int, int>> *currentObjs;

				if (LOWORD(wParam) == IDC_LIST_FROM)
				{
					presenter = FromDOPresenter;
					currentObjs = &fromCurrentObjs;
				}
				else
				{
					presenter = ToDOPresenter;
					currentObjs = &toCurrentObjs;
				}

				ListController::checkEqualityBetweenCurObjsAndList(
					hDlg, list, presenter, *currentObjs, 
					IsDlgButtonChecked(hDlg, IDC_CHECK_MULTIPLE_SEL));

				if (LOWORD(wParam) == IDC_LIST_FROM && toCurrentObjs.size() != 0) {
					toCurrentObjs.clear();
					SendDlgItemMessage(hDlg, IDC_LIST_TO, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);
				}
				else if (LOWORD(wParam) == IDC_LIST_TO && fromCurrentObjs.size() != 0) {
					fromCurrentObjs.clear();
					SendDlgItemMessage(hDlg, IDC_LIST_FROM, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);
				}

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

		case IDC_BUTTON_SET_PROBABIL:
		{
			if (!(fromCurrentObjs.size() == 1 && fromCurrentObjs.begin()->second.size() == 1) &&
				!(toCurrentObjs.size() == 1 && toCurrentObjs.begin()->second.size() == 1)) {
				MessageBox(ParticleSysCreator_hWnd, "You shoud choose only one object\nfor set probability", "Warning", MB_YESNO | MB_ICONQUESTION);
				return TRUE;
			}

			int *mas = new int[1024];

			bool isItemLocatedInFrom = true;
			HWND list, probList;
			std::map<std::string, std::map<int, AreaForParticleSys>> *koef;
			
			list = GetDlgItem(hDlg, IDC_LIST_FROM);
			probList = GetDlgItem(hDlg, IDC_LIST_FROM_PROB);
			koef = fromKoefsForParticleSys;

			long lSelCnt = SendMessage(list, LB_GETSELCOUNT, 0, 0);

			auto res = SendMessage(list, LB_GETSELITEMS, (WPARAM)lSelCnt, (LPARAM)mas);

			if (lSelCnt == 0) {
				list = GetDlgItem(hDlg, IDC_LIST_TO);
				probList = GetDlgItem(hDlg, IDC_LIST_TO_PROB);
				koef = toKoefsForParticleSys;
				lSelCnt = SendMessage(list, LB_GETSELCOUNT, 0, 0);
				res = SendMessage(list, LB_GETSELITEMS, (WPARAM)lSelCnt, (LPARAM)mas);
				isItemLocatedInFrom = false;
			}

			LPTSTR str = new TCHAR[80];
			SendMessage(list, LB_GETTEXT, (WPARAM)*mas, (LPARAM)str);
			char* buf = nullptr;
			char* objName = nullptr;

			char* num = nullptr;
			int intNum;

			objName = strtok_s(str, "#", &buf);

			num = strtok_s(NULL, "#", &buf);
			intNum = atoi(num);

			auto editDlg = DialogBox(ParticleSysCreator_hInstance,
				MAKEINTRESOURCE(IDD_SET_PROBABILITY_DLG),
				hDlg, (DLGPROC)SetProbabilityDlgProc);

			(*koef)[objName][intNum].chanceToAppear = probability;

			SendMessage(probList, LB_DELETESTRING, (WPARAM)*mas, 0);
			SendMessage(probList, LB_INSERTSTRING, 
				(WPARAM)*mas, (LPARAM)std::to_string(probability).c_str());
			

			return TRUE;
		}

		case IDC_BUTTON_APPLY_NUM_OF_PARTS:
		{
			GLfloat numOfParts = DlgHelper::getValFromEdit(hDlg, IDC_EDIT_NUM_OF_PARTICLES);
			particleInfo->numOfParticles = numOfParts;

			return TRUE;
		}

		case IDC_BUTTON_CREATE_PARTICLE_SYS:
		{
			if (!canCreateParticleSystem(*particleInfo))
				return TRUE;

			if (particleSystem != nullptr && isEditFromMapCreator)
				delete particleSystem;

			std::vector<std::map<std::string, std::map<int, AreaForParticleSys>>*> koefs = {
				fromKoefsForParticleSys,
				toKoefsForParticleSys
			};

			for (auto it_koef = koefs.begin(); it_koef != koefs.end(); ++it_koef)
				for (auto it_name = (*it_koef)->begin(); it_name != (*it_koef)->end(); ++it_name)
					for (auto it = it_name->second.begin(); it != it_name->second.end(); ++it) {
						auto presenter = FromDOPresenter;
						if (it_koef - koefs.begin() != 0)
							presenter = ToDOPresenter;
							
						int id = presenter->getObjId(it_name->first, it->first);
						auto pos = *presenter->getObj(id)->getPosition();
						auto rot = presenter->getObj(id)->getRotation();

						it->second.areaPos = pos;
						it->second.areaRot = rot;
					}

			particleSystem = new ParticleSystem(
				FromDOPresenter, ToDOPresenter, koefs,
				particleInfo, moveBtnsControl
			);

			return TRUE;
		}

		case IDC_BUTTON_SAVE_PART_SYS:
		{
			if (particleSystemFromMapCreator != nullptr)
				*particleSystemFromMapCreator = particleSystem;
			else
				saveModel();

			return TRUE;
		}

		case IDC_BUTTON_LOAD_PART_SYS:
		{
			loadModel();

			return TRUE;
		}

		return TRUE;
		}
	}
	return FALSE;
}

WinCreateParticleSystem::WinCreateParticleSystem(HWND & main_hWnd, ParticleSystem **partSys,
	DrawableObjectsPresenter2 *mapDOPresenter, std::string partName)
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	HGLRC		ParticleSysCreator_hRC = NULL;		// Permanent Rendering Context

	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.f };

	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, .0f }; // Çíà÷åíèÿ äèôôóçíîãî ñâåòà ( ÍÎÂÎÅ )

	GLfloat LightPosition[] = { 0.0f, .0f, 0.0f, .0f };     // Ïîçèöèÿ ñâåòà ( ÍÎÂÎÅ )

	std::vector<LightGL> lights_vect = { LightGL(LightAmbient, LightDiffuse, LightPosition) };

	if (!WindowHelper::CreateGLWindow(ParticleSysCreator_hWnd, ParticleSysCreator_hInstance, ParticleSysCreatorWndProc,
		&main_hWnd, ParticleSysCreator_hDC, ParticleSysCreator_hRC,
		InitGL, lights_vect, "ParticleSysCreator", 0, 0, "NeHe's Texture Mapping Tutorial", 640, 480, 16))
	{
		return;									// Quit If Window Was Not Created
	}
	Init();
	InitModelCreatorWindow(modelsMap);

	if (partSys != nullptr) {
		particleSystemFromMapCreator = partSys;
		particleSystem = *partSys;
	}

	auto dlg = CreateDialogParam(ParticleSysCreator_hInstance, MAKEINTRESOURCE(IDD_PARTICLE_SYS_DLG),
		ParticleSysCreator_hWnd, ParticleSysDlgProc, NULL);

	elementsMap["ObjDlg"] = dlg;
	ShowWindow(dlg, TRUE);

	logger.setElementsMap(&elementsMap);
	logger.addDrObjPresenter("from", FromDOPresenter);
	logger.addDrObjPresenter("to", ToDOPresenter);

	if (partSys != nullptr) {
		isEditFromMapCreator = true;

		auto koefs = particleSystem->getKoefs();
		auto partSysInfo = particleSystem->getParticleInfo();
		auto partSysPresenters = particleSystem->getPresenters();

		auto presenter = partSysPresenters.first;
		auto locPresenter = FromDOPresenter;
		int listId = IDC_LIST_FROM;
		int probListId = IDC_LIST_FROM_PROB;

		//FromDOPresenter = partSysPresenters.first;
		//ToDOPresenter = partSysPresenters.second;
		auto cam = moveBtnsControl->getCamera();
		for (int i = 0; i < 2; ++i) {
			auto indexesMap = presenter->getObjMapIndex();

			for (auto it_name = indexesMap->begin(); it_name != indexesMap->end(); ++it_name) {
				auto pairMap = &it_name->second;

				for (auto it = pairMap->begin(); it != pairMap->end(); ++it) {
					auto obj = presenter->getObj(it->second);

					auto objPos = *obj->getPosition();
					int index = locPresenter->addObj(obj, it_name->first, cam);
					locPresenter->getObj(index)->setPosition(objPos);

					ñolorMapForObjects[locPresenter][index] =
						DrawableObjectsPresenter2::getColorForSimpleDraw(ñolorMapForObjects);

					std::string objName = it_name->first + "#" + std::to_string(it->first);

					//Çàïîëíÿåì From è To ëèñòû
					SendDlgItemMessage(dlg, listId, LB_ADDSTRING, 0, (LPARAM)objName.c_str());

					//Çàïîëíÿåì FromProbability è ToProbability ëèñòû
					auto area = &(*koefs[i])[it_name->first][it->first];
					SendDlgItemMessage(dlg, probListId, LB_ADDSTRING, 0, (LPARAM)std::to_string(area->chanceToAppear).c_str());
				}
			}

			presenter = partSysPresenters.second;
			locPresenter = ToDOPresenter;
			particleName = partName;

			listId = IDC_LIST_TO;
			probListId = IDC_LIST_TO_PROB;
		}

		particleInfo = partSysInfo;
		*fromKoefsForParticleSys = *koefs[0];
		*toKoefsForParticleSys = *koefs[1];

		//SendDlgItemMessage(dlg, IDC_EDIT_NUM_OF_PARTICLES, )
		DlgHelper::setValForEdit(dlg, IDC_EDIT_NUM_OF_PARTICLES, partSysInfo->numOfParticles);
	}
	else
		particleSystemFromMapCreator = nullptr;

	

	while (!done)									// Loop That Runs While done=FALSE
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				if (MessageBox(ParticleSysCreator_hWnd, "Âû óâåðåíû, ÷òî õîòèòå âûéòè?\nÂñå íåñîõðàí¸ííûå èçìåíåíèÿ áóäóò ïîòåðÿíû", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					done = TRUE;							// If So done=TRUE
					WindowHelper::KillGLWindow(ParticleSysCreator_hWnd, 
						ParticleSysCreator_hInstance, ParticleSysCreator_hDC,
						ParticleSysCreator_hRC, "ParticleSysCreator");
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
			//Select Object
			if (addVars.keys[VK_LBUTTON] == FALSE || addVars.keys[VK_SHIFT] == FALSE)
				canSelObj = true;
			if (addVars.keys[VK_LBUTTON] && addVars.keys[VK_SHIFT] && canSelObj)
			{
				canSelObj = false;
				auto res1 = GraphicHelper::getSelectedObj(ParticleSysCreator_hWnd,
					moveBtnsControl->getCamera(), FromDOPresenter, ñolorMapForObjects[FromDOPresenter]);

				auto res2 = GraphicHelper::getSelectedObj(ParticleSysCreator_hWnd,
					moveBtnsControl->getCamera(), ToDOPresenter, ñolorMapForObjects[ToDOPresenter]);

				std::vector<NameIndexId> nameIndexIdVect = { res1, res2 };
				std::vector<std::map<std::string, std::map<int, int>>*> vectCurObjs = {
					&fromCurrentObjs,
					&toCurrentObjs

				};
				for (int k = 0; k < 2; ++k)
				{
					auto res = nameIndexIdVect[k];
					auto currentObjs = vectCurObjs[k];
					HWND list;
					DrawableObjectsPresenter2* DOPresenter;

					if (k == 0) {
						list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
						DOPresenter = FromDOPresenter;
					}
					else {
						list = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
						DOPresenter = ToDOPresenter;
					}


					if (isThisObjSelect(*currentObjs, res))
					{
						(*currentObjs)[res.name].erase((*currentObjs)[res.name].find(res.index));

						if ((*currentObjs)[res.name].size() == 0)
							currentObjs->erase(res.name);

						int itemIndexInList = SendMessage(list, LB_FINDSTRING, 0,
							(LPARAM)(std::string(res.name) + "#" + std::to_string(res.index)).c_str());

						SendMessage(list, LB_SETSEL,
							(WPARAM)(BOOL)FALSE, (LPARAM)(UINT)itemIndexInList);

						//if (auto hDlg = GetWindow(ParticleSysCreator_hWnd, IDD_DIALOG1))
							//SendMessage(hDlg, PD::WM_CHECHOBJDATA, 0, 0);

						if (elementsMap["EditDlg"]) {
							SendMessage(elementsMap["EditDlg"], PD::WM_CHECHOBJDATA, 0, 0);
							//SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
						}
					}
					else if (res.id != -1)
					{
						if (!IsDlgButtonChecked(elementsMap["ObjDlg"], IDC_CHECK_MULTIPLE_SEL)) {
							auto temp = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_FROM);
							SendMessage(temp, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);
							fromCurrentObjs.clear();

							temp = GetDlgItem(elementsMap["ObjDlg"], IDC_LIST_TO);
							SendMessage(temp, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);
							toCurrentObjs.clear();
						}

						ListController::addObjToCurObjs(elementsMap["ObjDlg"],
							list, DOPresenter, *currentObjs, res.id);

						if (elementsMap["EditDlg"]) {
							SendMessage(elementsMap["EditDlg"], PD::WM_CHECHOBJDATA, 0, 0);
							//SendMessage(elementsMap["EditDlg"], PD::WM_CHECKVERTDATA, 0, 0);
						}
					}
				}
			}


			if (addVars.keys['P'] == FALSE)
				pp = false;
			if (addVars.keys['P'] && !pp) //Particle edit dlg
			{
				pp = true;
				if (alreadyPressedP == false) {
					auto editParticleSysDlg = CreateDialogParam(ParticleSysCreator_hInstance,
						MAKEINTRESOURCE(IDD_EDIT_PARTICLE_SYS_DLG),
						ParticleSysCreator_hWnd, EditParticleSysDlgProc, NULL);
					ShowWindow(editParticleSysDlg, TRUE);	

					elementsMap["EditParticleSysDlg"] = editParticleSysDlg;
					alreadyPressedP = true;
				}
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
				SwapBuffers(ParticleSysCreator_hDC);					// Swap Buffers (Double Buffering)
				if (addVars.keys['L'] && !lp) // Êëàâèøà 'L' íàæàòà è íå óäåðæèâàåòñÿ?
				{
					lp = TRUE;      // lp ïðèñâîèëè TRUE
					light = !light; // Ïåðåêëþ÷åíèå ñâåòà TRUE/FALSE
				}
				if (!light)               // Åñëè íå ñâåò
				{
					glDisable(GL_LIGHTING);  // Çàïðåò îñâåùåíèÿ
				}
				else                      // Â ïðîòèâíîì ñëó÷àå
				{
					glEnable(GL_LIGHTING);   // Ðàçðåøèòü îñâåùåíèå
				}
			}
			if (!addVars.keys['L']) // Êëàâèøà 'L' Îòæàòà?
			{
				lp = FALSE;      // Åñëè òàê, òî lp ðàâíî FALSE
			}

			objectsControl(*moveBtnsControl, FromDOPresenter, fromCurrentObjs, &addVars.keys,
				addVars.wheelDelta, elementsMap, logger, IDC_EDIT_CAMERA_SPEED);//, currentObj, DOPresenter);
			objectsControl(*moveBtnsControl, ToDOPresenter, toCurrentObjs, &addVars.keys,
				addVars.wheelDelta, elementsMap, logger, IDC_EDIT_CAMERA_SPEED);

			cameraControl(*moveBtnsControl, &addVars.keys, addVars.wheelDelta,
				elementsMap, IDC_EDIT_CAMERA_SPEED);
		}
	}
}

int WinCreateParticleSystem::DrawGLScene()
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

	const CameraLocation& cam = moveBtnsControl->getCamera();
	glRotatef(cam.getYAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(cam.getXAngle(), 0.0f, 1.0f, 0.0f);

	glTranslatef(cam.getX(), cam.getY(), cam.getZ());      // Ïåðåíîñ Â/Âíå ýêðàíà ïî z
														   //glOrtho(-200, 200, -200, 200, -200, 200);
	glColor3ub(0, 130, 0);
	glCallList(modelsMap["grid"]);

	static std::pair<int, Vect3f*> vertForLink = { -1, nullptr };
	static std::map<int, std::map<Vect3f*, vector3ub>> colorMapForVertexes;
	static std::map<int, std::set<OpenGL_Base*>> OGLB_ForMapTex;
	static std::map<std::string, std::map<int, int>> currentObjs;

	glPushMatrix();

	if (mapDOPresenter != nullptr)
		mapDOPresenter->show(moveBtnsControl->getCamera(), currentObjs, 
			false, colorMapForVertexes, vertForLink, OGLB_ForMapTex);

	glPopMatrix();


	glPushMatrix();

	//DOPresenter->show(cam, currentObjs, isVertexesEnable, currentVertexes, vertForLink);
	if (particleSystem != nullptr &&
		IsDlgButtonChecked(elementsMap["ObjDlg"], IDC_CHECK_SHOW_PARTS))
	{
		if (IsDlgButtonChecked(elementsMap["ObjDlg"], IDC_CHECK_SHOW_AREAS))
			particleSystem->showAreas(fromCurrentObjs, toCurrentObjs);

		particleSystem->play();
	}
	else
	{
		FromDOPresenter->show(cam, fromCurrentObjs, false,
			colorMapForVertexes, vertForLink, OGLB_ForMapTex);

		ToDOPresenter->show(cam, toCurrentObjs, false,
			colorMapForVertexes, vertForLink, OGLB_ForMapTex);
	}



	glPopMatrix();

	glLoadIdentity();
	glColor3ub(0, 200, 200);

	glTranslatef(0, 0, -1.0f);
	glRasterPos2f(0.38, -0.38);

	glPrint(base, "%3.2f", fps);

	return TRUE;										// Keep Going
}

WinCreateParticleSystem::~WinCreateParticleSystem()
{
	clearAllParticleSystemCreator();

	//delete moveBtnsControl;
	if (particleSystemFromMapCreator == nullptr)
	{
		particleInfo->numOfParticles = 0;
		if (particleInfo->particle != nullptr)
			delete particleInfo->particle;

		particleInfo->particleFluct = Vect3f(0, 0, 0);
		particleInfo->particleFluctLen = Vect3f(0, 0, 0);
		particleInfo->particleRot = Vect3f(0, 0, 0);
		particleInfo->particleSpeed = 0.0f;
		particleInfo->speedOfOccur = 0.0f;
	}
	modelsMap.clear();
	elementsMap.clear();
}
