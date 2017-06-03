#ifndef WINDOW_HELPER_H
#define WINDOW_HELPER_H

#include <GL\freeglut.h>

#include <iostream>
#include <vector>
#include <map>

#include "structs.h"

//typedef LRESULT(CALLBACK* MY_WNDPROC)(HWND, UINT, WPARAM, LPARAM, std::string, AddVars);

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

class WindowHelper {
public:
	static std::map<std::string, AddVars> map;


	static BOOL CreateGLWindow(HWND &hWnd, HINSTANCE &hInstance, WNDPROC WNDPROC, HWND *parent, HDC &hDC, HGLRC &hRC,
		int (InitGL)(std::vector<LightGL> &), std::vector<LightGL> &lights_vect,
		char* className, long w_left_pos, long w_top_pos, char* title, int width, int height, int bits);


	// Properly Kill The Window
	static GLvoid KillGLWindow(HWND &hWnd, HINSTANCE &hInstance, HDC &hDC, HGLRC &hRC, char* className);


	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);

	//static void WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, AddVars addVars);

};
#endif