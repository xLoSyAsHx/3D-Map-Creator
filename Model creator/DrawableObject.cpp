#include "DrawableObject.h"

DrawableObject::DrawableObject(std::vector<Vect>& vertxMas, int mode) :
	vertexes3f(vertxMas), mode(mode)
{
}

void DrawableObject::draw() const
{
	glBegin(mode);

	for (auto obj : vertexes3f)
		glVertex3f(obj.getX(), obj.getY(), obj.getZ());

	glEnd();
}

Vect DrawableObject::getPosition() const
{
	return position;
}

Vect DrawableObject::getRotation() const
{
	return rotation;
}

DrawableObject::~DrawableObject()
{
}

DrawableObject *DrawableObject::loadFromFile(std::string path)
{
	std::fstream fin(path);

	if (!fin.is_open()) {
		char* err = "\nErr: DrawableObject::loadFromFile\tcan't open file.";

		dbgprintf(err);
		fin.close();
		return nullptr;
	}

	/*
	glMode
	num_of_points
	x y z
	...
	x y z
	*/
	int mode;
	fin >> mode;

	int size;
	fin >> size;

	std::vector<Vect> vertxMas;
	vertxMas.resize(size);
	for (int i = 0; i < size; ++i) {
		float x, y, z;
		fin >> x >> y >> z;
		vertxMas[i] = { x, y, z };
	}
	fin.close();

	return new DrawableObject(vertxMas, mode);
}
