#include "OpenGL_Base.h"



void OpenGL_Base::glDrawSquare(GLfloat sideLen, Vect3f &pos, Vect3f &delPos, Vect3f &rot)
{
	glPushMatrix();
	glTranslatef(pos);
	glTranslatef(delPos);
	glRotatef(rot);

	glBegin(GL_QUADS);

	glVertex3f(-sideLen / 2, -sideLen / 2, 0);
	glVertex3f(-sideLen / 2, sideLen / 2, 0);
	glVertex3f(sideLen / 2, sideLen / 2, 0);
	glVertex3f(sideLen / 2, -sideLen / 2, 0);
	

	glEnd();

	glPopMatrix();
}

void OpenGL_Base::glDrawCube(GLfloat sideLen, Vect3f & pos, vector3ub &color)
{
	glColor3ub(color);

	glDrawSquare(sideLen, pos, Vect3f(0, 0, sideLen / 2),  Vect3f(0, 0, 0));
	glDrawSquare(sideLen, pos, Vect3f(0, 0, -sideLen / 2), Vect3f(0, 0, 0));
	glDrawSquare(sideLen, pos, Vect3f(-sideLen / 2, 0, 0), Vect3f(0, 90, 0));
	glDrawSquare(sideLen, pos, Vect3f(sideLen / 2, 0, 0),  Vect3f(0, 90, 0));
	glDrawSquare(sideLen, pos, Vect3f(0, sideLen / 2, 0),  Vect3f(90, 0, 0));
	glDrawSquare(sideLen, pos, Vect3f(0, -sideLen / 2, 0), Vect3f(90, 0, 0));
}

void OpenGL_Base::clearColorTexture()
{
	if (textureCoord != nullptr)
	{
		for (auto it = textureCoord->begin(); it != textureCoord->end(); ++it)
			delete *it;

		delete textureCoord;
		textureCoord = nullptr;
		pathToTexture = "";
	}

	if (colors != nullptr)
	{
		for (auto it = colors->begin(); it != colors->end(); ++it)
			delete *it;

		delete colors;
		colors = nullptr;
	}
}

OpenGL_Base::OpenGL_Base()
{
	mode = GL_TRIANGLES;
	vertexes = new std::vector<Vect3f*>;
	texture = nullptr;
	textureCoord = nullptr;
}


OpenGL_Base::~OpenGL_Base()
{
	delete vertexes;

	if (textureCoord != nullptr) {
		delete textureCoord;
		delete texture;
	}
	if (colors != nullptr)
		delete colors;
}

int OpenGL_Base::getMode() { return mode; }

void OpenGL_Base::setMode(unsigned int _mode) { mode = _mode; }

void OpenGL_Base::setVertexes(std::vector<Vect3f*>* _vertexes) { vertexes = _vertexes; }

void OpenGL_Base::setTexture(GLuint * _texture, std::vector<Vect2f*>* _texCoord, std::string path)
{
	clearColorTexture();

	texture = _texture;
	textureCoord = _texCoord;
	pathToTexture = path;
}

void OpenGL_Base::setColors(std::vector<Vect4f*>* _colors) 
{
	clearColorTexture();

	colors = _colors;
}

std::vector<Vect3f*>* const OpenGL_Base::getVertexes() { return vertexes; }

GLuint* OpenGL_Base::getTexture()  { return texture; }

std::string OpenGL_Base::getPathToTexture()
{
	return pathToTexture;
}

std::vector<Vect2f*>* OpenGL_Base::getTextureCoord() { return textureCoord; }

std::vector<Vect4f*>* OpenGL_Base::getColors() { return colors; }

bool OpenGL_Base::hasTexture()
{
	if (texture == nullptr)
		return false;
	return true;
}

void OpenGL_Base::draw(bool isVertexesEnable, std::map<Vect3f*, vector3ub> &colorMapForVertexes,
	std::pair<int, Vect3f*> &vertForLink, bool isVertForMapTexOn, GLfloat vertSize)
{
	std::vector<Vect2f*>::iterator p_it_texCoord;
	std::vector<Vect4f*>::iterator p_it_color;

	bool isTexCoord = false;
	bool isColor = false;

	if (textureCoord != nullptr) {
		glColor3ub(255, 255, 255);
		p_it_texCoord = textureCoord->begin();
		isTexCoord = true;
	}

	if (colors != nullptr) {
		p_it_color = colors->begin();
		isColor = true;
	}

	if (textureCoord != nullptr)
		glBindTexture(GL_TEXTURE_2D, *texture);

	glBegin(mode);

	for (auto obj : *vertexes) {
		if (isTexCoord)
			glTexCoord2f(**p_it_texCoord);
		else if (isColor)
			glColor4ub(**p_it_color);//glColor4f(1, 1, 1, 0.5f);//


		glVertex3f(obj->getX(), obj->getY(), obj->getZ());


		if (isTexCoord)
			p_it_texCoord++;
		else if (isColor)
			p_it_color++;

	}

	glEnd();
	if (isTexCoord)
		glBindTexture(GL_TEXTURE_2D, 0);

	vector3ub color = { 255, 0, 0 };
	//glRotatef(camDir);
	if (isVertForMapTexOn)
	{
		std::vector<vector3ub> colorsForMapTex = { {255, 0, 0}, {0, 255, 0}, {0, 0, 255} };
		for (int i = 0; i < 3; ++i) {
			color = colorsForMapTex[i];
			glDrawCube(vertSize, *((*vertexes)[i]), color);
		}
	}
	else if (isVertexesEnable)
		for (auto obj : *vertexes) 
		{
			if (vertForLink.first != -1 && vertForLink.second == obj)
			{
				color = {200, 0, 200};
			}
			else
			{
				if (colorMapForVertexes.find(obj) != colorMapForVertexes.end())
					color = { 0, 200, 200 };
				else
					color = { 255, 0, 0 };
			}
			glDrawCube(vertSize, *obj, color);
		}
	
}

void OpenGL_Base::simpleDraw(vector3ub &color)
{
	glBegin(mode);

	glColor3ub(color);//glColor4f(1, 1, 1, 0.5f);//
	//glColor3f(0, 0, 1);
	//glColor3ub(1, 1, 0);
	for (auto obj : *vertexes) 
		glVertex3f(obj->getX(), obj->getY(), obj->getZ());


	glEnd();
}

void OpenGL_Base::simpleDrawVertexes(std::map<Vect3f*, vector3ub> &colorMap, GLfloat vertSize)
{
	for (auto it = vertexes->begin(); it != vertexes->end(); ++it)
		glDrawCube(vertSize, **it, colorMap[*it]);
}
