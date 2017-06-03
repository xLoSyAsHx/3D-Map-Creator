#include "Particle.h"


std::map<Vect3f*, vector3ub> Particle::colorMapForVertexes;
std::set<OpenGL_Base*> Particle::OGLB_ForMappedTexture;
std::pair<int, Vect3f*> Particle::vertForLink = { -1, nullptr };



Particle::Particle(Vect3f curPos, Vect3f endPos,
	DrawableObject2 * obj) :
	curPos(curPos), endPos(endPos), obj(obj)
{
	update();
}

void Particle::setCurPos(Vect3f vect)
{
	curPos = vect;
}

void Particle::setEndPos(Vect3f vect)
{
	endPos = vect;
}

void Particle::move(Vect3f particleFluct, Vect3f particleFluctLen,
	Vect3f particleRot, GLfloat particleSpeed)
{
	auto partShift = direct.mul(particleSpeed);

	if (abs(curPos.getX() - endPos.getX()) < partShift.getX() + 0.1 &&
		abs(curPos.getY() - endPos.getY()) < partShift.getY() + 0.1 &&
		abs(curPos.getZ() - endPos.getZ()) < partShift.getZ() + 0.1)
		canDelete = true;




	curPos = curPos.sum(partShift);
	obj->setRotation(obj->getRotation().sum(particleRot));

	GLfloat x_koef = sinf(x_alpha);
	GLfloat y_koef = sinf(y_alpha);
	GLfloat z_koef = sinf(z_alpha);

	Vect3f shift(
		particleFluctLen.getX() * x_koef,
		particleFluctLen.getY() * y_koef,
		particleFluctLen.getZ() * z_koef);

	obj->setPosition(curPos.sum(shift));

	x_alpha += particleFluct.getX();
	y_alpha += particleFluct.getY();
	z_alpha += particleFluct.getZ();

	obj->updateCoords();
}

bool Particle::draw(CameraLocation & cam)
{
	
	obj->draw(cam, false, colorMapForVertexes, vertForLink, OGLB_ForMappedTexture);

	if (canDelete)
		return true;
	return false;
}

void Particle::update()
{
	canDelete = false;

	direct = endPos.minus(curPos);
	for (char key = 'x'; key <= 'z'; ++key)
		direct.set(key, direct.get(key) / 1000);

}

