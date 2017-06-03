#include "DlgHelper.h"

GLfloat DlgHelper::getValFromEdit(HWND hDlg, int e)
{
	TCHAR e_str[250];// = new TCHAR[15];

	auto hWnd = GetDlgItem(hDlg, e);

	GetWindowText(hWnd, e_str, 250);
	GLfloat val = ::atof(e_str);

	return val;
}

Vect3f DlgHelper::getValFromEdits(HWND hDlg, int e1, int e2, int e3)
{
	HWND edit_x, edit_y, edit_z;

	TCHAR str_x[50];
	TCHAR str_y[50];
	TCHAR str_z[50];

	edit_x = GetDlgItem(hDlg, e1);
	edit_y = GetDlgItem(hDlg, e2);
	edit_z = GetDlgItem(hDlg, e3);

	GetWindowText(edit_x, str_x, 50);
	GetWindowText(edit_y, str_y, 50);
	GetWindowText(edit_z, str_z, 50);

	GLfloat x = ::atof(str_x);
	GLfloat y = ::atof(str_y);
	GLfloat z = ::atof(str_z);

	return Vect3f(x, y, z);
}

void DlgHelper::setValForEdit(HWND hDlg, int e, GLfloat val)
{
	SendDlgItemMessage(hDlg, e, WM_SETTEXT, 0, (LPARAM)std::to_string(val).c_str());
}
