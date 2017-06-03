#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <vector>
#include <map>
#include "Vect4f.h"
#include "CameraLocation.h"

class SelectedCube;
class CameraLocation;

class Drawable {
public:
	//virtual SelectedCube draw(CameraLocation const &cam) { SelectedCube a; return a; };
};

struct LightGL {

	LightGL(GLfloat _LightAmbient[4], GLfloat _LightDiffuse[4], GLfloat _LightPosition[4]) {
		for (int i = 0; i < 4; ++i) {
			LightAmbient[i] = _LightAmbient[i];
			LightDiffuse[i] = _LightDiffuse[i];
			LightPosition[i] = _LightPosition[i];
		}
	}

	GLfloat LightAmbient[4] = { 0.5f, 0.5f, 0.5f, 1.f };

	GLfloat LightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света ( НОВОЕ )

	GLfloat LightPosition[4] = { 0.0f, .0f, 0.0f, .0f };     // Позиция света ( НОВОЕ )
};


struct AddVars {
	bool	active = FALSE;
	bool	keys[256];
	int		wheelDelta = 0;
};

struct vector3ub
{
	GLubyte v[3];
	bool operator==(vector3ub color2);
	bool operator<(vector3ub  &color2);

	operator long();
};

struct VertexKey {
	int id;
	Vect3f *pos;

	//bool operator<(const VertexKey vertKey);
};

struct NameIndexId {
	std::string name;
	int index;
	int id;
};

struct MapOldNewScal {
	std::map<Vect3f*, Vect3f> *shifts;
	Vect3f oldScale;
	Vect3f newScale;

	MapOldNewScal(std::map<Vect3f*, Vect3f> *shifts, Vect3f oldScale, Vect3f newScale);
};

struct ColorTexture {
	std::vector<Vect4f*> *colors;
	std::string pathToTex;
	GLuint *texture;
	std::vector<Vect2f*> *texCoord;

	ColorTexture(std::vector<Vect4f*> *colors, std::string pathToTex,
		GLuint *texture, std::vector<Vect2f*> *texCoord);

private:
	ColorTexture(const ColorTexture&) = delete;
	void operator=(const ColorTexture&) = delete;
};

struct StructForCreateObj {
	std::string presenterName;
	LPTSTR objName;
	CameraLocation cam;
	int index;
};

struct AreaForParticleSys {
	std::string type;
	GLfloat a;
	GLfloat b;
	GLfloat c;
	Vect3f areaPos;
	Vect3f areaRot;
	GLfloat chanceToAppear;
};

class DrawableObject2;

struct ParticleInfo {
	int numOfParticles = 0;
	int speedOfOccur = 0; // in microseconds
	Vect3f particleFluct = Vect3f(0,0,0);
	Vect3f particleFluctLen = Vect3f(0, 0, 0);;
	Vect3f particleRot = Vect3f(0, 0, 0);;
	GLfloat particleSpeed = 0.0f;
	std::string particleName;
	DrawableObject2 * particle = nullptr;
};

#endif STRUCTS_H
