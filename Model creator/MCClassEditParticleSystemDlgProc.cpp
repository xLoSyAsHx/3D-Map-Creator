#include "MCClassEditParticleSystemDlgProc.h"


HINSTANCE MCClassEditParticleSystemDlgProc::hInstance;

AddVars *MCClassEditParticleSystemDlgProc::addVars;

bool *MCClassEditParticleSystemDlgProc::alreadyPressedP;
ParticleInfo *MCClassEditParticleSystemDlgProc::particleInfo;
std::string *MCClassEditParticleSystemDlgProc::particleName;

std::map<std::string, DrawableObject2*> *MCClassEditParticleSystemDlgProc::drawableObjsMap;
std::map<std::string, HWND> *MCClassEditParticleSystemDlgProc::elementsMap;



INT_PTR MCClassEditParticleSystemDlgProc::EditParticleSysDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

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
		addVars->keys['P'] = FALSE;
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
		if (particleName->length() < 23)
			i = ((405 - 218) - particleName->length()  * ((405 - 218) / 32)) / 2;
		::TextOut(testDC, 218 + i, 270, particleName->c_str(), particleName->length());
		EndPaint(hDlg, &ps);

		if (isFirst) {
			isFirst = false;
			InvalidateRect(hDlg, NULL, TRUE);
			UpdateWindow(hDlg);
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
			MCClassItemsAddDlgProc::setNecessaryDatas(particleInfo, particleName,
				drawableObjsMap, elementsMap);

			

			DialogBox(hInstance,
				MAKEINTRESOURCE(IDD_ADD_ITEM_DLG),
				hDlg, (DLGPROC)MCClassItemsAddDlgProc::ItemsAddDlgProc);

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
					IDC_BUTTON_PART_ROT_DOWN_X, ID_APPLY_PART_ROT);
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

void MCClassEditParticleSystemDlgProc::setNecessaryData(HINSTANCE _hInstance,
	AddVars *_addVars, bool *_alreadyPressedP, ParticleInfo *_particleInfo,
	std::string *_particleName, std::map<std::string, DrawableObject2*> *_drawableObjsMap,
	std::map<std::string, HWND> *_elementsMap)
{
	hInstance = _hInstance;
	addVars = _addVars;
	alreadyPressedP = _alreadyPressedP;
	particleInfo = _particleInfo;
	particleName = _particleName;
	drawableObjsMap = _drawableObjsMap;
	elementsMap = _elementsMap;
}
