#ifndef DRAWABLE_OBJECTS_PRESENTER_2_H
#define DRAWABLE_OBJECTS_PRESENTER_2_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include "DrawableObject2.h"
#include "CameraLocation.h"

class Loader;

class DrawableObjectsPresenter2
{
	std::map<std::string, std::map<int, int>> *objMapIndex; //triangle 1, id = 4
	std::vector<DrawableObject2*> objMap;
	HWND hWnd;

	static OpenGL_Base* generateSimilarOGL_B(OpenGL_Base* base);
public:
	static DrawableObject2* genegateSimilarDrObj(DrawableObject2* obj);

	DrawableObjectsPresenter2* genSimilarDrObjPresenter(CameraLocation & const cam);

	DrawableObjectsPresenter2();

	DrawableObjectsPresenter2(std::map<std::string, std::map<int, int>> *objMapIndex, std::vector<DrawableObject2*> objMap);

	HWND getMainHWND() const;

	void setMainHWND(HWND _hWnd);

	int getobjMapSize();

	int getNumOfObjs();

	bool isObjExist(std::string name, int num);

	DrawableObject2 *getObj(int id);

	DrawableObject2 *getObj(std::string name, int num);

	int getObjId(std::string name, int index);

	std::pair<std::string, int> getNameIndexById(int id);

	int addObj(DrawableObject2* obj, std::string name, CameraLocation const &cam);

	void addAlreadyExistObj(DrawableObject2* obj, std::string name, int index);

	std::vector<std::string> getObjNames();

	std::vector<int> getObjectIndexes(std::string name);

	std::map<std::string, std::map<int, int>>*  getObjMapIndex();

	void eraseObjFromObjMap(int i);

	void clear();

	std::pair<int, OpenGL_Base*> getOJLBByIdAndPosVertPtr(std::pair<int, Vect3f*>& pair);

	void show(CameraLocation const &cam,
		std::map<std::string, std::map<int, int>> &selectedMap, bool isVertexesEnable, 
		std::map<int, std::map<Vect3f*, vector3ub>> &colorMapForVertexes,
		std::pair<int, Vect3f*> &vertForLink,
		std::map<int, std::set<OpenGL_Base*>> &OGLB_ForMappedTexture);

	void simpleShow(std::map<int, vector3ub> &colorMapForObjects);

	void simpleShowVertexes(std::map<std::string, std::map<int, int>> currentObjs, 
		std::map<int, std::map<Vect3f*, vector3ub>> &colorMapForVecrtexes);


	static vector3ub getColorForSimpleDraw(std::map<DrawableObjectsPresenter2*, std::map<int, vector3ub>>& map);

	static std::string loadFromFile(std::string path, std::map<std::string, DrawableObject2*> &map);

	static void saveToFile(std::string path, DrawableObject2* map);

	~DrawableObjectsPresenter2();


	friend Loader;
};

#endif DRAWABLE_OBJECTS_PRESENTER_2_H
