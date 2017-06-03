#include "Logger.h"

void Logger::check()
{
	//if (index == -1) 
		//return;

	int size = (sysLog.size()) - 1;
	if (index != size)
		for (int i = index; i < size; ++i) {
			//sysLog.erase(sysLog.begin() + index + 1);
			delete sysLog[index + 1];
			sysLog.erase(sysLog.begin() + (index + 1));
		}
}

void Logger::applyChangesFromLog()
{
	Logger::LogType type;
	if (index == -1)
		type = logTypeVect[index + 1];
	else
		type = logTypeVect[index];

	switch (type)
	{
	case Logger::Move_Objs:
	{
		LoggerMoveObjs * curObj = static_cast<LoggerMoveObjs *> (sysLog[index]);
		
		auto vect = curObj->getDrObjsVect();
		for (auto it = vect->begin(); it != vect->end(); ++it) {
			(*it)->move(curObj->getShift());

			(*it)->updateCoords();
			(*it)->updateCube();
		}

		changeDlgData((*elementsMap)["EditDlg"], curObj->getShift(),
			IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z);

		break;
	}

	case Logger::Move_Verts:
	{
		LoggerMoveVerts * curObj = static_cast<LoggerMoveVerts *> (sysLog[index]);

		auto vect = curObj->getVertVect();
		for (auto it = vect->begin(); it != vect->end(); ++it)
			(*it)->move(curObj->getShift());

		changeDlgData((*elementsMap)["EditDlg"], curObj->getShift(),
			IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z);

		updateAllObjs();

		break;
	}

	case Logger::Rot_Objs:
	{
		LoggerRotObjs * curObj = static_cast<LoggerRotObjs *> (sysLog[index]);

		auto vect = curObj->getDrObjsVect();
		for (auto it = vect->begin(); it != vect->end(); ++it) {
			(*it)->move(curObj->getShift());

			(*it)->updateCoords();
			(*it)->updateCube();
		}

		changeDlgData((*elementsMap)["EditDlg"], curObj->getShift(),
			IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z);

		break;
	}

	case Logger::Scal_Objs:
	{
		LoggerScalObjs * curObj = static_cast<LoggerScalObjs *> (sysLog[index]);

		auto vect = curObj->getVertVect();
		auto shifts = curObj->getShifts();
		int i = 0;
		for (auto it_map = vect->begin(); it_map != vect->end(); ++it_map) {
			auto it = it_map->first;
			it->setPosition(it->getPosition()->sum((*shifts)[it->getPosition()]));
			for (auto it = it_map->second->begin(); it != it_map->second->end(); ++it) {
				auto vert = it->first;
				vert->set(vert->sum((*shifts)[it->first].invert().minus(*it_map->first->getPosition())));
			}

			it->updateCoords();
			it->updateCube();
		}

		//Vect3f shiftForDlg = curObj->getNewScale().minus(curObj->getOldScale());
		if (vect->size() == 1)
			vect->begin()->first->setScal(curObj->getNewScale());
		else
		{
			Vect3f change = curObj->getNewScale();
			change.setX(change.getX() / curObj->getOldScale().getX());
			change.setY(change.getY() / curObj->getOldScale().getY());
			change.setZ(change.getZ() / curObj->getOldScale().getZ());

			for (auto it = vect->begin(); it != vect->end(); ++it) {
				auto obj = it->first;
				obj->setScal(obj->getScal().mul(change));
			}
			//vect->begin()->first->setPosition(curObj->getOldScale());
		}

		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], IDC_EDIT_SCAL_X, curObj->getNewScale().getX());
		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], IDC_EDIT_SCAL_Y, curObj->getNewScale().getY());
		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], IDC_EDIT_SCAL_Z, curObj->getNewScale().getZ());


		break;
	}

	case Logger::Set_Texture:
	case Logger::Set_Color:
	{
		LoggerSetTexColor * curObj = static_cast<LoggerSetTexColor *> (sysLog[index]);

		auto vect = curObj->getOGLB_vect();
		auto vectNewStruct = curObj->getNewStruct();
		for (auto it_OGLB = vect->begin(); it_OGLB != vect->end(); ++it_OGLB)
			if (vectNewStruct->texCoord != nullptr)
			{
				auto newTexCoord = new std::vector<Vect2f*>();
				auto coord = vectNewStruct->texCoord;
				for (auto it = coord->begin(); it != coord->end(); ++it)
					newTexCoord->push_back(new Vect2f((*it)->getX(), (*it)->getY()));

				(*it_OGLB)->setTexture(
					vectNewStruct->texture,
					newTexCoord,
					vectNewStruct->pathToTex);
			}
			else
			{
				auto newColors = new std::vector<Vect4f*>();
				auto colors = vectNewStruct->colors;
				for (auto it = colors->begin(); it != colors->end(); ++it)
					newColors->push_back(new Vect4f((*it)->getX(), (*it)->getY(), (*it)->getZ(), (*it)->getAlpha()));

				(*it_OGLB)->setColors(newColors);
			}
			//(*it_OGLB)->setColors(vectNewStruct->colors);

		break;
	}

	case Logger::Create_Obj:
	{
		LoggerCreateDelObj * curObj = static_cast<LoggerCreateDelObj *> (sysLog[index]);

		auto drObj = curObj->getDrObj();
		auto info = curObj->getAdditionalInfo();

		auto DOPresenter = DOPresenterMap[curObj->getPresenterName()];

		DOPresenter->addAlreadyExistObj(drObj, info.objName, info.index);

		int itemIndexInList = SendDlgItemMessage((*elementsMap)["ObjDlg"], IDC_LIST1, LB_ADDSTRING, 0,
			(LPARAM)(std::string(info.objName) + "#" + std::to_string(info.index)).c_str());

		break;
	}

	case Logger::Delete_Obj:
	{
		LoggerCreateDelObj * curObj = static_cast<LoggerCreateDelObj *> (sysLog[index]);

		auto drObj = curObj->getDrObj();
		auto info = curObj->getAdditionalInfo();

		auto DOPresenter = DOPresenterMap[curObj->getPresenterName()];

		auto indexesMap = DOPresenter->getObjMapIndex();

		auto obj = (*indexesMap)[info.objName];

		int indexForDel = -1;
		for (auto it = obj.begin(); it != obj.end(); ++it)
			if (it->first == info.index) {
				indexForDel = it->first;
				break;
			}

		if (indexForDel != -1)
			(*indexesMap)[info.objName].erase(indexForDel);

		if ((*indexesMap)[info.objName].size() == 0)
			indexesMap->erase(info.objName);

		DOPresenter->eraseObjFromObjMap(indexForDel);
		//DOPresenter

		int itemIndexInList = SendDlgItemMessage((*elementsMap)["ObjDlg"], IDC_LIST1, LB_FINDSTRING, 0,
			(LPARAM)(std::string(info.objName) + "#" + std::to_string(info.index)).c_str());

		SendDlgItemMessage((*elementsMap)["ObjDlg"], IDC_LIST1, LB_DELETESTRING, itemIndexInList, 0);

		break;
	}

	case Logger::Move_PartSys:
	{
		LoggerMovePartSys * curObj = static_cast<LoggerMovePartSys *> (sysLog[index + 1]);

		auto partSys = curObj->getParticleSys();

		partSys->move(curObj->getShift());

		break;
	}

	default:
		break;
	}
}

void Logger::rollBackChangesFromLog()
{
	Logger::LogType type;
	//if (index == -1)
		type = logTypeVect[index + 1];
	//else
		//type = logTypeVect[index];

	switch (type)
	{
	case Logger::Move_Objs:
	{
		LoggerMoveObjs * curObj = static_cast<LoggerMoveObjs *> (sysLog[index + 1]);

		auto vect = curObj->getDrObjsVect();
		for (auto it = vect->begin(); it != vect->end(); ++it)
			(*it)->move(curObj->getShift().invert());

		changeDlgData((*elementsMap)["EditDlg"], curObj->getShift().invert(),
			IDC_EDIT_POS_X, IDC_EDIT_POS_Y, IDC_EDIT_POS_Z);

		break;
	}

	case Logger::Move_Verts:
	{
		LoggerMoveVerts * curObj = static_cast<LoggerMoveVerts *> (sysLog[index + 1]);

		auto vect = curObj->getVertVect();
		for (auto it = vect->begin(); it != vect->end(); ++it)
			(*it)->move(curObj->getShift().invert());
		
		changeDlgData((*elementsMap)["EditDlg"], curObj->getShift().invert(),
			IDC_EDIT_VPOS_X, IDC_EDIT_VPOS_Y, IDC_EDIT_VPOS_Z);

		updateAllObjs();

		break;
	}

	case Logger::Rot_Objs:
	{
		LoggerRotObjs * curObj = static_cast<LoggerRotObjs *> (sysLog[index + 1]);

		auto vect = curObj->getDrObjsVect();
		for (auto it = vect->begin(); it != vect->end(); ++it) {
			(*it)->move(curObj->getShift().invert());

			(*it)->updateCoords();
			(*it)->updateCube();
		}

		changeDlgData((*elementsMap)["EditDlg"], curObj->getShift().invert(),
			IDC_EDIT_ROT_X, IDC_EDIT_ROT_Y, IDC_EDIT_ROT_Z);

		break;
	}

	case Logger::Scal_Objs:
	{
		LoggerScalObjs * curObj = static_cast<LoggerScalObjs *> (sysLog[index + 1]);

		auto vect = curObj->getVertVect();
		auto shifts = curObj->getShifts(); 
		for (auto it_map = vect->begin(); it_map != vect->end(); ++it_map) {
			for (auto it = it_map->second->begin(); it != it_map->second->end(); ++it) {
				auto vert = it->first;
				vert->set(vert->sum((*shifts)[it->first].invert().sum(*it_map->first->getPosition())));
			}
			auto it = it_map->first;
			it->setPosition(it->getPosition()->minus((*shifts)[it->getPosition()]));
		}

		if (vect->size() == 1)
			vect->begin()->first->setScal(curObj->getOldScale());
		else
		{
			Vect3f change = curObj->getOldScale();
			change.setX(change.getX() / curObj->getNewScale().getX());
			change.setY(change.getY() / curObj->getNewScale().getY());
			change.setZ(change.getZ() / curObj->getNewScale().getZ());

			for (auto it = vect->begin(); it != vect->end(); ++it) {
				auto obj = it->first;
				obj->setScal(obj->getScal().mul(change));
			}
			//vect->begin()->first->setPosition(curObj->getOldScale());
		}

		for (auto it = vect->begin(); it != vect->end(); ++it) {
			auto obj = it->first;
			obj->updateCoords();
			obj->updateCube();
		}
		//Vect3f shiftForDlg = curObj->getNewScale().minus(curObj->getOldScale()).invert();
		
		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], IDC_EDIT_SCAL_X, curObj->getOldScale().getX());
		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], IDC_EDIT_SCAL_Y, curObj->getOldScale().getY());
		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], IDC_EDIT_SCAL_Z, curObj->getOldScale().getZ());
		
		/*
		changeDlgData((*elementsMap)["EditDlg"], shiftForDlg,
			IDC_EDIT_SCAL_X, IDC_EDIT_SCAL_Y, IDC_EDIT_SCAL_Z);
			*/
		break;
	}

	case Logger::Set_Texture:
	case Logger::Set_Color:
	{
		LoggerSetTexColor * curObj = static_cast<LoggerSetTexColor *> (sysLog[index + 1]);

		auto vect = curObj->getOGLB_vect();
		auto vectOldStruct = curObj->getOldStruct();
		for (int i = 0; i < vect->size(); ++i)
			if ((*vectOldStruct)[i]->texCoord != nullptr)
			{
				auto newTexCoord = new std::vector<Vect2f*>();
				auto coord = (*vectOldStruct)[i]->texCoord;
				for (auto it = coord->begin(); it != coord->end(); ++it)
					newTexCoord->push_back(new Vect2f((*it)->getX(), (*it)->getY()));

				(*vect)[i]->setTexture(
					(*vectOldStruct)[i]->texture,
					newTexCoord,
					(*vectOldStruct)[i]->pathToTex);
			}
			else
			{
				auto newColors = new std::vector<Vect4f*>();
				auto colors = (*vectOldStruct)[i]->colors;
				for (auto it = colors->begin(); it != colors->end(); ++it)
					newColors->push_back(new Vect4f((*it)->getX(), (*it)->getY(), (*it)->getZ(), (*it)->getAlpha()));

				(*vect)[i]->setColors(newColors);
			}
				//(*vect)[i]->setColors((*vectOldStruct)[i]->colors);

		break;
	}

	case Logger::Create_Obj:
	{
		LoggerCreateDelObj * curObj = static_cast<LoggerCreateDelObj *> (sysLog[index + 1]);

		auto drObj = curObj->getDrObj();
		auto info = curObj->getAdditionalInfo();

		auto DOPresenter = DOPresenterMap[curObj->getPresenterName()];

		auto indexesMap = DOPresenter->getObjMapIndex();

		auto obj = (*indexesMap)[info.objName];

		int indexForDel = -1;
		int idForDel = -1;
		for (auto it = obj.begin(); it != obj.end(); ++it)
			if (it->first == info.index) {
				indexForDel = it->first;
				idForDel = it->second;
				break;
			}

		if (indexForDel != -1)
			(*indexesMap)[info.objName].erase(indexForDel);

		if ((*indexesMap)[info.objName].size() == 0)
			indexesMap->erase(info.objName);

		DOPresenter->eraseObjFromObjMap(idForDel);
		//DOPresenter

		int itemIndexInList = SendDlgItemMessage((*elementsMap)["ObjDlg"], IDC_LIST1, LB_FINDSTRING, 0,
			(LPARAM)(std::string(info.objName) + "#" + std::to_string(info.index)).c_str());

		SendDlgItemMessage((*elementsMap)["ObjDlg"], IDC_LIST1, LB_DELETESTRING, itemIndexInList, 0);

		break;
	}

	case Logger::Delete_Obj:
	{
		LoggerCreateDelObj * curObj = static_cast<LoggerCreateDelObj *> (sysLog[index + 1]);

		auto drObj = curObj->getDrObj();
		auto info = curObj->getAdditionalInfo();

		auto DOPresenter = DOPresenterMap[curObj->getPresenterName()];

		DOPresenter->addAlreadyExistObj(drObj, info.objName, info.index);

		int itemIndexInList = SendDlgItemMessage((*elementsMap)["ObjDlg"], IDC_LIST1, LB_ADDSTRING, 0,
			(LPARAM)(std::string(info.objName) + "#" + std::to_string(info.index)).c_str());

		break;
	}

	case Logger::Move_PartSys:
	{
		LoggerMovePartSys * curObj = static_cast<LoggerMovePartSys *> (sysLog[index + 1]);

		auto partSys = curObj->getParticleSys();

		partSys->move(curObj->getShift().invert());

		break;
	}

	default:
		break;
	}
}

void Logger::changeDlgData(bool isDlgExist, Vect3f iShift, int e1, int e2, int e3)
{
	if (isDlgExist) {
		Vect3f editVals = DlgHelper::getValFromEdits((*elementsMap)["EditDlg"], e1, e2, e3);

		editVals = editVals.sum(iShift);

		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], e1, editVals.getX());
		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], e2, editVals.getY());
		DlgHelper::setValForEdit((*elementsMap)["EditDlg"], e3, editVals.getZ());
	}
}

void Logger::updateAllObjs()
{
	for (auto it = DOPresenterMap.begin(); it != DOPresenterMap.end(); ++it) {
		auto objMap = it->second->getObjMapIndex();

		for (auto it_name = objMap->begin(); it_name != objMap->end(); ++it_name)
			for (auto it_index = it_name->second.begin(); it_index != it_name->second.end(); ++it_index) {
				auto obj = it->second->getObj(it_index->second);

				obj->updateCoords();
				obj->updateCube();
			}
	}
}

void Logger::addDrObjPresenter(std::string name, DrawableObjectsPresenter2 * presenter)
{
	DOPresenterMap[name] = presenter;
}

void Logger::setElementsMap(std::map<std::string, HWND>* _map)
{
	elementsMap = _map;
}

void Logger::log(void * obj, void * _struct, LogType type)
{

	switch (type)
	{
	case Logger::Move_Objs:
	{
		std::vector<DrawableObject2*> * curObj;
		Vect3f vect;

		try { 
			curObj = static_cast<std::vector<DrawableObject2*> *> (obj); 
			vect = *(static_cast<Vect3f*> (_struct));
		}
		catch (...) { return; }
		
		check();
		index++;
		logTypeVect.push_back(type);
		sysLog.push_back(new LoggerMoveObjs(curObj, vect));
		
		return;
	}

	case Logger::Move_Verts:
	{
		std::vector<Vect3f*> * curObj;
		Vect3f vect;

		try { 
			curObj = static_cast<std::vector<Vect3f*> *> (obj); 
			vect = *(static_cast<Vect3f*> (_struct));
		}
		catch (...)	{ return; }

		check();
		index++;
		logTypeVect.push_back(type);
		sysLog.push_back(new LoggerMoveVerts(curObj, vect));
		break;
	}

	case Logger::Rot_Objs:
	{
		std::vector<DrawableObject2*> * curObj;
		Vect3f vect;

		try	{ 
			curObj = static_cast<std::vector<DrawableObject2*> *> (obj);
			vect = *(static_cast<Vect3f*> (_struct));
		}
		catch (...) { return; }

		check();
		index++;
		logTypeVect.push_back(type);
		sysLog.push_back(new LoggerRotObjs(curObj, vect));

		break;
	}

	case Logger::Scal_Objs:
	{
		std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>  *curObj;
		MapOldNewScal mapOldNewScal(nullptr, Vect3f(), Vect3f());

		try { 
			curObj = static_cast<std::vector<std::pair<DrawableObject2*, std::map<Vect3f*, Vect3f>*>>  *> (obj);
			mapOldNewScal = *(static_cast<MapOldNewScal *> (_struct));
		}
		catch (...) { return; }

		check();
		index++;
		logTypeVect.push_back(type);
		sysLog.push_back(
			new LoggerScalObjs(curObj, mapOldNewScal.shifts,
				mapOldNewScal.oldScale, mapOldNewScal.newScale)
		);

		break;
	}

	case Logger::Set_Texture:
	case Logger::Set_Color:
	{
		std::pair<std::vector<OpenGL_Base*>*, bool> curObj;
		std::pair<ColorTexture*, std::vector<ColorTexture*>*> pair;

		try {
			curObj = *(static_cast<std::pair<std::vector<OpenGL_Base*>*, bool>*> (obj));
			pair = *(static_cast<std::pair<ColorTexture*, std::vector<ColorTexture*>*> *> (_struct));
		}
		catch (...) { return; }

		check();
		index++;
		logTypeVect.push_back(type);
		sysLog.push_back(
			new LoggerSetTexColor(curObj.first, curObj.second, pair)
		);

		break;
	}

	case Logger::Create_Obj:
	case Logger::Delete_Obj:
	{
		DrawableObject2 *curObj;
		StructForCreateObj info;
		try {
			curObj = static_cast<DrawableObject2*> (obj);
			info = *(static_cast<StructForCreateObj*> (_struct));
		}
		catch (...) { return; }

		check();
		index++;
		logTypeVect.push_back(type);
		sysLog.push_back(
			new LoggerCreateDelObj(curObj, info)
		);

		break;
	}

	case Logger::Move_PartSys:
	{
		ParticleSystem * curObj;
		Vect3f vect;

		try {
			curObj = static_cast<ParticleSystem *> (obj);
			vect = *(static_cast<Vect3f*> (_struct));
		}
		catch (...) { return; }

		check();
		index++;
		logTypeVect.push_back(type);
		sysLog.push_back(new LoggerMovePartSys(curObj, vect));

		break;
	}

	default:
		break;
	}
}

void Logger::forward()
{
	
	if (index < (int)(sysLog.size() - 1)) {
		index++;
		applyChangesFromLog();
	}
}

void Logger::backward()
{
	if (index >= 0) {
		index--;
		rollBackChangesFromLog();
	}
}

void Logger::clear()
{
	for (int i = 0; i < sysLog.size(); ++i) {

		switch (logTypeVect[i])
		{
		case Logger::Move_Objs:
		{
			LoggerMoveObjs * curObj = static_cast<LoggerMoveObjs *> (sysLog[i]);

			delete curObj;

			break;
		}

		case Logger::Move_Verts:
		{
			LoggerMoveVerts * curObj = static_cast<LoggerMoveVerts *> (sysLog[i]);

			delete curObj;

			break;
		}

		case Logger::Rot_Objs:
		{
			LoggerRotObjs * curObj = static_cast<LoggerRotObjs *> (sysLog[i]);

			delete curObj;

			break;
		}

		case Logger::Scal_Objs:
		{
			LoggerScalObjs * curObj = static_cast<LoggerScalObjs *> (sysLog[i]);

			delete curObj;

			break;
		}

		case Logger::Set_Texture:
		case Logger::Set_Color:
		{
			LoggerSetTexColor * curObj = static_cast<LoggerSetTexColor *> (sysLog[i]);

			delete curObj;

			break;
		}

		case Logger::Create_Obj:
		{
			LoggerCreateDelObj * curObj = static_cast<LoggerCreateDelObj *> (sysLog[i]);

			delete curObj;

			break;
		}

		case Logger::Delete_Obj:
		{
			LoggerCreateDelObj * curObj = static_cast<LoggerCreateDelObj *> (sysLog[i]);

			delete curObj;

			break;
		}

		default:
			break;
		}
	}
	sysLog.clear();
	logTypeVect.clear();
	index = -1;
}
