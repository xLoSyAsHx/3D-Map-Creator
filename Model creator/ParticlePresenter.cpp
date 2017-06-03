#include "ParticlePresenter.h"

ParticlePresenter::ParticlePresenter(Vect3f particleFluct, Vect3f particleFluctLen,
	Vect3f particleRot, GLfloat particleSpeed) :
	particleFluct(particleFluct), particleFluctLen(particleFluctLen),
	particleRot(particleRot), particleSpeed(particleSpeed)
{
	particles = new std::vector<Particle*>();
}

void ParticlePresenter::addParticle(Particle * particle)
{
	particles->push_back(particle);
}

void ParticlePresenter::moveParticles()
{
	for (auto it = particles->begin(); it != particles->end(); ++it) {
		(*it)->move(particleFluct, particleFluctLen,
		particleRot, particleSpeed);
	}
}

void ParticlePresenter::draw(std::vector<int> &vect, CameraLocation & cam)
{
	int id = 0;
	for (auto it = particles->begin(); it != particles->end(); ++it) {
		bool checker = (*it)->draw(cam);
		if (checker != false)
			vect.push_back(id);
		id++;
	}
}

void ParticlePresenter::updateParticle(int id, Vect3f startPos, Vect3f endPos)
{
	(*particles)[id]->setCurPos(startPos);
	(*particles)[id]->setEndPos(endPos);
	(*particles)[id]->update();
}
