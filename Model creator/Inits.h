#ifndef INITS_H
#define INITS_H

#include "GL\freeglut.h"

#include "helper.h"

#include "structs.h"

#include <iostream>
#include <vector>
#include <map>

int InitGL(std::vector<LightGL> &lights_vect);

int InitMain(std::map<std::string, GLuint> &modelsMap);

int InitModelCreatorWindow(std::map<std::string, GLuint> &modelsMap);

GLvoid CreateModelBuildLists(std::map<std::string, GLuint> &modelsMap);

#endif INITS_H