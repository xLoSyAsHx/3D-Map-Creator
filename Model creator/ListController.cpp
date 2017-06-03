#include "ListController.h"


void ListController::addObjToCurObjs(HWND hDlg, HWND list,
	DrawableObjectsPresenter2* DOPresenter, 
	std::map<std::string, std::map<int, int>> &currentObjs, int id)
{
	auto pair = DOPresenter->getNameIndexById(id);

	if (pair.second == -1)
		return;

	addObjToCurObjs(currentObjs, pair.first.c_str(), pair.second, id);

	std::string text = pair.first + "#" + std::to_string(pair.second);

	int idInList = SendMessage(list, LB_FINDSTRING, (WPARAM)-1, (LPARAM)text.c_str());
	SendMessage(list, LB_SETSEL, (WPARAM)(BOOL)TRUE, idInList);
}

void ListController::addObjToCurObjs(
	std::map<std::string, std::map<int, int>>& currentObjs,
	const char * objName, int intNum, int objId)
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

void ListController::applyChangesForMultList(HWND hDlg, HWND list,
	DrawableObjectsPresenter2 * DOPresenter,
	std::map<std::string, std::map<int, int>>& currentObjs)
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

		addObjToCurObjs(currentObjs, objName, intNum, objId);

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
	}

	delete LBString;
}

void ListController::applyChangesForSingleList(HWND hDlg, HWND list,
	DrawableObjectsPresenter2 *DOPresenter,
	std::map<std::string, std::map<int, int>>& currentObjs)
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

	SendMessage(list, LB_SETSEL, (WPARAM)(BOOL)FALSE, (LPARAM)(UINT)-1);

	currentObjs.clear();

	if (val > 0) {
		SendMessage(list, LB_SETSEL, (WPARAM)(BOOL)TRUE, idCurSel);

		addObjToCurObjs(currentObjs, objName, intNum, objId);
	}

	delete LBString;
}

void ListController::checkEqualityBetweenCurObjsAndList(HWND hDlg, HWND list,
	DrawableObjectsPresenter2 *DOPresenter,
	std::map<std::string, std::map<int, int>> &currentObjs,
	int checkedBoxIsMultiple)
{
	int curObjsSize = currentObjs.size();

	int numOfItems = SendMessage(list, LB_GETSELCOUNT, 0, 0);

	bool isMultiple = IsDlgButtonChecked(hDlg, checkedBoxIsMultiple);

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
		applyChangesForMultList(hDlg, list, DOPresenter, currentObjs);
	else
		applyChangesForSingleList(hDlg, list, DOPresenter, currentObjs);
}