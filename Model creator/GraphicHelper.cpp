#include "GraphicHelper.h"

vector3ub GraphicHelper::getPixColor(HWND hWnd)
{
	GLint    viewport[4];    // параметры viewport-a.
	GLdouble projection[16]; // матрица проекции.
	GLdouble modelview[16];  // видовая матрица.

	glGetIntegerv(GL_VIEWPORT, viewport);           // узнаём параметры viewport-a.
	glGetDoublev(GL_PROJECTION_MATRIX, projection); // узнаём матрицу проекции.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // узнаём видовую матрицу.

	GLdouble vx, vy;       // координаты курсора мыши в системе координат viewport-a.

	POINT cursorPos;
	GetCursorPos(&cursorPos);


	RECT cRect;
	GetClientRect(hWnd, &cRect);

	POINT point;
	point.x = cRect.left;
	point.y = cRect.top;
	ClientToScreen(hWnd, &point);

	RECT winRect;
	GetWindowRect(hWnd, &winRect);

	vx = cursorPos.x - point.x;
	vy = winRect.bottom - cursorPos.y - 1 - 8;

	glLoadIdentity();

	vector3ub color;
	glReadPixels(vx, vy, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color.v[0]);

	return color;
}

NameIndexId GraphicHelper::getSelectedObj(
	HWND hWnd, const CameraLocation & cam,
	DrawableObjectsPresenter2 * DOPresenter, 
	std::map<int, vector3ub> &colorMapForObjects)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();

	glRotatef(cam.getYAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(cam.getXAngle(), 0.0f, 1.0f, 0.0f);

	glTranslatef(cam.getX(), cam.getY(), cam.getZ());      // Перенос В/Вне экрана по z

	DOPresenter->simpleShow(colorMapForObjects);


	vector3ub color = getPixColor(hWnd);

	for (auto it = colorMapForObjects.begin(); it != colorMapForObjects.end(); ++it)
		if (it->second == color) {
			auto nameIndex = DOPresenter->getNameIndexById(it->first);
			return {nameIndex.first, nameIndex.second, it->first};
		}
	return{"error", -1, -1};
}

std::pair<int, Vect3f*> GraphicHelper::getSelectedVertex(HWND hWnd, const CameraLocation & cam,
	DrawableObjectsPresenter2 * DOPresenter, 
	std::map<std::string, std::map<int, int>> &currentObjs,
	std::map<int, std::map<Vect3f*, vector3ub>> &colorMapForVertexes)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();

	glRotatef(cam.getYAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(cam.getXAngle(), 0.0f, 1.0f, 0.0f);

	glTranslatef(cam.getX(), cam.getY(), cam.getZ());      // Перенос В/Вне экрана по z

	DOPresenter->simpleShowVertexes(currentObjs, colorMapForVertexes);


	vector3ub color = getPixColor(hWnd);

	for (auto it_id = colorMapForVertexes.begin(); it_id != colorMapForVertexes.end(); ++it_id)
		for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it)
			if (it->second == color)
				return { it_id->first, it->first };
	return { -1, nullptr };
}

int GraphicHelper::getSelectedOpenGL_Base(HWND hWnd, const CameraLocation & cam, 
	DrawableObject2 * drawableObj)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();

	glRotatef(cam.getYAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(cam.getXAngle(), 0.0f, 1.0f, 0.0f);

	glTranslatef(cam.getX(), cam.getY(), cam.getZ());      // Перенос В/Вне экрана по z

	//DOPresenter->simpleShowVertexes(currentObjs, colorMapForVertexes);
	
	std::vector<vector3ub> colorVect;
	vector3ub color = { 0, 0, 1 };


	auto it_vertPos = drawableObj->getVertPosition()->begin();
	auto it_vertRot = drawableObj->getVertRotation()->begin();

	glPushMatrix();

	glTranslatef(*drawableObj->getPosition());
	glRotatef(drawableObj->getRotation());
	for (auto it = drawableObj->getVertexes()->begin(); it != drawableObj->getVertexes()->end(); ++it) {
		glPushMatrix();

		glTranslatef(**it_vertPos);
		glRotatef(**it_vertRot);

		(*it)->simpleDraw(color);

		glPopMatrix();
		//glEnd();

		it_vertPos++;
		it_vertRot++;
		color = to_vector3ub(color + 1);
	}


	vector3ub resColor = getPixColor(hWnd);

	if (resColor != 0 && resColor <= color)
		return resColor - 1;
	return -1;
}
