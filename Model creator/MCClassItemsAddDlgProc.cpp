#include "MCClassItemsAddDlgProc.h"



ParticleInfo *MCClassItemsAddDlgProc::particleInfo;
std::string *MCClassItemsAddDlgProc::particleName;
std::map<std::string, DrawableObject2*> *MCClassItemsAddDlgProc::drawableObjsMap;
std::map<std::string, HWND> *MCClassItemsAddDlgProc::elementsMap;


std::vector<std::string> MCClassItemsAddDlgProc::getDrObjKeys(std::map<std::string, DrawableObject2*> map)
{
	std::vector<std::string> keys;

	for (auto it = map.begin(); it != map.end(); ++it)
		keys.push_back(it->first);

	return keys;
}

INT_PTR MCClassItemsAddDlgProc::ItemsAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

		std::vector<std::string> keys = getDrObjKeys(*drawableObjsMap);
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
				(*drawableObjsMap)[LBString]);

			*particleName = LBString;


			InvalidateRect((*elementsMap)["EditParticleSysDlg"], NULL, TRUE);
			UpdateWindow((*elementsMap)["EditParticleSysDlg"]);

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

void MCClassItemsAddDlgProc::setNecessaryDatas(ParticleInfo * _particleInfo, 
	std::string * _particleName, std::map<std::string, DrawableObject2*>* _drawableObjsMap,
	std::map<std::string, HWND>* _elementsMap)
{
	particleInfo = _particleInfo;
	particleName = _particleName;
	drawableObjsMap = _drawableObjsMap;
	elementsMap = _elementsMap;
}
