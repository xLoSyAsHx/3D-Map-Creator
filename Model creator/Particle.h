#ifndef PARTICLE_H
#define PARTICLE_H

#include "DrawableObject2.h"

class Particle
{
	bool canDelete = false;

	Vect3f direct;
	Vect3f endPos;
	Vect3f curPos;

	float x_alpha = 0.0f;
	float y_alpha = 0.0f;
	float z_alpha = 0.0f;


	DrawableObject2 *obj;

	static std::map<Vect3f*, vector3ub> colorMapForVertexes;
	static std::set<OpenGL_Base*> OGLB_ForMappedTexture;
	static std::pair<int, Vect3f*> vertForLink;
public:

	Particle(Vect3f curPos, Vect3f endPos, DrawableObject2 * obj);

	void setCurPos(Vect3f vect);

	void setEndPos(Vect3f vect);

	void move(Vect3f particleFluct, Vect3f particleFluctLen,
		Vect3f particleRot, GLfloat particleSpeed);

	bool draw(CameraLocation &cam);

	void update();
};

#endif PARTICLE_H