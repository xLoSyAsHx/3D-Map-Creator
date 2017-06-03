#include "helper2.h"



void objectsControl(MoveBtnsController& moveBtnsControl, DrawableObjectsPresenter2 *DOPresenter,
	std::map<std::string, std::map<int, int>>  & currentObjs, bool(keys[][256]),
	int &wheelDelta, std::map<std::string, HWND> &elementsMap, Logger &logger,
	int cam_speed_el) {//, 
					  //std::pair<int, std::string> currentObj, DrawableObjectsPresenter* DOPresenter) {
					  //Объект
					  //---------------------------------------------------
					  //std::pair<int, std::string> a;
	
	bool isObjMove = false;
	static bool canLogObjsState = false;
	static Vect3f shiftForLog;
	static auto time = std::chrono::system_clock::now();
	if (currentObjs.size() != 0)
		if ((*keys)[VK_CONTROL] && !(*keys)[VK_LSHIFT])
		{
			
			time = std::chrono::system_clock::now();
			bool canUseW = moveBtnsControl.canUse('W');
			bool canUseS = moveBtnsControl.canUse('S');
			bool canUseA = moveBtnsControl.canUse('A');
			bool canUseD = moveBtnsControl.canUse('D');

			//
			GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
			if ((*keys)['W'])
			{
				if (canUseW)
					z -= 1;
			}
			else
				moveBtnsControl.setKeyState('W', false);

			if ((*keys)['S'])
			{
				if (canUseS)
					z += 1;
			}
			else
				moveBtnsControl.setKeyState('S', false);

			if ((*keys)['A'])
			{
				if (canUseA)
					x -= 1;
			}
			else
				moveBtnsControl.setKeyState('A', false);

			if ((*keys)['D'])
			{
				if (canUseD)
					x += 1;
			}
			else
				moveBtnsControl.setKeyState('D', false);

			if (wheelDelta != 0)
			{
				y += wheelDelta / abs(wheelDelta);
				wheelDelta = 0;
			}
			//
			if (x != 0.0f || y != 0.0f || z != 0.0f)
				for (auto it = currentObjs.begin(); it != currentObjs.end(); ++it)
					for (auto id_iter = it->second.begin(); id_iter != it->second.end(); ++id_iter) {
						auto obj = DOPresenter->getObj(it->first, id_iter->first);
						obj->move(x, y, z);
						obj->updateCoords();
						obj->updateCube();
					}

			if (x != 0.0f || y != 0.0f || z != 0.0f) {
				isObjMove = true;
				canLogObjsState = true;

				if (elementsMap["EditDlg"])
					SendMessage(elementsMap["EditDlg"], PD::WM_CHECHOBJDATA, 0, 0);
			}
			shiftForLog.move(x, y, z);
			
		}
	if (!isObjMove) {
		auto curTime = std::chrono::system_clock::now();
		int miliSec = std::chrono::duration_cast<std::chrono::milliseconds> (curTime - time).count();
		if (miliSec > 100 && canLogObjsState)
		{
			std::vector<DrawableObject2*> *vect = new std::vector<DrawableObject2*>();
			for (auto it = currentObjs.begin(); it != currentObjs.end(); ++it)
				for (auto id_iter = it->second.begin(); id_iter != it->second.end(); ++id_iter)
					vect->push_back(DOPresenter->getObj(it->first, id_iter->second));
			logger.log(vect, &shiftForLog, Logger::Move_Objs);

			shiftForLog = Vect3f(0.0f, 0.0f, 0.0f);
			canLogObjsState = false;
		}
	}
	

	
}

void particleSysControl(MoveBtnsController & moveBtnsControl,
	ParticleSystemPresenter * PSPresenter, ParticleSystem * currentPartSys, bool(keys[][256]),
	int & wheelDelta, std::map<std::string, HWND>& elementsMap, Logger & logger, int cam_speed_el)
{
	bool isObjMove = false;
	static bool canLogObjsState = false;
	static Vect3f shiftForLog;
	static auto time = std::chrono::system_clock::now();
	if (currentPartSys != nullptr)
		if ((*keys)[VK_CONTROL] && !(*keys)[VK_LSHIFT])
		{

			time = std::chrono::system_clock::now();
			bool canUseW = moveBtnsControl.canUse('W');
			bool canUseS = moveBtnsControl.canUse('S');
			bool canUseA = moveBtnsControl.canUse('A');
			bool canUseD = moveBtnsControl.canUse('D');

			//
			GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
			if ((*keys)['W'])
			{
				if (canUseW)
					z -= 1;
			}
			else
				moveBtnsControl.setKeyState('W', false);

			if ((*keys)['S'])
			{
				if (canUseS)
					z += 1;
			}
			else
				moveBtnsControl.setKeyState('S', false);

			if ((*keys)['A'])
			{
				if (canUseA)
					x -= 1;
			}
			else
				moveBtnsControl.setKeyState('A', false);

			if ((*keys)['D'])
			{
				if (canUseD)
					x += 1;
			}
			else
				moveBtnsControl.setKeyState('D', false);

			if (wheelDelta != 0)
			{
				y += wheelDelta / abs(wheelDelta);
				wheelDelta = 0;
			}
			//
			if (x != 0.0f || y != 0.0f || z != 0.0f) {
				currentPartSys->move(x, y, z);
				currentPartSys->update();
				currentPartSys->updateCube();
			}

			if (x != 0.0f || y != 0.0f || z != 0.0f) {
				isObjMove = true;
				canLogObjsState = true;

				if (elementsMap["EditDlg"])
					SendMessage(elementsMap["EditDlg"], PD::WM_CHECHOBJDATA, 0, 0);
			}
			shiftForLog.move(x, y, z);

		}
	if (!isObjMove) {
		auto curTime = std::chrono::system_clock::now();
		int miliSec = std::chrono::duration_cast<std::chrono::milliseconds> (curTime - time).count();
		if (miliSec > 100 && canLogObjsState)
		{
			logger.log(currentPartSys, &shiftForLog, Logger::Move_PartSys);

			shiftForLog = Vect3f(0.0f, 0.0f, 0.0f);
			canLogObjsState = false;
		}
	}
}

void cameraControl(MoveBtnsController& moveBtnsControl,	bool(keys[][256]),
	int &wheelDelta, std::map<std::string, HWND> &elementsMap, int cam_speed_el)
{
	//Камера
	//---------------------------------------------------

	GLfloat temp_x = 0, temp_y = 0, temp_z = 0;
	if (!(*keys)[VK_CONTROL] && !(*keys)[VK_LSHIFT])
	{
		if ((*keys)['W'])
			temp_x = 1.0f;

		if ((*keys)['S'])
			temp_x = -1.0f;

		if ((*keys)['A'])
			temp_z = -1.0f;

		if ((*keys)['D'])
			temp_z = 1.0f;

		if (wheelDelta != 0)
		{
			temp_y += GLfloat(wheelDelta) / abs(wheelDelta);
			wheelDelta = 0;
		}
	}

	//Мышь
	//---------------------------------------------------
	CameraLocation& cam = moveBtnsControl.getCamera();
	//CameraLocation* cam = moveBtnsControl.getCamera();

	if ((*keys)[VK_LBUTTON] && !(*keys)[VK_CONTROL] && !(*keys)[VK_LSHIFT])
		//if (!(*keys)[VK_CONTROL])
	{

		if (!moveBtnsControl.isPresssed(VK_LBUTTON))
		{
			moveBtnsControl.setKeyState(VK_LBUTTON, true);
			cam.updateMousePos();
		}

		cam.defAngles();
		cam.defRotVect();
	}
	else
		moveBtnsControl.setKeyState(VK_LBUTTON, false);

	if (temp_x != 0.0f || temp_y != 0.0f || temp_z != 0.0f)
	{
		GLfloat speed = DlgHelper::getValFromEdit(elementsMap["ObjDlg"], cam_speed_el);

		if (speed > 100 || speed < 10)
			speed = 20;

		cam.updatePos(temp_x, temp_y, temp_z, speed);
	}
}

