#include "RoundDrawableObject.h"

SelectedCube RoundDrawableObject::detSelectedCube(CameraLocation const & cam)
{

	Vect3f left, right, up, down, toward, back;

	Vect3f cam_pos(cam.getX(), cam.getY(), cam.getZ());

	//Получаем матрицу перехода из B1 в global
	Matrix basisObj;
	basisObj.rotate(objRotation);
	//basisObj.getInvetMatrix(); // обратная матрица перехода

	int check = 0;
	auto it_OpenGL_Base = vertexes3f->begin();
	auto it_OpenGL_Base_pos = vertPosition->begin();
	auto it_OpenGL_Base_rot = vertRotation->begin();

	for (auto i = 0; i != vertexes3f->size(); ++i)
	{
		//Получаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в local
		std::vector<Vect3f*>* const p_vertexes = (*it_OpenGL_Base)->getVertexes();

		//Получаем матрицу перехода из B2 в B1																	     
		Matrix basisOpenGL_baseObj;
		basisOpenGL_baseObj.rotate(**it_OpenGL_Base_rot);

		//Получаем матрицу перехода из B2 в global
		Matrix fromB2ToGlob = basisObj * basisOpenGL_baseObj;

		//Получаем вектор, который надо добавить к координатам (получаемым ниже), чтобы получить координаты в global
		Vect3f additionToGlobal = Matrix::multMatOnVect(basisObj, **it_OpenGL_Base_pos);
		additionToGlobal = additionToGlobal.sum(*objPosition).sum(cam_pos);

		//Получаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в global
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

std::map<Vect3f*, Vect3f>* RoundDrawableObject::getAllGlobalCoord(CameraLocation const & cam)
{
	std::map<Vect3f*, Vect3f> *map = new std::map<Vect3f*, Vect3f>();

	Vect3f cam_pos(cam.getX(), cam.getY(), cam.getZ());

	//Получаем матрицу перехода из B1 в global
	Matrix basisObj;
	basisObj.rotate(objRotation);
	//basisObj.getInvetMatrix(); // обратная матрица перехода

	int check = 0;
	auto it_OpenGL_Base = vertexes3f->begin();
	auto it_OpenGL_Base_pos = vertPosition->begin();
	auto it_OpenGL_Base_rot = vertRotation->begin();

	for (auto i = 0; i != vertexes3f->size(); ++i)
	{
		//Получаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в local
		std::vector<Vect3f*>* const p_vertexes = (*it_OpenGL_Base)->getVertexes();

		//Получаем матрицу перехода из B2 в B1																	     
		Matrix basisOpenGL_baseObj;
		basisOpenGL_baseObj.rotate(**it_OpenGL_Base_rot);

		//Получаем матрицу перехода из B2 в global
		Matrix fromB2ToGlob = basisObj * basisOpenGL_baseObj;

		//Получаем вектор, который надо добавить к координатам (получаемым ниже), чтобы получить координаты в global
		Vect3f additionToGlobal = Matrix::multMatOnVect(basisObj, **it_OpenGL_Base_pos);
		additionToGlobal = additionToGlobal.sum(*objPosition).sum(cam_pos);

		//Получаем координаты точек объекта OpenGL_Base (из которых состоит сам DrawableObject) в global
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

	//(*map)[(&objPosition)] = objPosition;

	return map;
}

RoundDrawableObject::RoundDrawableObject(std::vector<OpenGL_Base*>* vertxMas, 
	std::vector<Vect3f*>* pos, std::vector<Vect3f*>* rot, 
	Vect3f objPos, Vect3f objRot) :
	Drawable(), vertexes3f(vertexes3f), vertPosition(pos), vertRotation(rot), objRotation(objRot)
{
	scale = Vect3f(1, 1, 1);
	objPosition = new Vect3f(objPos);
}
