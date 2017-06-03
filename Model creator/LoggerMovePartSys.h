#ifndef LOGGER_MOVE_PARTICLE_SYS_H
#define LOGGER_MOVE_PARTICLE_SYS_H

#include <iostream>

#include "ParticleSystem.h"
#include "Vect3f.h"

#include "BaseLoggerObject.h"

class LoggerMovePartSys :
	public BaseLoggerObject
{
	Vect3f shift;
	ParticleSystem *partSys;

	LoggerMovePartSys(const LoggerMovePartSys&) = delete;
	void operator=(const LoggerMovePartSys&) = delete;
public:
	LoggerMovePartSys(ParticleSystem *partSys, Vect3f shift);

	ParticleSystem *getParticleSys();

	Vect3f getShift() const;
};

#endif LOGGER_MOVE_PARTICLE_SYS_H