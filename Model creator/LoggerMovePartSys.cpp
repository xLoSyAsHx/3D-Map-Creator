#include "LoggerMovePartSys.h"

LoggerMovePartSys::LoggerMovePartSys(ParticleSystem * partSys, Vect3f shift) :
	partSys(partSys), shift(shift)
{
}

ParticleSystem* LoggerMovePartSys::getParticleSys()
{
	return partSys;
}

Vect3f LoggerMovePartSys::getShift() const
{
	return shift;
}
