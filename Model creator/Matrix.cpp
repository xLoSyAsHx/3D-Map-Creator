#include "Matrix.h"
#include "Matrix4.h"



void Matrix::setVal(int i, int j, GLdouble val)
{
	mat[i][j] = val;
}

GLdouble Matrix::getMinor(int i_koef, int j_koef)
{
	GLdouble temp[2][2];

	int i1 = 0, j1 = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i != i_koef && j != j_koef) {
				temp[i1][j1] = mat[i][j];
				i1++;
			}
		}
		i1 = 0;
		if (i_koef != i)
			j1++;
	}
	return (((i_koef + j_koef) % 2 == 0) ? 1 : -1) *
		(temp[0][0] * temp[1][1] - temp[0][1] * temp[1][0]);
}

GLdouble Matrix::getVal(int i, int j) const
{
	return mat[i][j];
}

Matrix Matrix::getInvetMatrix()
{
	GLdouble invertMat[3][3];

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			invertMat[i][j] = getMinor(i, j);

	std::swap(invertMat[0][1], invertMat[1][0]);
	std::swap(invertMat[0][2], invertMat[2][0]);
	std::swap(invertMat[1][2], invertMat[2][1]);

	if (this->det == nullptr)
		this->calculateDet();
	GLdouble temp_det = this->getDet();

	return Matrix(
		Vect3f(invertMat[0][0], invertMat[0][1], invertMat[0][2]),
		Vect3f(invertMat[1][0], invertMat[1][1], invertMat[1][2]),
		Vect3f(invertMat[2][0], invertMat[2][1], invertMat[2][2])
	) * (1.0 / temp_det);
}

Vect3f Matrix::multMatOnVect(Matrix mat, Vect3f vect)
{
	GLdouble coord[3];

	for (int i = 0; i < 3; ++i) {
		Vect3f line = mat.getLine(i);
		coord[i] = line.getX() * vect.getX() +
			line.getY() * vect.getY() +
			line.getZ() * vect.getZ();
	}

	return Vect3f(coord[0], coord[1], coord[2]);
}

Vect3f Matrix::rotationMatrix_X(Vect3f vect, GLdouble angle)
{
	angle = -angle;
	Matrix mat(
		Vect3f(1, 0, 0),
		Vect3f(0, cos(angle * aglToRad), -sin(angle * aglToRad)),
		Vect3f(0, sin(angle * aglToRad), cos(angle * aglToRad))
	);

	return multMatOnVect(mat, vect);
}

Vect3f Matrix::rotationMatrix_Y(Vect3f vect, GLdouble angle)
{
	angle = -angle;
	Matrix mat(
		Vect3f(cos(angle * aglToRad), 0, sin(angle * aglToRad)),
		Vect3f(0, 1, 0),
		Vect3f(-sin(angle * aglToRad), 0, cos(angle * aglToRad))
	);

	return multMatOnVect(mat, vect);
}

Vect3f Matrix::rotationMatrix_Z(Vect3f vect, GLdouble angle)
{
	angle = -angle;
	Matrix mat(
		Vect3f(cos(angle * aglToRad), -sin(angle * aglToRad), 0),
		Vect3f(sin(angle * aglToRad), cos(angle * aglToRad), 0),
		Vect3f(0, 0, 1)
	);

	return multMatOnVect(mat, vect);
}

Matrix::Matrix()
{
	mat[0][0] = 1;
	mat[1][1] = 1;
	mat[2][2] = 1;
	det = nullptr;
}

Matrix::Matrix(Vect3f a, Vect3f b, Vect3f c)
{
	/*
	|	a.x		a.y		a.z		|
	|	b.x		b.y		b.z		|
	|	c.x		c.y		c.z		|
	
	mat[i][j]
	i - строка
	j - столбец
	*/
	Vect3f mas[3] = { a, b, c };

	//std::ofstream fout("matrix.txt", std::ios_base::app);

	for (int i = 0; i < 3; ++i) {
		



		mat[i][0] = mas[i].getX();
		mat[i][1] = mas[i].getY();
		mat[i][2] = mas[i].getZ();


		//fout << mat[i][0] << " " << mat[i][1] << " " << mat[i][2] << "\n";
	}
	//fout << "\n\n---------------------------------\n";
	//fout.close();

	det = nullptr;
}

Matrix::Matrix(Plane3D a, Plane3D b, Plane3D c)
{
	Plane3D mas[3] = { a, b, c };
	//std::ofstream fout("matrix.txt", std::ios_base::app);
	//fout << "matrix: \n\n";
	for (int i = 0; i < 3; ++i) {
		


		mat[i][0] = mas[i].getA();
		mat[i][1] = mas[i].getB();
		mat[i][2] = mas[i].getC();


		//fout << mat[i][0] << " " << mat[i][1] << " " << mat[i][2] << "\n";
	}
	//fout << "\n\n---------------------------------\n";
	//fout.close();

	det = nullptr;
}

void Matrix::calculateDet()
{
	if (det == nullptr)
		det = new GLdouble;
	//std::ofstream fout("matrix.txt", std::ios_base::app);
	//std::ofstream fout("asdasd.txt", std::ios_base::app);
	//std::ofstream fout2("matrix.txt", std::ios_base::app);
	//fout << "det:\n\n";
	//fout2 << "matrix in calcDet: \n\n";
	for (int i = 0; i < 3; ++i)
		//fout << mat[i][0] << "\t\t" << mat[i][1] << "\t\t" << mat[i][2] << "\n";


	*det = mat[0][0] * mat[1][1] * mat[2][2] + // +
		mat[2][0] * mat[0][1] * mat[1][2] + //+
		mat[1][0] * mat[2][1] * mat[0][2] - //+
		mat[2][0] * mat[1][1] * mat[0][2] - //+
		mat[0][0] * mat[2][1] * mat[1][2] - // была ошибка
		mat[1][0] * mat[0][1] * mat[2][2]; //+
}

void Matrix::rotate(float a, float b, float g)
{

}


void Matrix::rotate(Vect3f vect)
{
	std::vector<Vect3f> vectPool;
	for (int i = 0; i < 3; ++i)
		vectPool.push_back(Vect3f(mat[i][0], mat[i][1], mat[i][2]));


	auto funcPool = { rotationMatrix_X, rotationMatrix_Y, rotationMatrix_Z };
	auto it = funcPool.begin();

	//auto asd = { Vect3f::getX() , Vect3f::getX() ,Vect3f::getX()}

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			vectPool[i] = (*(it + j))(vectPool[i], vect[j]);

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			mat[i][j] = vectPool[i][j];
		//mat[i][0] = vectPool[i].getX();
		//mat[i][1] = vectPool[i].getY();
		//mat[i][2] = vectPool[i].getZ();
}

void Matrix::setColumn(Vect3f vect, int i)
{
	mat[0][i] = vect.getX();
	mat[1][i] = vect.getY();
	mat[2][i] = vect.getZ();
}

GLdouble Matrix::getDet() const
{
	return *det;
}

Vect3f Matrix::getLine(int i)
{
	return Vect3f(mat[i][0], mat[i][1], mat[i][2]);
}

void Matrix::setLine(Vect3f vect, int i)
{
	mat[i][0] = vect.getX();
	mat[i][1] = vect.getY();
	mat[i][2] = vect.getZ();
}

Vect3f Matrix::getColumn(int i) const
{
	return Vect3f(mat[0][i], mat[1][i], mat[2][i]);
}



Matrix Matrix::operator*(const Matrix & mat2)
{
	Matrix newMat;

	auto func = [this](int i, int j, Matrix mat2) -> GLfloat {
		GLdouble res = 0;
		for (int k = 0; k < 3; ++k)
			res += mat[i][k] * mat2.getVal(k, j);
		return res;
	};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j)
			newMat.setVal(i, j, func(i, j, mat2));
	}

	return newMat;
}

Matrix Matrix::operator*(const GLdouble koef)
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		this->mat[i][j] = koef * this->mat[i][j];
	return *this;
}

Matrix::~Matrix()
{
}
