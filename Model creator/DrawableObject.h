#ifndef DRAWABLE_OBJECT
#define DRAWABLE_OBJECT

#include <iostream>
#include <vector>
#include <fstream>
#include "helper.h"

class DrawableObject
{
	int mode;
	Vect position, rotation;
	std::vector<Vect> vertexes3f;
public:
	DrawableObject(std::vector<Vect>& vertxMas, int mode);

	void draw() const;

	Vect getPosition() const;

	Vect getRotation() const;

	~DrawableObject();

	static DrawableObject* loadFromFile(std::string path);
};

#endif DRAWABLE_OBJECT