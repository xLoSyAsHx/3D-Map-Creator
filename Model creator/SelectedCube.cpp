#include "SelectedCube.h"

SelectedCube::SelectedCube(Vect3f & left, Vect3f & right, Vect3f & down, Vect3f & up, Vect3f & back, Vect3f & toward)
/*
Куб со стороной 2 в (0,0,0)

Низ:
(-1,-1,-1)     (1,-1,-1)
____________________
|                  |
|                  |
|                  |
|__________________|
(-1,-1,1)     (1,-1,1)


Верх:
(-1,1,-1)     (1,1,-1)
____________________
|                  |
|                  |
|                  |
|__________________|
(-1,1,1)     (1,1,1)
*/
{
	Vect3f _x[3] = { left, {}, right };
	Vect3f _y[3] = { down, {}, up };
	Vect3f _z[3] = { back, {}, toward };



	for (int i = -1; i < 2; i += 2) {
		for (int j = -1; j < 2; j += 2) {
			for (int k = -1; k < 2; k += 2) {
				auto key = std::to_string(i) + std::to_string(j) + std::to_string(k);
				GLfloat a, b, c;

				a = _x[i + 1].getX();
				b = _y[j + 1].getY();
				c = _z[k + 1].getZ();

				cube[key] = Vect3f(a, b, c);
			}
		}
	}

	//cube["-1-1-1"] = Vect3f(left.getX(), down.getY(), back.getZ());
}

SelectedCube::SelectedCube()
{
	for (int i = -1; i < 2; i += 2) {
		for (int j = -1; j < 2; j += 2) {
			for (int k = -1; k < 2; k += 2) {
				auto key = std::to_string(i) + std::to_string(j) + std::to_string(k);

				cube[key] = Vect3f(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

GLfloat SelectedCube::getLeft()
{
	return cube["-1-1-1"].getX();
}

GLfloat SelectedCube::getRight() 
{
	return cube["111"].getX();
}

GLfloat SelectedCube::getDown() 
{
	return cube["-1-1-1"].getY();
}

GLfloat SelectedCube::getUp() 
{
	return cube["111"].getY();
}

GLfloat SelectedCube::getFront() 
{
	return cube["111"].getZ();
}

GLfloat SelectedCube::getBack() 
{
	return cube["-1-1-1"].getZ();
}

void SelectedCube::scal(Vect3f vect)
{
	scal(vect.getX(), vect.getY(), vect.getZ());
}

void SelectedCube::scal(GLfloat x, GLfloat y, GLfloat z)
{
	Vect3f diagonal = cube["111"].minus(cube["-1-1-1"]);

	auto getParam = [](std::string key, int *param) {

		int index = 0;
		for (int i = 0; i < 3; ++i)
			if (key[index] != '1') {
				param[i] = -1;
				index += 2;
			}
			else {
				param[i] = 1;
				index++;
			}
	};

	for (int i = -1; i < 2; i += 2) {
		for (int j = -1; j < 2; j += 2) {
			for (int k = -1; k < 2; k += 2) {
				auto key = std::to_string(i) + std::to_string(j) + std::to_string(k);

				int param[3];
				getParam(key, param);

				for (int index = 0; index != 3; ++index)
					if (param[index] == 1)
						cube[key].set('x' + index, cube[key].get('x' + index) + diagonal.get('x' + index));
			}
		}
	}
}

/*
Куб со стороной 2 в (0,0,0)

Низ:
(-1,-1,-1)     (1,-1,-1)
____________________
|                  |
|                  |
|                  |
|__________________|
(-1,-1,1)     (1,-1,1)


Верх:
(-1,1,-1)     (1,1,-1)
____________________
|                  |
|                  |
|                  |
|__________________|
(-1,1,1)     (1,1,1)
*/
void SelectedCube::draw()
{
	glColor3ub(0, 200, 200);
	glBegin(GL_LINES);

	//низ
	glVertex3f(cube["-1-11"]);
	glVertex3f(cube["-1-1-1"]);

	glVertex3f(cube["-1-1-1"]);
	glVertex3f(cube["1-1-1"]);

	glVertex3f(cube["1-1-1"]);
	glVertex3f(cube["1-11"]);

	glVertex3f(cube["1-11"]);
	glVertex3f(cube["-1-11"]);


	//верх
	glVertex3f(cube["-111"]);
	glVertex3f(cube["-11-1"]);

	glVertex3f(cube["-11-1"]);
	glVertex3f(cube["11-1"]);

	glVertex3f(cube["11-1"]);
	glVertex3f(cube["111"]);

	glVertex3f(cube["111"]);
	glVertex3f(cube["-111"]);

	//бок
	glVertex3f(cube["-1-11"]);
	glVertex3f(cube["-111"]);

	glVertex3f(cube["-1-1-1"]);
	glVertex3f(cube["-11-1"]);

	glVertex3f(cube["1-1-1"]);
	glVertex3f(cube["11-1"]);

	glVertex3f(cube["1-11"]);
	glVertex3f(cube["111"]);


	glEnd();
}

SelectedCube SelectedCube::makeCubeFrom2(SelectedCube & a, SelectedCube & b)
{
	SelectedCube newCube;

	Vect3f av, bv;

	auto getParam = [](std::string key, int *param) {

		int index = 0;
		for (int i = 0; i < 3; ++i)
			if (key[index] != '1') {
				param[i] = -1;
				index += 2;
			}
			else {
				param[i] = 1;
				index++;
			}
	};


	for (int i = -1; i < 2; i += 2) {
		for (int j = -1; j < 2; j += 2) {
			for (int k = -1; k < 2; k += 2) {
				auto key = std::to_string(i) + std::to_string(j) + std::to_string(k);

				int param[3];
				getParam(key, param);
				for (int index = 0; index < 3; ++index)
					if (param[index] == -1)
						newCube.cube[key].set('x' + index, std::min(av.get('x' + index), bv.get('x' + index)));
					else
						newCube.cube[key].set('x' + index, std::max(av.get('x' + index), bv.get('x' + index)));
			}
		}
	}

	return newCube;
}
