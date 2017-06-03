#ifndef DLG_HELPER_H
#define DLG_HELPER_H

#include <string>

#include "Vect3f.h"

class DlgHelper
{

public:
	static GLfloat getValFromEdit(HWND hDlg, int e);

	static Vect3f getValFromEdits(HWND hDlg, int e1, int e2, int e3);

	static void setValForEdit(HWND hDlg, int e, GLfloat val);
};

#endif DLG_HELPER_H