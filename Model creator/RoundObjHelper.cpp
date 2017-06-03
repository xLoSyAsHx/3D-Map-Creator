#include "RoundObjHelper.h"



std::vector<OpenGL_Base*>* genOGLBVectByPointVect(std::vector<Vect3f> &minPointsVect,
	GLfloat x1, GLfloat x2, GLfloat a, GLfloat b, GLfloat c, GLfloat r)
{
	//minPointsVect должен быть отсортирован в порядке, указанном ниже	
	//x, a, b, c и r задают эллипс для генерации в нём второго круга точек
	int size = minPointsVect.size();
	//x^2 + y^2 + z^2 = r^2
	//z = 0
	//x = -1
	//y^2 = r^2 - z^2 - x^2
	//y^2 = r^2 - x^2

	double y_width = 2 * sqrt((r*r - (x1*x1 / a*a)) * b*b);
	double z_width = 2 * sqrt((r*r - (x1*x1 / a*a)) * c*c);

	y_width = floor(y_width * 10000) / 10000.0;
	z_width = floor(z_width * 10000) / 10000.0;

	double step = y_width / size;

	std::vector<GLfloat> yValForVect2;
	std::vector<GLfloat> zValForVect2;

	float curY = -y_width / 2;
	for (int i = 0; i < size; ++i) {
		double val = (r*r - (x1*x1) / (a*a) - (curY*curY) / (b*b)) * c*c;
		val = floor(val * 1000) / 1000.0;
		val = sqrt(val);

		//if (curY < 0)
			val = -val;

		zValForVect2.push_back(val);
		yValForVect2.push_back(floor(curY * 1000) / 1000.0);
		curY += step;
	}

	for (int i = 0; i < size; ++i) {
		zValForVect2.push_back(-zValForVect2[i]);
		yValForVect2.push_back(-yValForVect2[i]);
	}

	std::vector<Vect3f> vect2;
	for (int i = 0; i < size * 2; ++i)
		vect2.push_back(Vect3f(x2, yValForVect2[i], zValForVect2[i]));
	/*
	нумерация точек
	        3		^
	    2 /--\ 4	|
	     |    |		|
	    1 \--/ 5	|
		    0		| y
	*/
	//y^2 / b^2 + z^2 / c^2 = r^2;

	auto calcCenter = [&a, &b, &c, &r](std::vector<Vect3f> &vect, int i) -> Vect3f {
		int i1 = i;
		int i2 = i == vect.size() - 1 ? 0 : i + 1;

		double midlZ = (vect[i1].getZ() + vect[i2].getZ()) / 2;

		double val = (r*r - ((vect[i].getX())*(vect[i].getX()) / (a*a)) - (midlZ*midlZ / (c*c))) * b*b;
		val = floor(val * 1000) / 1000.0;
		val = sqrt(val);

		return Vect3f(vect[i].getX(), val, floor(midlZ * 1000) / 1000.0);
	};

	std::vector<Vect3f> vect1; // Первый круг
	for (int i = 0; i < size ; ++i) {
		vect1.push_back(minPointsVect[i]);
		vect1.push_back(calcCenter(minPointsVect, i));
	}

	auto gen2VertsFromPoints = [](
		std::vector<Vect3f> &vect1,
		std::vector<Vect3f> &vect2,
		int index) -> std::pair<std::vector<Vect3f*>*, std::vector<Vect3f*>*> {
		int i1 = index;
		int i2 = index == vect1.size() - 1 ? 0 : index + 1;

		Vect3f l1, l2, r1, r2;
		l1 = vect1[i1];
		l2 = vect1[i2];
		r1 = vect2[i1];
		r2 = vect2[i2];

		std::pair<std::vector<Vect3f*>*, std::vector<Vect3f*>*> pair;

		std::vector<Vect3f*>* first = new std::vector<Vect3f*>;
		std::vector<Vect3f*>* second = new std::vector<Vect3f*>;

		first->push_back(new Vect3f(l1));
		first->push_back(new Vect3f(l2));
		first->push_back(new Vect3f(r1));


		second->push_back(new Vect3f(l2));
		second->push_back(new Vect3f(r2));
		second->push_back(new Vect3f(r1));

		pair.first = first;
		pair.second = second;

		return pair;
	};

	std::vector<Vect4f> color;

	color.push_back(Vect4f(0, 0, 50));
	color.push_back(Vect4f(0, 0, 100));
	color.push_back(Vect4f(0, 0, 150));
	color.push_back(Vect4f(0, 0, 200));
	color.push_back(Vect4f(0, 0, 250));
	color.push_back(Vect4f(0, 50, 0));
	color.push_back(Vect4f(0, 100, 0));
	color.push_back(Vect4f(0, 150, 0));


	std::vector<OpenGL_Base*>* OGLBVect = new std::vector<OpenGL_Base*>;
	for (int i = 0; i < size * 2; ++i) {
		auto obj = gen2VertsFromPoints(vect1, vect2, i);
		std::vector<std::vector<Vect3f*>*> temp = { obj.first, obj.second };

		for (int k = 0; k < 2; ++k) {
			OpenGL_Base* OGLB = new OpenGL_Base;

			std::vector<Vect4f*> *colors = new std::vector<Vect4f*>;
			for (int j = 0; j < 3; ++j)
				colors->push_back(new Vect4f(255,255,255));

			OGLB->setColors(colors);
			OGLB->setMode(4);
			OGLB->setVertexes(temp[k]);

			OGLBVect->push_back(OGLB);
		}
	}

	return OGLBVect;
}

DrawableObject2* genDisc(Vect3f pos, GLfloat a, GLfloat c, int n)
{
	if (abs(a) < 0.001)
		a = 1.0f;
	if (abs(c) < 0.001)
		c = 1.0f;

	//n = 124;
	//x^2 + z^2 = r^2
	if (n % 2 != 0)
		n++;

	std::vector<GLfloat> xValForVect;
	std::vector<GLfloat> zValForVect;

	//x^2 / a^2 + y^2 / b^2 + z^2 / c^2 = 1
	//y = 0
	//z^2 = (1 - x^2 / a^2) * c^2

	double curX = -a;
	double step = a / n * 4;
	for (int i = 0; i < n / 2; ++i) {
		double val = (1 - (curX*curX) / (a*a)) * c*c;
		//val = floor(val * 1000) / 1000.0;
		val = val < 0 ? 0 : val;
		val = sqrt(val);

		val = -val;

		zValForVect.push_back(val);
		xValForVect.push_back(floor(curX * 1000) / 1000.0);
		curX += step;
	}

	for (int i = 0; i < n / 2; ++i) {
		zValForVect.push_back(-zValForVect[i]);
		xValForVect.push_back(-xValForVect[i]);
	}

	std::vector<Vect3f> vect;
	for (int i = 0; i < n; ++i)
		vect.push_back(Vect3f(xValForVect[i], 0, zValForVect[i]));


	auto genVertsFrom3Points = [&pos](
		std::vector<Vect3f> &vect1,
		int index) -> std::vector<Vect3f*>* {
		int i1 = index;
		int i2 = index == vect1.size() - 1 ? 0 : index + 1;

		std::pair<std::vector<Vect3f*>*, std::vector<Vect3f*>*> pair;

		std::vector<Vect3f*>* verts = new std::vector<Vect3f*>;

		verts->push_back(new Vect3f(vect1[i1]));
		verts->push_back(new Vect3f(0, 0, 0));
		verts->push_back(new Vect3f(vect1[i2]));

		return verts;
	};

	std::vector<OpenGL_Base*>* OGLBVect = new std::vector<OpenGL_Base*>;
	for (int i = 0; i < n; ++i) {
		auto verts = genVertsFrom3Points(vect, i);

		OpenGL_Base* OGLB = new OpenGL_Base;

		std::vector<Vect4f*> *colors = new std::vector<Vect4f*>;
		for (int j = 0; j < 3; ++j)
			colors->push_back(new Vect4f(255, 255, 255));

		OGLB->setColors(colors);
		OGLB->setMode(4);
		OGLB->setVertexes(verts);

		OGLBVect->push_back(OGLB);
	}

	std::vector<Vect3f*>* objsPos = new std::vector<Vect3f*>;
	std::vector<Vect3f*>* objsRot = new std::vector<Vect3f*>;

	objsPos->resize(n);
	objsRot->resize(n);

	for (int i = 0; i < n; ++i) {
		(*objsPos)[i] = new Vect3f(0, 0, 0);
		(*objsRot)[i] = new Vect3f(0, 0, 0);
	}

	DrawableObject2 *drObj = new DrawableObject2(OGLBVect, objsPos, objsRot, pos, Vect3f(0, 0, 0));

	return drObj;
}

DrawableObject2 * genCone(Vect3f botPos, Vect3f topPos, GLfloat a, GLfloat c, int n)
{
	//auto bottom = genDisc(botPos, r, a, c, n1);
	auto top	= genDisc(botPos, a, c, n);

	auto obj = top->getVertexes();
	for (auto it = obj->begin(); it != obj->end(); ++it) {
		(*(*it)->getVertexes())[1]->setX(topPos.getX());
		(*(*it)->getVertexes())[1]->setY(topPos.getY());
		(*(*it)->getVertexes())[1]->setZ(topPos.getZ());
	}

	top->setPosition(botPos);
	return top;
}

DrawableObject2 * genCylinder(Vect3f botPos, Vect3f topPos, GLfloat a, GLfloat c, int n)
{
	if (n % 2 != 0)
		n++;

	std::vector<GLfloat> xValForVect;
	std::vector<GLfloat> zValForVect;

	double curX = -a;
	double step = a / n * 4;
	for (int i = 0; i < n / 2; ++i) {
		double val = (1 - (curX*curX) / (a*a)) * c*c;
		val = floor(val * 1000) / 1000.0;
		val = sqrt(val);

		val = -val;

		zValForVect.push_back(val);
		xValForVect.push_back(floor(curX * 1000) / 1000.0);
		curX += step;
	}

	for (int i = 0; i < n / 2; ++i) {
		zValForVect.push_back(-zValForVect[i]);
		xValForVect.push_back(-xValForVect[i]);
	}

	std::vector<Vect3f> vect1;
	for (int i = 0; i < n; ++i)
		vect1.push_back(Vect3f(xValForVect[i], 0, zValForVect[i]));

	Vect3f tempTopPos = topPos.minus(botPos);

	std::vector<Vect3f> vect2;
	for (int i = 0; i < n; ++i)
		vect2.push_back(Vect3f(xValForVect[i], 0, zValForVect[i]).sum(tempTopPos));


	auto gen2VertsFromPoints = [](
		std::vector<Vect3f> &vect1,
		std::vector<Vect3f> &vect2,
		int index) -> std::pair<std::vector<Vect3f*>*, std::vector<Vect3f*>*> {
		int i1 = index;
		int i2 = index == vect1.size() - 1 ? 0 : index + 1;

		Vect3f l1, l2, r1, r2;
		l1 = vect1[i1];
		l2 = vect1[i2];
		r1 = vect2[i1];
		r2 = vect2[i2];

		std::pair<std::vector<Vect3f*>*, std::vector<Vect3f*>*> pair;

		std::vector<Vect3f*>* first = new std::vector<Vect3f*>;
		std::vector<Vect3f*>* second = new std::vector<Vect3f*>;

		first->push_back(new Vect3f(l1));
		first->push_back(new Vect3f(l2));
		first->push_back(new Vect3f(r1));


		second->push_back(new Vect3f(l2));
		second->push_back(new Vect3f(r2));
		second->push_back(new Vect3f(r1));

		pair.first = first;
		pair.second = second;

		return pair;
	};

	std::vector<OpenGL_Base*>* OGLBVect = new std::vector<OpenGL_Base*>;
	for (int i = 0; i < n; ++i) {
		auto obj = gen2VertsFromPoints(vect1, vect2, i);
		std::vector<std::vector<Vect3f*>*> temp = { obj.first, obj.second };

		for (int k = 0; k < 2; ++k) {
			OpenGL_Base* OGLB = new OpenGL_Base;

			std::vector<Vect4f*> *colors = new std::vector<Vect4f*>;
			for (int j = 0; j < 3; ++j)
				colors->push_back(new Vect4f(255, 255, 255));

			OGLB->setColors(colors);
			OGLB->setMode(4);
			OGLB->setVertexes(temp[k]);

			OGLBVect->push_back(OGLB);
		}
	}

	std::vector<Vect3f*>* objsPos = new std::vector<Vect3f*>;
	std::vector<Vect3f*>* objsRot = new std::vector<Vect3f*>;

	objsPos->resize(2*n);
	objsRot->resize(2*n);

	for (int i = 0; i < 2 * n; ++i) {
		(*objsPos)[i] = new Vect3f(0, 0, 0);
		(*objsRot)[i] = new Vect3f(0, 0, 0);
	}

	DrawableObject2 *drObj = new DrawableObject2(OGLBVect, objsPos, objsRot, botPos, Vect3f(0, 0, 0));

	return drObj;
}

DrawableObject2 * genEllipsoid(Vect3f pos, GLfloat a, GLfloat b, GLfloat c, int n)
{
	if (n % 2 != 0)
		n++;

	
	float price = n / (a + b + c);
	/*
	int xNumOfPoints = a * price;
	int yNumOfPoints = b * price;
	int zNumOfPoints = c * price;
	*/
	int xNumOfPoints = 10;
	int yNumOfPoints = 10;
	int zNumOfPoints = 10;

	if (xNumOfPoints % 2 != 0)
		xNumOfPoints++;

	double curX = -a;
	double step = 2 * a / (xNumOfPoints - 1);
	std::vector<GLfloat> xValForVect;
	std::vector<GLfloat> yWidthForX;
	std::vector<GLfloat> zWidthForX;

	
	for (int i = 0; i < xNumOfPoints; ++i) {
		xValForVect.push_back(floor(curX * 1000) / 1000.0);
		//x^2 / a^2 + y^2 / b^2 + z^2 / c^2 = 1
		//y = 0
		//y^2 = (1 - x^2 / a^2) * b^2
		//z^2 = (1 - x^2 / a^2) * c^2
		double val = (1 - (curX*curX) / (a*a)) * c*c;
		val = floor(val * 1000) / 1000.0;
		val = val < 0 ? 0 : val;
		val = sqrt(val);

		zWidthForX.push_back(val);

		val = (1 - (curX*curX) / (a*a)) * b*b;
		val = floor(val * 1000) / 1000.0;
		val = val < 0 ? 0 : val;
		val = sqrt(val);

		yWidthForX.push_back(val);

		curX += step;
	}



	auto lCone = genCone(
		Vect3f(xValForVect[1], 0, 0),
		Vect3f(0, xValForVect[1] - xValForVect[0], 0), 
		yWidthForX[1], zWidthForX[1], 30);
	

	int vectSize = xValForVect.size();
	auto rCone = genCone(
		Vect3f(xValForVect[vectSize - 2], 0, 0),
		Vect3f(0, xValForVect[vectSize - 2] - xValForVect[vectSize - 1], 0),
		yWidthForX[vectSize - 2], zWidthForX[vectSize - 2], 30);
	
	std::vector<Vect3f> vect1;
	std::vector<OpenGL_Base*>* GlobalOGLBVect = new std::vector<OpenGL_Base*>;

	for (int i = 0; i < vectSize - 2; ++i) {

		//
		std::vector<GLfloat> zVals;
		std::vector<GLfloat> xVals;

		curX = -zWidthForX[i + 1];
		step = (2 * zWidthForX[i + 1]) / 10;
		for (int j = 0; j < 10; ++j) {
			double val = (1 - (curX*curX) / (yWidthForX[i + 1] * yWidthForX[i + 1])) * zWidthForX[i + 1] * zWidthForX[i + 1];
			val = floor(val * 1000) / 1000.0;
			val = val < 0 ? 0 : val;
			val = sqrt(val);

			val = -val;

			zVals.push_back(val);
			xVals.push_back(floor(curX * 1000) / 1000.0);
			curX += step;
		}

		for (int j = 0; j < 10; ++j) {
			zVals.push_back(-zVals[j]);
			xVals.push_back(-xVals[j]);
		}

		std::vector<Vect3f> vect2;
		for (int k = 0; k < xVals.size(); ++k)
			vect2.push_back(Vect3f(xValForVect[i + 1], xVals[k], zVals[k]));

		if (vect1.size() == 0) {
			vect1 = vect2;
			continue;
		}

	//
		auto gen2VertsFromPoints = [](
			std::vector<Vect3f> &vect1,
			std::vector<Vect3f> &vect2,
			int index) -> std::pair<std::vector<Vect3f*>*, std::vector<Vect3f*>*> {
			int i1 = index;
			int i2 = index == vect1.size() - 1 ? 0 : index + 1;

			Vect3f l1, l2, r1, r2;
			l1 = vect1[i1];
			l2 = vect1[i2];
			r1 = vect2[i1];
			r2 = vect2[i2];

			std::pair<std::vector<Vect3f*>*, std::vector<Vect3f*>*> pair;

			std::vector<Vect3f*>* first = new std::vector<Vect3f*>;
			std::vector<Vect3f*>* second = new std::vector<Vect3f*>;

			first->push_back(new Vect3f(l1));
			first->push_back(new Vect3f(l2));
			first->push_back(new Vect3f(r1));


			second->push_back(new Vect3f(l2));
			second->push_back(new Vect3f(r2));
			second->push_back(new Vect3f(r1));

			pair.first = first;
			pair.second = second;

			return pair;
		};


		std::vector<OpenGL_Base*>* OGLBVect = new std::vector<OpenGL_Base*>;
		for (int j = 0; j < vect1.size(); ++j) {
			auto obj = gen2VertsFromPoints(vect1, vect2, j);
			std::vector<std::vector<Vect3f*>*> temp = { obj.first, obj.second };

			for (int k = 0; k < 2; ++k) {
				OpenGL_Base* OGLB = new OpenGL_Base;

				std::vector<Vect4f*> *colors = new std::vector<Vect4f*>;
				for (int k = 0; k < 3; ++k)
					colors->push_back(new Vect4f(255, 255, 255));

				OGLB->setColors(colors);
				OGLB->setMode(4);
				OGLB->setVertexes(temp[k]);

				OGLBVect->push_back(OGLB);
			}
		}
		GlobalOGLBVect->insert(GlobalOGLBVect->end(), OGLBVect->begin(), OGLBVect->end());

		vect1 = vect2;
	}

	//GlobalOGLBVect->insert(GlobalOGLBVect->end(), lCone->getVertexes()->begin(),
		//lCone->getVertexes()->end());



	std::vector<Vect3f*>* objsPos = new std::vector<Vect3f*>;
	std::vector<Vect3f*>* objsRot = new std::vector<Vect3f*>;

	objsPos->resize(GlobalOGLBVect->size() + 
		lCone->getVertexes()->size() + rCone->getVertexes()->size());
	objsRot->resize(GlobalOGLBVect->size() +
		lCone->getVertexes()->size() + rCone->getVertexes()->size());

	for (int i = 0; i < GlobalOGLBVect->size(); ++i) {
		(*objsPos)[i] = new Vect3f(0, 0, 0);
		(*objsRot)[i] = new Vect3f(0, 0, 0);
	}

	int start = GlobalOGLBVect->size();
	int end = GlobalOGLBVect->size() + lCone->getVertexes()->size();
	for (int i = start; i != end; ++i) {
		(*objsPos)[i] = new Vect3f(*lCone->getPosition());
		(*objsRot)[i] = new Vect3f(0, 0, 90);
	}

	start = GlobalOGLBVect->size() + lCone->getVertexes()->size();
	end = GlobalOGLBVect->size() + lCone->getVertexes()->size() + rCone->getVertexes()->size();
	for (int i = start; i != end; ++i) {
		(*objsPos)[i] = new Vect3f(*rCone->getPosition());
		(*objsRot)[i] = new Vect3f(0, 0, 90);
	}

	auto verts = lCone->getVertexes();
	GlobalOGLBVect->insert(GlobalOGLBVect->end(), verts->begin(), verts->end());
	verts = rCone->getVertexes();
	GlobalOGLBVect->insert(GlobalOGLBVect->end(), verts->begin(), verts->end());

	DrawableObject2 *drObj = new DrawableObject2(GlobalOGLBVect, objsPos, objsRot, pos, Vect3f(0, 0, 0));

	return drObj;
}
