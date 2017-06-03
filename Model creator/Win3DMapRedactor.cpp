#include "Win3DMapRedactor.h"

HWND Win3DMapRedactor::MapCreator_hWnd = NULL;
HDC	Win3DMapRedactor::MapCreator_hDC = NULL;
HGLRC Win3DMapRedactor::MapCreator_hRC = NULL;
HINSTANCE Win3DMapRedactor::MapCreator_hInstance = NULL;

Logger Win3DMapRedactor::logger;
AddVars Win3DMapRedactor::addVars;
MoveBtnsController Win3DMapRedactor::moveBtnsControl;
DrawableObjectsPresenter2* Win3DMapRedactor::DOPresenter = new DrawableObjectsPresenter2();
ParticleSystemPresenter* Win3DMapRedactor::PSPresenter = new ParticleSystemPresenter();

ParticleSystem* Win3DMapRedactor::currentPartSys = nullptr;
std::map<std::string, DrawableObject2*> *Win3DMapRedactor::drawableObjsMap = new std::map<std::string, DrawableObject2*>;
std::map<std::string, HWND> Win3DMapRedactor::elementsMap;

bool Win3DMapRedactor::haveSmthgNew;
std::string Win3DMapRedactor::newObjName;
std::string Win3DMapRedactor::particleName;
std::map<std::string, GLuint> Win3DMapRedactor::modelsMap;

std::map<std::string, ParticleSystem*> Win3DMapRedactor::particleSysMap;
std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>> Win3DMapRedactor::сolorMapForObjects;

bool Win3DMapRedactor::alreadyPressedE = false;
bool Win3DMapRedactor::alreadyPressedP = false;
//std::map<std::string, std::vector<vector3ub>> colorMapForObjects = { { "common",{{0,0,1}} },{ "trans",{} } };

void Win3DMapRedactor::Init()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	std::string fileName;
	std::string fileNameForLoader;
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
			//fileNameForLoader = (FindFileData.cFileName);
			char* index = strchr(FindFileData.cFileName, '.');
			if (index != NULL)
			{
				if (strlen(index) == 6)
				{
					if (strcmp(index, ".3drps") == 0) {//  index == ".3dr") {
						auto name = "Data/Models/" + std::string(FindFileData.cFileName);
						try {
							DrawableObjectsPresenter2 *FromDOPresenter = new DrawableObjectsPresenter2();
							DrawableObjectsPresenter2 *ToDOPresenter = new DrawableObjectsPresenter2();
							std::map<std::string, std::map<int, AreaForParticleSys>> *fromKoefsForParticleSys = new std::map<std::string, std::map<int, AreaForParticleSys>>;
							std::map<std::string, std::map<int, AreaForParticleSys>> *toKoefsForParticleSys = new std::map<std::string, std::map<int, AreaForParticleSys>>;
							std::map<
								DrawableObjectsPresenter2*,
								std::map<std::string, std::map<int, AreaForParticleSys>>> mapKoefsForPartSys;

							ParticleInfo *particleInfo = new ParticleInfo;
							//std::string particleName;

							auto objName = Loader::loadParticleSystem(name, nullptr,
								FromDOPresenter, ToDOPresenter, fromKoefsForParticleSys,
								toKoefsForParticleSys, *particleInfo,
								moveBtnsControl.getCamera(), сolorMapForObjects,
								NULL, NULL, NULL, NULL, NULL, particleName);

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

							ParticleSystem *partSys = new ParticleSystem(FromDOPresenter, ToDOPresenter,
								koefs, particleInfo, &moveBtnsControl);

							particleSysMap[objName] = partSys;
							
						}
						catch (...) {
							MessageBox(MapCreator_hWnd, "File was damaged", (std::string("Can't load Particle System") + fileName).c_str(), MB_OK);
						}
						
					}
				}
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}

	CreateModelBuildLists(modelsMap);

	BuildFont(MapCreator_hDC, base);
}

std::vector<std::string> Win3DMapRedactor::getParticleSysKeys(std::map<std::string, ParticleSystem*> map)
{
	std::vector<std::string> keys;

	for (auto it = map.begin(); it != map.end(); ++it)
		keys.push_back(it->first);

	return keys;
}

void Win3DMapRedactor::loadMap()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = MapCreator_hWnd;
	ofn.lpstrFilter = "Model Creator Files (*.3drm)\0*.3drm\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrInitialDir = "C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Model creator\\Data\\Models";

	if (GetOpenFileName(&ofn))
	{
		std::ifstream isFileExist(ofn.lpstrFile, std::ios_base::in | std::ios_base::_Nocreate);
		if (isFileExist.is_open())
		{
			isFileExist.close();
			//if (MessageBox(MapCreator_hWnd, "Model with such name already exist. Do you want to overwrite it?", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				//WindowHelper::KillGLWindow(ModelCreator_hWnd, ModelCreator_hInstance, ModelCreator_hDC, ModelCreator_hRC, "ModelCreator");
				//DestroyWindow(elementsMap["commonObjList"]);
				//EnableWindow(main_hWnd, TRUE);
				Loader::loadMapForMapRedactor(ofn.lpstrFile, DOPresenter, PSPresenter,
					&moveBtnsControl, GetDlgItem(elementsMap["objDlg"], IDC_LIST_PARTICLE_SYSTEMS));
			}
		}
		else
		{
			std::fstream f(ofn.lpstrFile);
			if (f.is_open())
				f.close();

			Loader::loadMapForMapRedactor(ofn.lpstrFile, DOPresenter, PSPresenter,
				&moveBtnsControl, GetDlgItem(elementsMap["objDlg"], IDC_LIST_PARTICLE_SYSTEMS));
		}
		// Do something usefull with the filename stored in szFileName 
	}

	
}

void Win3DMapRedactor::saveMap()
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = MapCreator_hWnd;
	ofn.lpstrFilter = "Model Creator Files (*.3dm)\0*.3dm\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrInitialDir = "C:\\Users\\User\\Desktop\\HSE\\Programming\\C++\\Курс 2\\OpenGL\\курсач\\Model creator\\Model creator\\Data\\Models";

	if (GetSaveFileName(&ofn))
	{
		std::ifstream isFileExist(ofn.lpstrFile, std::ios_base::in | std::ios_base::_Nocreate);
		if (isFileExist.is_open())
		{
			isFileExist.close();
			if (MessageBox(MapCreator_hWnd, "Model with such name already exist. Do you want to overwrite it?", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				//WindowHelper::KillGLWindow(ModelCreator_hWnd, ModelCreator_hInstance, ModelCreator_hDC, ModelCreator_hRC, "ModelCreator");
				//DestroyWindow(elementsMap["commonObjList"]);
				//EnableWindow(main_hWnd, TRUE);

				Saver::saveMap(ofn.lpstrFile, DOPresenter, PSPresenter, &moveBtnsControl);
			}
		}
		else
		{
			std::fstream f(ofn.lpstrFile);
			if (f.is_open())
				f.close();

			Saver::saveMap(ofn.lpstrFile, DOPresenter, PSPresenter, &moveBtnsControl);
		}
		// Do something usefull with the filename stored in szFileName 
	}
}

INT_PTR Win3DMapRedactor::ParticleSysAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR LBString[128];

	switch (message)
	{

	case WM_INITDIALOG:
	{
		HWND list = GetDlgItem(hDlg, IDC_LIST_ALL_ITEMS);
		//SendMessage(list, LB_SETITEMDATA, 0, (LPARAM)"asd");
		//int pos = (int)SendMessage(list, LB_ADDSTRING, 0,
		//(LPARAM)"item");


		//
		std::vector<std::string> keys = getParticleSysKeys(particleSysMap);
		for (auto key : keys)
			SendMessage(list, LB_ADDSTRING, 0, (LPARAM)key.c_str());

		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ADD:
		{
			HWND list = GetDlgItem(hDlg, IDC_LIST_ALL_ITEMS);

			int id = SendMessage(list, LB_GETCURSEL, 0, 0);

			SendMessage(list, LB_GETTEXT, (WPARAM)id, (LPARAM)LBString);

			/*
			char* buf = nullptr;
			char *objName = strtok_s(LBString, "#", &buf);

			char* num = strtok_s(NULL, "#", &buf);
			int intNum = atoi(num);
			*/

			if (particleSysMap.find(LBString) == particleSysMap.end())
				return TRUE;

			int index = PSPresenter->addParticleSystem(
				particleSysMap[LBString], LBString, &moveBtnsControl);

			newObjName = std::string(LBString) + "#" + std::to_string(index);

			haveSmthgNew = true;

		}

		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));

			return TRUE;
		}

		return TRUE;
		}
	}
	return FALSE;
}

LRESULT CALLBACK Win3DMapRedactor::MapCreatorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;


	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CREATE:
	{
		HWND button = CreateWindow("button", "New map", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_MULTILINE,
		0, 0, 150, 20, hWnd, (HMENU)ID_MAIN_BUTTON_NEW_MAP, NULL, NULL);
		//SetWindowLongA(Button.hwnd, GWL_STYLE, Style);
		//CreateCompatibleBitmap(Main_hDC, 15, 20);
		elementsMap["1"] = button;


		//SendMessageA(button1, BM_SETIMAGE, IMAGE_BITMAP, hBitmap);

		//RoundRect
		button = CreateWindow("button", "Load map", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_MULTILINE,
			151, 0, 130, 30, hWnd, (HMENU)ID_MAIN_BUTTON_LOAD_MAP, NULL, NULL);
		elementsMap["2"] = button;
		button = CreateWindow("button", "Save map", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_MULTILINE,
			302, 0, 130, 30, hWnd, (HMENU)ID_MAIN_BUTTON_SAVE_MAP, NULL, NULL);
		elementsMap["3"] = button;
		button = CreateWindow("button", "Settings", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_MULTILINE,
			453, 0, 130, 30, hWnd, (HMENU)ID_MAIN_BUTTON_SETTINGS, NULL, NULL);
		elementsMap["4"] = button;
		button = CreateWindow("button", "Particle system\nCreator", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_MULTILINE,
			453, 0, 130, 30, hWnd, (HMENU)ID_MAIN_BUTTON_PARTICLE_SYS_CREATOR, NULL, NULL);
		elementsMap["ParticleSysCreator"] = button;
		button = CreateWindow("button", "Objects\nCreator", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_MULTILINE,
			453, 0, 130, 30, hWnd, (HMENU)ID_MAIN_BUTTON_OBJECTS_CREATOR, NULL, NULL);
		elementsMap["ObjectsCreator"] = button;
		return 0;
	}
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
	/*
	case WM_PAINT:
	{
		
		RECT rect;
		GetWindowRect(hWnd, &rect);
		if (!GetUpdateRect(hWnd, &rect, FALSE)) 
			return 1;

		Main_hDC = BeginPaint(hWnd, &ps);
		// Рисуем прямоугольник.
		Rectangle(Main_hDC, 100, 100, 200, 80);
		// Заканчиваем рисование.
		EndPaint(hWnd, &ps);
		
	}
	*/
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

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_MAIN_BUTTON_NEW_MAP:
		{


			return 0;
		}

		case ID_MAIN_BUTTON_LOAD_MAP:
		{
			loadMap();

			return 0;
		}

		case ID_MAIN_BUTTON_SAVE_MAP:
		{
			saveMap();

			return 0;
		}

		case ID_MAIN_BUTTON_SETTINGS:
		{


			return 0;
		}

		case ID_MAIN_BUTTON_PARTICLE_SYS_CREATOR:
		{
			for (auto it = elementsMap.begin(); it != elementsMap.end(); ++it)
				ShowWindow(it->second, FALSE);
			WinCreateParticleSystem newCreateParticleSystem(MapCreator_hWnd);

			for (auto it = elementsMap.begin(); it != elementsMap.end(); ++it)
				ShowWindow(it->second, SW_SHOW);

			if (!wglMakeCurrent(MapCreator_hDC, MapCreator_hRC))				// Are We Able To Get A Rendering Context?
			{
				WindowHelper::KillGLWindow(hWnd, MapCreator_hInstance,
					MapCreator_hDC, MapCreator_hRC, "MapRedactor");									// Reset The Display
				MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return FALSE;								// Return FALSE
			}

			ShowWindow(hWnd, SW_SHOW);
			SetFocus(hWnd);

			return 0;
		}

		case ID_MAIN_BUTTON_OBJECTS_CREATOR:
		{
			for (auto it = elementsMap.begin(); it != elementsMap.end(); ++it)
				ShowWindow(it->second, FALSE);

			WinCreateModel winCreateModel(MapCreator_hWnd);

			for (auto it = elementsMap.begin(); it != elementsMap.end(); ++it)
				ShowWindow(it->second, SW_SHOW);

			if (!wglMakeCurrent(MapCreator_hDC, MapCreator_hRC))				// Are We Able To Get A Rendering Context?
			{
				WindowHelper::KillGLWindow(hWnd, MapCreator_hInstance,
					MapCreator_hDC, MapCreator_hRC, "MapRedactor");									// Reset The Display
				MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				return FALSE;								// Return FALSE
			}

			ShowWindow(hWnd, SW_SHOW);
			SetFocus(hWnd);

			return 0;
		}

		}
		return 0;
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
		int width = LOWORD(lParam), height = HIWORD(lParam);

		int btnWidth = width / 6;
		int curPos = 0;


		bool asd = SetWindowPos(elementsMap["1"], HWND_TOP, curPos, 0, (int)btnWidth, 30, SWP_FRAMECHANGED);
		curPos += btnWidth + 1;
		SetWindowPos(elementsMap["2"], HWND_TOP, curPos, 0, (int)btnWidth, 30, SWP_FRAMECHANGED);
		curPos += btnWidth + 1;
		SetWindowPos(elementsMap["3"], HWND_TOP, curPos, 0, (int)btnWidth, 30, SWP_FRAMECHANGED);
		curPos += btnWidth + 1;
		SetWindowPos(elementsMap["4"], HWND_TOP, curPos, 0, (int)btnWidth, 30, SWP_FRAMECHANGED);
		curPos += btnWidth + 1;
		SetWindowPos(elementsMap["ParticleSysCreator"], HWND_TOP, curPos, 0, (int)btnWidth, 30, SWP_FRAMECHANGED);
		curPos += btnWidth + 1;
		SetWindowPos(elementsMap["ObjectsCreator"], HWND_TOP, curPos, 0, (int)btnWidth, 30, SWP_FRAMECHANGED);
		
		WindowHelper::ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Win3DMapRedactor::ParticleSystemsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	static LPTSTR LBString = new TCHAR[80];
	char* buf = nullptr;
	char* objName = nullptr;
	char* num = nullptr;
	int intNum;
	int objId;

	if (haveSmthgNew == true)
	{
		haveSmthgNew = false;
		HWND list = GetDlgItem(hDlg, IDC_LIST_PARTICLE_SYSTEMS);

		SendMessage(list, LB_ADDSTRING, 0, (LPARAM)newObjName.c_str());
	}

	switch (message)
	{

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ADD_PS:
		{
			DialogBox(MapCreator_hInstance,
				MAKEINTRESOURCE(IDD_ADD_ITEM_DLG),
				hDlg, (DLGPROC)ParticleSysAddDlgProc);

			return TRUE;
		}

		case IDC_BUTTON_EDIT_PS:
		{
			if (currentPartSys != nullptr) {
				for (auto it = elementsMap.begin(); it != elementsMap.end(); ++it)
					ShowWindow(it->second, FALSE);

				auto name = PSPresenter->erase(currentPartSys);

				WinCreateParticleSystem winCreateParticleSystem(MapCreator_hWnd,
					&currentPartSys, DOPresenter, particleName);

				//PSPresenter->clear();
				;
				PSPresenter->addParticleSystem(
					currentPartSys, name, &moveBtnsControl);

				for (auto it = elementsMap.begin(); it != elementsMap.end(); ++it)
					ShowWindow(it->second, SW_SHOW);

				if (!wglMakeCurrent(MapCreator_hDC, MapCreator_hRC))				// Are We Able To Get A Rendering Context?
				{
					WindowHelper::KillGLWindow(MapCreator_hWnd, MapCreator_hInstance,
						MapCreator_hDC, MapCreator_hRC, "MapRedactor");									// Reset The Display
					MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
					return FALSE;								// Return FALSE
				}

				ShowWindow(MapCreator_hWnd, SW_SHOW);
				SetFocus(MapCreator_hWnd);
			}

			return TRUE;
		}

		case IDC_BUTTON_DELETE_PS:
		{
			/*
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
			*/

			return TRUE;
		}

		case IDC_LIST_PARTICLE_SYSTEMS:
		{
			switch (HIWORD(wParam))
			{

			case LBN_SELCHANGE:
			{
				char* str = "asdasd";

				HWND list = GetDlgItem(hDlg, IDC_LIST_PARTICLE_SYSTEMS);

				int id = SendMessage(list, LB_GETCURSEL, 0, 0);

				SendMessage(list, LB_GETTEXT, (WPARAM)id, (LPARAM)LBString);

				
				buf = nullptr;
				objName = strtok_s(LBString, "#", &buf);

				num = strtok_s(NULL, "#", &buf);
				intNum = atoi(num);
				
				auto obj = PSPresenter->getObj(objName, intNum);

				if (obj != nullptr)
					currentPartSys = obj;

				if (elementsMap["EditDlg"]) {
					SendMessage(elementsMap["EditDlg"], WM_CLOSE, 0, 0);
				}

				InvalidateRect(hDlg, NULL, TRUE);
				UpdateWindow(hDlg);
				
				return TRUE;
			}
			}
			return TRUE;
		}

		case IDC_CHECK_PLAY_CUR_PS:
		{
			bool state = IsDlgButtonChecked(hDlg, IDC_CHECK_PLAY_CUR_PS);

			if (state == true)
				SendDlgItemMessage(hDlg, IDC_CHECK_PLAY_ALL_PSs, BM_SETCHECK, FALSE, 0);

			return TRUE;
		}

		case IDC_CHECK_PLAY_ALL_PSs:
		{
			bool state = IsDlgButtonChecked(hDlg, IDC_CHECK_PLAY_ALL_PSs);

			if (state == true)
				SendDlgItemMessage(hDlg, IDC_CHECK_PLAY_CUR_PS, BM_SETCHECK, FALSE, 0);

			return TRUE;
		}

		case IDC_CHECK_SHOW_CUR_PS_AREA:
		{
			bool state = IsDlgButtonChecked(hDlg, IDC_CHECK_SHOW_CUR_PS_AREA);

			if (state == true)
				SendDlgItemMessage(hDlg, IDC_CHECK_SHOW_ALL_PS_AREAS, BM_SETCHECK, FALSE, 0);

			return TRUE;
		}

		case IDC_CHECK_SHOW_ALL_PS_AREAS:
		{
			bool state = IsDlgButtonChecked(hDlg, IDC_CHECK_SHOW_ALL_PS_AREAS);

			if (state == true)
				SendDlgItemMessage(hDlg, IDC_CHECK_SHOW_CUR_PS_AREA, BM_SETCHECK, FALSE, 0);

			return TRUE;
		}

		return TRUE;
		}
	}

	return FALSE;
}

Win3DMapRedactor::Win3DMapRedactor()
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop


	//HGLRC		MapCreator_hRC = NULL;		// Permanent Rendering Context

	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, .0f };

	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, .0f }; // Значения диффузного света ( НОВОЕ )

	GLfloat LightPosition[] = { 0.0f, .0f, 0.0f, .0f };     // Позиция света ( НОВОЕ )

	std::vector<LightGL> lights_vect = { LightGL(LightAmbient, LightDiffuse, LightPosition) };



	if (!WindowHelper::CreateGLWindow(MapCreator_hWnd, MapCreator_hInstance, MapCreatorWndProc,
		(new HWND()), MapCreator_hDC, MapCreator_hRC,
		InitGL, lights_vect, "MapRedactor", 0, 0, "Main", 640, 480, 16))
	{
		return;									// Quit If Window Was Not Created
	}

	auto dlg = CreateDialogParam(MapCreator_hInstance, MAKEINTRESOURCE(IDD_PARTICLE_SYSTEMS_DLG),
		MapCreator_hWnd, ParticleSystemsDlgProc, NULL);
	elementsMap["ObjDlg"] = dlg;
	//HWND dlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, ListBoxExampleProc);
	ShowWindow(dlg, TRUE);

	Init();

	//

	while (!done)									// Loop That Runs While done=FALSE
	{



		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				if (MessageBox(MapCreator_hWnd, "Вы уверены, что хотите выйти?\nВсе несохранённые изменения будут потеряны", "Warning", MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					done = TRUE;							// If So done=TRUE
					WindowHelper::KillGLWindow(MapCreator_hWnd, MapCreator_hInstance,
						MapCreator_hDC, MapCreator_hRC, "MapRedactor");
				}
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{


			if (addVars.keys['P'] == FALSE)
				pp = false;
			if (addVars.keys['P'] && !pp) //Particle edit dlg
			{
				pp = true;
				if (alreadyPressedP == false && currentPartSys != nullptr) {

					currentPartSys->getParticleInfo();



					MCClassEditParticleSystemDlgProc::setNecessaryData(
						MapCreator_hInstance, &addVars, &alreadyPressedP,
						currentPartSys->getParticleInfo(),
						&particleName, drawableObjsMap, &elementsMap);

					auto editParticleSysDlg = CreateDialogParam(MapCreator_hInstance,
						MAKEINTRESOURCE(IDD_EDIT_PARTICLE_SYS_DLG), MapCreator_hWnd,
						MCClassEditParticleSystemDlgProc::EditParticleSysDlgProc, NULL);
					ShowWindow(editParticleSysDlg, TRUE);

					elementsMap["EditParticleSysDlg"] = editParticleSysDlg;
					alreadyPressedP = true;
				}
			}







			if ((addVars.active && !DrawGLScene()) || addVars.keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done = TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(MapCreator_hDC);					// Swap Buffers (Double Buffering)
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


			cameraControl(moveBtnsControl, &addVars.keys, addVars.wheelDelta,
				elementsMap, IDC_EDIT_CAM_SPEED_MAIN);

			particleSysControl(moveBtnsControl, PSPresenter, currentPartSys, &addVars.keys,
				addVars.wheelDelta, elementsMap, logger, IDC_EDIT_CAM_SPEED_MAIN);

			//control(moveBtnsControl, DOPresenter, currentObjs, &addVars.keys, addVars.wheelDelta);//, currentObj, DOPresenter);



			/*
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
			bool a = drawableObject->isSelected(cam, mouseVect, main_hWnd);
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
			*/
		}
	}

}


int Win3DMapRedactor::DrawGLScene()

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


	const CameraLocation& cam = moveBtnsControl.getCamera();
	glRotatef(cam.getYAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(cam.getXAngle(), 0.0f, 1.0f, 0.0f);

	glTranslatef(cam.getX(), cam.getY(), cam.getZ());      // Перенос В/Вне экрана по z
														   //glOrtho(-200, 200, -200, 200, -200, 200);
	glColor3ub(0, 130, 0);
	glCallList(modelsMap["grid"]);

	glPushMatrix();

	static std::map<std::string, std::map<int, int>> currentObjs;
	static std::map<int, std::map<Vect3f*, vector3ub>> currentVertexes;
	static std::pair<int, Vect3f*> vertForLink = { -1, nullptr };
	static std::map<int, std::set<OpenGL_Base*>> OGLB_ForMappedTexture;
	DOPresenter->show(cam, currentObjs, false, currentVertexes, vertForLink, OGLB_ForMappedTexture);

	glPopMatrix();

	glPushMatrix();

	if (IsDlgButtonChecked(elementsMap["ObjDlg"], IDC_CHECK_PLAY_ALL_PSs))
		PSPresenter->show();
	else if (IsDlgButtonChecked(elementsMap["ObjDlg"], IDC_CHECK_PLAY_CUR_PS) && currentPartSys != nullptr)
		PSPresenter->show(currentPartSys);

	if (IsDlgButtonChecked(elementsMap["ObjDlg"], IDC_CHECK_SHOW_ALL_PS_AREAS))
		PSPresenter->showAreas(currentPartSys);
	else if (IsDlgButtonChecked(elementsMap["ObjDlg"], IDC_CHECK_SHOW_CUR_PS_AREA) && currentPartSys != nullptr)
		PSPresenter->showArea(currentPartSys);

	//PSPresenter->show();

	glPopMatrix();


	glLoadIdentity();
	glColor3ub(0, 200, 200);

	glTranslatef(0, 0, -1.0f);
	glRasterPos2f(0.38, -0.38);

	glPrint(base, "%3.2f", fps);

	return TRUE;										// Keep Going
}
