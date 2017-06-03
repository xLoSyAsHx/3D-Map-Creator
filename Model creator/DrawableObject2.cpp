#include "DrawableObject2.h"



SelectedCube DrawableObject2::detSelectedCube(CameraLocation const & cam)
{

	Vect3f left, right, up, down, toward, back;

	Vect3f cam_pos(cam.getX(), cam.getY(), cam.getZ());

	//ѕолучаем матрицу перехода из B1 в global
	Matrix basisObj;
	basisObj.rotate(objRotation);
	//basisObj.getInvetMatrix(); // обратна€ матрица перехода

	int check = 0;
	auto it_OpenGL_Base = vertexes3f->begin();
	auto it_OpenGL_Base_pos = vertPosition->begin();
	auto it_OpenGL_Base_rot = vertRotation->begin();
	
	for (auto i = 0; i != vertexes3f->size(); ++i)
	{
		//ѕолучаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в local
		std::vector<Vect3f*>* const p_vertexes = (*it_OpenGL_Base)->getVertexes();

		//ѕолучаем матрицу перехода из B2 в B1																	     
		Matrix basisOpenGL_baseObj;
		basisOpenGL_baseObj.rotate(**it_OpenGL_Base_rot);

		//ѕолучаем матрицу перехода из B2 в global
		Matrix fromB2ToGlob = basisObj * basisOpenGL_baseObj;

		//ѕолучаем вектор, который надо добавить к координатам (получаемым ниже), чтобы получить координаты в global
		Vect3f additionToGlobal = Matrix::multMatOnVect(basisObj, **it_OpenGL_Base_pos);
		additionToGlobal = additionToGlobal.sum(*objPosition).sum(cam_pos);

		//ѕолучаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в global
		Vect3f a = Matrix::multMatOnVect(fromB2ToGlob, *(*p_vertexes)[0]).sum(additionToGlobal);
		Vect3f b = Matrix::multMatOnVect(fromB2ToGlob, *(*p_vertexes)[1]).sum(additionToGlobal);
		Vect3f c = Matrix::multMatOnVect(fromB2ToGlob, *(*p_vertexes)[2]).sum(additionToGlobal);

		if (check == 0) {
			check = 1;
			left = a;
			right = a;
			up = a;
			down = a;
			toward = a;
			back = a;
		}


		auto checkFunc = [&a, &b, &c](Vect3f &low, Vect3f &high, char key) {

			auto points = { a, b, c };

			for (auto it = points.begin(); it != points.end(); ++it) {

				if (it->get(key) < low.get(key))
					low = *it;
				if (it->get(key) > high.get(key))
					high = *it;
			}
		};


		checkFunc(left, right, 'x');
		checkFunc(down, up, 'y');
		checkFunc(back, toward, 'z');

		it_OpenGL_Base++;
		it_OpenGL_Base_pos++;
		it_OpenGL_Base_rot++;
	}

	auto func = [&cam](Vect3f vect) -> Vect3f {
		return vect.minus(Vect3f(cam.getX(), cam.getY(), cam.getZ()));
	};
	
	SelectedCube cube(func(left), func(right), func(down), func(up), func(back), func(toward));
	/*
	delete left;
	delete right;
	delete up;
	delete down;
	delete toward;
	delete back;
*/
	return cube;
}

std::map<Vect3f*, Vect3f>* DrawableObject2::getAllGlobalCoord(CameraLocation const & cam)
{
	std::map<Vect3f*, Vect3f> *map = new std::map<Vect3f*, Vect3f>();

	Vect3f cam_pos(cam.getX(), cam.getY(), cam.getZ());

	//ѕолучаем матрицу перехода из B1 в global
	Matrix basisObj;
	basisObj.rotate(objRotation);
	//basisObj.getInvetMatrix(); // обратна€ матрица перехода

	int check = 0;
	auto it_OpenGL_Base = vertexes3f->begin();
	auto it_OpenGL_Base_pos = vertPosition->begin();
	auto it_OpenGL_Base_rot = vertRotation->begin();

	for (auto i = 0; i != vertexes3f->size(); ++i)
	{
		//ѕолучаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в local
		std::vector<Vect3f*>* const p_vertexes = (*it_OpenGL_Base)->getVertexes();

		//ѕолучаем матрицу перехода из B2 в B1																	     
		Matrix basisOpenGL_baseObj;
		basisOpenGL_baseObj.rotate(**it_OpenGL_Base_rot);

		//ѕолучаем матрицу перехода из B2 в global
		Matrix fromB2ToGlob = basisObj * basisOpenGL_baseObj;

		//ѕолучаем вектор, который надо добавить к координатам (получаемым ниже), чтобы получить координаты в global
		Vect3f additionToGlobal = Matrix::multMatOnVect(basisObj, **it_OpenGL_Base_pos);
		additionToGlobal = additionToGlobal.sum(*objPosition).sum(cam_pos);

		//ѕолучаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в global
		Vect3f a = Matrix::multMatOnVect(fromB2ToGlob, *(*p_vertexes)[0]).sum(additionToGlobal);
		Vect3f b = Matrix::multMatOnVect(fromB2ToGlob, *(*p_vertexes)[1]).sum(additionToGlobal);
		Vect3f c = Matrix::multMatOnVect(fromB2ToGlob, *(*p_vertexes)[2]).sum(additionToGlobal);

		(*map)[(*p_vertexes)[0]] = a;
		(*map)[(*p_vertexes)[1]] = b;
		(*map)[(*p_vertexes)[2]] = c;

		it_OpenGL_Base++;
		it_OpenGL_Base_pos++;
		it_OpenGL_Base_rot++;
	}

	return map;
}

DrawableObject2::DrawableObject2(std::vector<OpenGL_Base*>* vertexes3f,
	std::vector<Vect3f*>* pos, std::vector<Vect3f*>* rot,
	Vect3f objPos, Vect3f objRot) :
	Drawable(), vertexes3f(vertexes3f), vertPosition(pos), vertRotation(rot), objRotation(objRot)
{
	scale = Vect3f(1, 1, 1);
	objPosition = new Vect3f(objPos);
	cube = new SelectedCube;
	coords = new std::map<Vect3f*, Vect3f>;
}

SelectedCube DrawableObject2::draw(CameraLocation const &cam, bool isVertexesEnable,
	std::map<Vect3f*, vector3ub> &colorMapForVertexes,
	std::pair<int, Vect3f*> &vertForLink,
	std::set<OpenGL_Base*> &OGLB_ForTex_set)
{
	if (isNeedUpdateCube) {
		*cube = detSelectedCube(cam);
		isNeedUpdateCube = false;
	}

	if (isNeedUpdateCoords) {
		if (coords != nullptr)
			delete coords;

		coords = getAllGlobalCoord(cam);
		for (auto it = coords->begin(); it != coords->end(); ++it)
			it->second.set(it->second.minus(Vect3f(cam.getX(), cam.getY(), cam.getZ())));

		isNeedUpdateCoords = false;
	}

	for (auto it_OGLB = vertexes3f->begin(); it_OGLB != vertexes3f->end(); ++it_OGLB)
	{
		std::vector<Vect2f*>::iterator p_it_texCoord;
		std::vector<Vect4f*>::iterator p_it_color;

		auto pVertexes = (*it_OGLB)->getVertexes();

		bool isTexCoord = false;
		bool isColor = false;

		if ((*it_OGLB)->getTextureCoord() != nullptr) {
			glColor3ub(255, 255, 255);
			p_it_texCoord = (*it_OGLB)->getTextureCoord()->begin();
			isTexCoord = true;
		}
		else if ((*it_OGLB)->getColors() != nullptr) {
			p_it_color = (*it_OGLB)->getColors()->begin();
			isColor = true;
		}

		if (isTexCoord)
			glBindTexture(GL_TEXTURE_2D, *(*it_OGLB)->getTexture());

		glBegin((*it_OGLB)->getMode());
		//glBegin(4);

		for (auto it = pVertexes->begin(); it != pVertexes->end(); ++it) {
			if (isTexCoord)
				glTexCoord2f(**p_it_texCoord);
			else if (isColor)
				glColor4ub(**p_it_color);//glColor4f(1, 1, 1, 0.5f);//


			glVertex3f((*coords)[*it]);

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

		if (OGLB_ForTex_set.find(*it_OGLB) != OGLB_ForTex_set.end())
		{
			std::vector<vector3ub> colorsForMapTex = { { 255, 0, 0 },{ 0, 255, 0 },{ 0, 0, 255 } };
			int i = 0;
			for (auto it = pVertexes->begin(); it != pVertexes->end(); ++it) {
				color = colorsForMapTex[i++];
				glDrawCube(vertSize, (*coords)[*it], color);
			}
		}
		else if (isVertexesEnable)
			for (auto obj : *pVertexes)
			{
				if (vertForLink.first != -1 && vertForLink.second == obj)
				{
					color = { 200, 0, 200 };
				}
				else
				{
					if (colorMapForVertexes.find(obj) != colorMapForVertexes.end())
						color = { 0, 200, 200 };
					else
						color = { 255, 0, 0 };
				}
				glDrawCube(vertSize, (*coords)[obj], color);
			}
	}

	

	return *cube;
}

void DrawableObject2::updateCoords()
{
	isNeedUpdateCoords = true;
}


	void DrawableObject2::updateCube()
	{
		isNeedUpdateCube = true;
	}

	void DrawableObject2::simpleDraw(vector3ub &colorVect)
{

	auto it_vertPos = vertPosition->begin();
	auto it_vertRot = vertRotation->begin();

	glPushMatrix();

	glTranslatef(*objPosition);
	glRotatef(objRotation);

	for (auto vert = vertexes3f->begin(); vert != vertexes3f->end(); ++vert)
	{

		//glBegin((*vert)->getMode());
		glPushMatrix();

		glTranslatef(**it_vertPos);
		glRotatef(**it_vertRot);

		(*vert)->simpleDraw(colorVect);

		glPopMatrix();
		//glEnd();

		it_vertPos++;
		it_vertRot++;
	}

	glPopMatrix();
}

void DrawableObject2::simpleDrawVertexes(std::map<Vect3f*, vector3ub> &colorMap)
{
	auto it_vertPos = vertPosition->begin();
	auto it_vertRot = vertRotation->begin();

	glPushMatrix();

	glTranslatef(*objPosition);
	glRotatef(objRotation);

	for (auto vert = vertexes3f->begin(); vert != vertexes3f->end(); ++vert)
	{

		//glBegin((*vert)->getMode());
		glPushMatrix();

		glTranslatef(**it_vertPos);
		glRotatef(**it_vertRot);

		(*vert)->simpleDrawVertexes(colorMap, vertSize);

		glPopMatrix();
		//glEnd();

		it_vertPos++;
		it_vertRot++;
	}

	glPopMatrix();
}

void DrawableObject2::setPosition(Vect3f pos)
{
	if (objPosition == nullptr)
		objPosition = new Vect3f();

	objPosition->set(pos);
}

void DrawableObject2::setVertSize(GLfloat newVertSize)
{
	vertSize = newVertSize;
}

void DrawableObject2::move(GLfloat x, GLfloat y, GLfloat z)
{
	objPosition->move(x, y, z);

}

void DrawableObject2::move(Vect3f vect)
{
	objPosition->move(vect);
}

void DrawableObject2::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	objRotation.move(x, y, z);
}

void DrawableObject2::rotate(Vect3f vect)
{
	objRotation.move(vect);
}

bool DrawableObject2::isSelected(CameraLocation cam, Vect2f camPos, HWND hWnd) //544, 112
{
	POINT ppp;
	GetCursorPos(&ppp);

	int mouse_x = ppp.x, mouse_y = ppp.y;

	GLint    viewport[4];    // параметры viewport-a.
	GLdouble projection[16]; // матрица проекции.
	GLdouble modelview[16];  // видова€ матрица.
	GLdouble vx, vy, vz;       // координаты курсора мыши в системе координат viewport-a.
	GLdouble wx, wy, wz;       // возвращаемые мировые координаты.

	glGetIntegerv(GL_VIEWPORT, viewport);           // узнаЄм параметры viewport-a.
	glGetDoublev(GL_PROJECTION_MATRIX, projection); // узнаЄм матрицу проекции.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // узнаЄм видовую матрицу.
													// переводим оконные координаты курсора в систему координат viewport-a.
	vx = mouse_x;
	vy = 480 - mouse_y - 1; // где height - текуща€ высота окна.

						  // вычисл€ем ближний конец селектирующего отрезка.
	vz = -1;
	gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);


	return false;
}

void DrawableObject2::setRotation(Vect3f rot)
{
	objRotation.set(rot);
}

Vect3f* DrawableObject2::getPosition()
{
	return objPosition;
}

Vect3f DrawableObject2::getRotation()
{
	return objRotation;
}

Vect3f DrawableObject2::getScal()
{
	return scale;
}

void DrawableObject2::setScal(Vect3f newScal)
{
	scale = newScal;
}

std::vector<Vect3f*>* DrawableObject2::getVertPosition()
{
	return vertPosition;
}

std::vector<Vect3f*>* DrawableObject2::getVertRotation()
{
	return vertRotation;
}

std::vector<OpenGL_Base*>* DrawableObject2::getVertexes()
{
	return vertexes3f;
}

DrawableObject2::~DrawableObject2()
{
	delete vertexes3f;
	delete vertPosition;
	delete vertRotation;
	delete cube;
	delete coords;
}

