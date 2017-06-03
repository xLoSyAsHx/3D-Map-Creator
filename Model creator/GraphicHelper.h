#ifndef GRAPHIC_HELPER_H
#define GRAPHIC_HELPER_H

#include <iostream>
#include <map>
#include <vector>

#include "DrawableObjectPresenter2.h"
#include "CameraLocation.h"

class GraphicHelper
{
	static vector3ub getPixColor(HWND hWnd);

public:
	static NameIndexId getSelectedObj(
		HWND hWnd, const CameraLocation & cam,
		DrawableObjectsPresenter2 * DOPresenter,
		std::map<int, vector3ub> &colorMapForObjects);

	static std::pair<int, Vect3f*> getSelectedVertex(HWND hWnd, const CameraLocation & cam,
		DrawableObjectsPresenter2 * DOPresenter,
		std::map<std::string, std::map<int, int>> &currentObjs,
		std::map<int, std::map<Vect3f*, vector3ub>> &colorMapForVertexes);

	static int getSelectedOpenGL_Base(
		HWND hWnd, const CameraLocation & cam,
		DrawableObject2 * drawableObj);
};

#endif GRAPHIC_HELPER_H