#ifndef PARTICLE_PRESENTER_H
#define PARTICLE_PRESENTER_H

#include "Particle.h"
#include "Vect3f.h"

#include "DrawableObjectPresenter2.h"

class ParticlePresenter
{
	Vect3f particleFluct;
	Vect3f particleFluctLen;
	Vect3f particleRot;
	GLfloat particleSpeed;
	//DrawableObject2 * particle;

	std::vector<Particle*> *particles;
public:
	ParticlePresenter(Vect3f particleFluct, Vect3f particleFluctLen,
		Vect3f particleRot, GLfloat particleSpeed);

	void addParticle(Particle * particle);

	void moveParticles();

	void draw(std::vector<int> &vect, CameraLocation &cam);

	void updateParticle(int id, Vect3f startPos, Vect3f endPos);
};

#endif PARTICLE_PRESENTER_H