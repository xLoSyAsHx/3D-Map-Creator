#ifndef HELPER_H
#define HELPER_H

#include <GL\freeglut.h>
#include <iostream>
#include <Windows.h>
#include <commdlg.h>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>     // Заголовочный файл для математической библиотеки ( НОВОЕ )
#include <stdarg.h>   // Заголовочный файл для функций для работы с переменным


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "Vect4f.h"
#include "structs.h"
#include "gloabalVars.h"
#include "Matrix.h"


int LoadGLTextures(std::string filename, GLuint *texture);

void glDrawSquare(GLfloat sideLen, Vect3f &pos, Vect3f &delPos, Vect3f &rot);

void glDrawCube(GLfloat sideLen, Vect3f & pos, vector3ub &color);

void BuildFont(HDC hDC, GLuint &base);

void glPrint(GLuint &base, const char *fmt, ...);

void glTranslatef(const Vect3f& vect);

void glRotatef(const Vect3f& vect);

void glTexCoord2f(const Vect2f& vect);

void glColor4f(const Vect4f& vect);

void glColor4ub(const Vect4f& vect);

void glColor3f(const Vect3f& vect);

void glColor3ub(const vector3ub vect);

void glVertex3f(const Vect3f vect);

//void glRotatef(const Vect3f& vect);
//void useMoveRotMatrix(std::vector<Vect3f*>* vertexes, std::vector<Vect3f*>* shift, std::vector<Vect3f*>* rot);

std::vector<std::vector<int>> getRotationMatrix(int index, float alpha); // 0 - X; 1 - Y; 2 - Z

//void multiplieMatrix(std::vector<std::vector<int>> &mat1, std::vector<std::vector<int>> &mat2);



Vect3f rotateVect3f(int index, Vect3f& vect, float alpha);

float calcSquare(Vect3f a, Vect3f b, Vect3f c);

float calcLen(Vect3f a, Vect3f b);

float sqr(GLfloat a);

bool isUniqueNumPressed(bool key[][256], int val);

vector3ub getColorForSimpleDraw(std::map<int, vector3ub> &map);

vector3ub getColorForVertColorMap(std::map<int, std::map<Vect3f*, vector3ub>> &map);

vector3ub to_vector3ub(long val);

std::vector<std::string> getKeys(std::map<std::string, std::map<int, int>> &obj);

int getNumOfPointsByMode(int mode);

std::pair<int, OPENFILENAME> getOFN(HWND hWnd, char* filter);

Vect2f * genColor2f(Vect2f * color);

std::vector<Vect2f*> * genColorVect2f(std::vector<Vect2f*> *vect);

Vect4f * genColor4f(Vect4f * color);

std::vector<Vect4f*> * genColorVect4f(std::vector<Vect4f*> *vect);

bool operator<(const VertexKey l, const VertexKey r);

#endif HELPER_H