/*
*		This Code Was Created By Jeff Molofee 2000
*		A HUGE Thanks To Fredric Echols For Cleaning Up
*		And Optimizing The Base Code, Making It More Flexible!
*		If You've Found This Code Useful, Please Let Me Know.
*		Visit My Site At nehe.gamedev.net
*/

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library
#include <GL\freeglut.h>
//#include <glaux.h>

#include "WindowHelper.h"
#include "helper.h"
#include "helper2.h"

#include "Inits.h"

#include "resource.h"
#include "DrawableObject2.h"
#include "DrawableObjectPresenter2.h"
#include "MoveBtnsController.h"
#include "gloabalVars.h"
#include "structs.h"
//#include "DrawableObjectsPresenter2.h"


#include "Win3DMapRedactor.h"

MoveBtnsController moveBtnsControl;

HWND hedit;
HWND dlg;
DrawableObjectsPresenter2* DOPresenter = new DrawableObjectsPresenter2();

std::map<std::string, std::vector<int>> currentObjs = { {"common", {} }, {"trans", {} } };

//std::pair<int, std::string> currentObj = {-1, "no"};




TCHAR textBuffer[10] = { 0 };


BOOL light;      // ���� ��� / ����
BOOL lp;         // L ������?
BOOL fp;         // F ������?
bool  bp;    // B ������? ( ����� )
bool blend;

GLfloat xrot;         // X ��������
GLfloat yrot;         // Y ��������
GLfloat zrot;
GLfloat xspeed;       // X �������� ��������
GLfloat yspeed;       // Y �������� ��������


GLfloat oz = -5.0f, oy = 0.0f, ox = 0.0f;      


//GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // �������� �������� ����� ( ����� )
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.f };

GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // �������� ���������� ����� ( ����� )

GLfloat LightPosition[] = { 0.0f, .0f, 0.0f, .0f };     // ������� ����� ( ����� )

std::vector<LightGL> lights_vect = { LightGL(LightAmbient, LightDiffuse, LightPosition) };

GLuint modelList[1], gridList;

GLuint filter;         // ����� ������ ������������
GLuint texture[3];     // ����� ��� �������� 3 �������





#include <strsafe.h>


int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	Win3DMapRedactor win3DMapRedactor;
	//win3DMapRedactor.start();

	//return (msg.wParam);							// Exit The Program
	return 0;
}
