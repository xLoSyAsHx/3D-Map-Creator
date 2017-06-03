#ifndef OPENGL_BASE_H
#define OPENGL_BASE_H

#include <GL\freeglut.h>
#include <iostream>
#include <vector>

#include "Vect4f.h"
#include "helper.h"

class DrawableObjectsPresenter2;

class OpenGL_Base
{
	unsigned int mode;
	std::vector<Vect3f*> *vertexes;

	GLuint *texture;
	std::vector<Vect2f*> *textureCoord;
	std::vector<Vect4f*> *colors;
	std::string pathToTexture;

	void glDrawSquare(GLfloat sideLen, Vect3f &pos, Vect3f &delPos, Vect3f &rot);
	void glDrawCube(GLfloat sideLen, Vect3f &pos, vector3ub &color);
	void clearColorTexture();

public:
	OpenGL_Base();

	int getMode();

	void setMode(unsigned int _mode);
	void setVertexes(std::vector<Vect3f*> *_vertexes);
	void setTexture(GLuint *_texture, std::vector<Vect2f*> *_texCoord, std::string path);
	void setColors(std::vector<Vect4f*> *_colors);

	std::vector<Vect3f*>* const getVertexes();
	GLuint* getTexture();
	std::string getPathToTexture();
	std::vector<Vect2f*>* getTextureCoord();
	std::vector<Vect4f*>* getColors();
	bool hasTexture();

	void draw(bool isVertexesEnable, std::map<Vect3f*, vector3ub> &colorMapForVertexes,
		std::pair<int, Vect3f*> &vertForLink, bool isVertForMapTexOn, GLfloat vertSize);
	void simpleDraw(vector3ub &colorVect);
	void simpleDrawVertexes(std::map<Vect3f*, vector3ub> &colorMap, GLfloat vertSize);

	~OpenGL_Base();

	friend DrawableObjectsPresenter2;
};

#endif OPENGL_BASE_H