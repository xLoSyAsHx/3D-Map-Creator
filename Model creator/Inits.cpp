#include "Inits.h"


int InitGL(std::vector<LightGL> &lights_vect)										// All Setup For OpenGL Goes Here
{
	//if (LoadGLTextures("Data/glass.bmp", texture))								// Jump To Texture Loading Routine ( NEW )
		//return 1;										// If Texture Didn't Load Return FALSE
	//BuildLists();


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(.0f, .0f, .0f, .5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glLightfv(GL_LIGHT1, GL_AMBIENT, lights_vect[0].LightAmbient);    // Установка Фонового Света
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lights_vect[0].LightDiffuse);    // Установка Диффузного Света
	glLightfv(GL_LIGHT1, GL_POSITION, lights_vect[0].LightPosition);   // Позиция света
	glEnable(GL_LIGHT1); // Разрешение источника света номер один

						 //glColor4f(1.0f, 1.0f, 1.0f, 1.f);   // Полная яркость, 50% альфа (НОВОЕ)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Функция смешивания для непрозрачности,
									   // базирующаяся на значении альфы(НОВОЕ)

	return 0;											// Initialization Went OK
}

int InitMain(std::map<std::string, GLuint> &modelsMap)
{
	CreateModelBuildLists(modelsMap);



	return 0;
}

int InitModelCreatorWindow(std::map<std::string, GLuint> &modelsMap)//640 550
{
	//if (LoadGLTextures("Data/glass.bmp", texture))								// Jump To Texture Loading Routine ( NEW )
		//return 1;										// If Texture Didn't Load Return FALSE
	CreateModelBuildLists(modelsMap);

	return 0;
}

GLvoid CreateModelBuildLists(std::map<std::string, GLuint> &modelsMap)
{

	modelsMap["grid"] = glGenLists(1);
	glNewList(modelsMap["grid"], GL_COMPILE);

	float x_board = 5, y_board = 5, z_board = 5, step = 5.f;
	glBegin(GL_LINES);
	// красный цвет

	for (int j = -y_board; j < y_board; ++j) { // y
		for (int i = -x_board; i < x_board; ++i) { // x
			glVertex3f(step * i, step * j, -x_board * step);
			glVertex3f(step * i, step * j, x_board * step);
		}
	}

	for (int j = -y_board; j < y_board; ++j) { // y
		for (int k = -z_board; k < z_board; ++k) { // z
			glVertex3f(-x_board * step, step * j, step * k);
			glVertex3f(x_board * step, step * j, step * k);
		}
	}

	for (int k = -z_board; k < z_board; ++k) { // y
		for (int i = -x_board; i < x_board; ++i) { // x
			glVertex3f(step * i, -y_board * step, step * k);
			glVertex3f(step * i, y_board * step, step * k);
		}
	}
	glEnd();
	glEndList();
}