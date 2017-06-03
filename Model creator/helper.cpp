#include "helper.h"

/*
void print(float f) {
	char str[10];
	sprintf_s(str, "%f\n", f);
	dbgprintf(str);
}

void dbgprintf(char * format, ...)
{
	va_list args;
	int len;
	char * buffer;

	va_start(args, format);
	len = _vscprintf(format, args) // _vscprintf doesn't count
		+ 4; // terminating '\0'
	buffer = (char*)malloc(len * sizeof(char));
	unsigned u = vsprintf_s(buffer, len, format, args);
	buffer[u] = 13;
	buffer[u + 1] = 10;
	buffer[u + 2] = 0;
	OutputDebugString(buffer);
	free(buffer);
}
*/

int LoadGLTextures(std::string filename, GLuint *texture)                                    // Load Bitmaps And Convert To Textures
{		
	IplImage* image = 0;
	image = cvLoadImage(filename.c_str(), -1);

	if (image == nullptr)
		return 1;

	cvFlip(image);

	glGenTextures(1, &texture[0]);					// Create The Texture

													// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	if (image->nChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image->width, image->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image->imageData);
	else if (image->nChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image->width, image->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image->imageData);
	else
		return 1;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;                                        // Return Success
}

void glDrawSquare(GLfloat sideLen, Vect3f &pos, Vect3f &delPos, Vect3f &rot)
{
	glPushMatrix();
	glTranslatef(pos);
	glTranslatef(delPos);
	glRotatef(rot);

	glBegin(GL_QUADS);

	glVertex3f(-sideLen / 2, -sideLen / 2, 0);
	glVertex3f(-sideLen / 2, sideLen / 2, 0);
	glVertex3f(sideLen / 2, sideLen / 2, 0);
	glVertex3f(sideLen / 2, -sideLen / 2, 0);


	glEnd();

	glPopMatrix();
}

void glDrawCube(GLfloat sideLen, Vect3f & pos, vector3ub &color)
{
	glColor3ub(color);

	glDrawSquare(sideLen, pos, Vect3f(0, 0, sideLen / 2), Vect3f(0, 0, 0));
	glDrawSquare(sideLen, pos, Vect3f(0, 0, -sideLen / 2), Vect3f(0, 0, 0));
	glDrawSquare(sideLen, pos, Vect3f(-sideLen / 2, 0, 0), Vect3f(0, 90, 0));
	glDrawSquare(sideLen, pos, Vect3f(sideLen / 2, 0, 0), Vect3f(0, 90, 0));
	glDrawSquare(sideLen, pos, Vect3f(0, sideLen / 2, 0), Vect3f(90, 0, 0));
	glDrawSquare(sideLen, pos, Vect3f(0, -sideLen / 2, 0), Vect3f(90, 0, 0));
}

void BuildFont(HDC hDC, GLuint &base)
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(-24,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void glPrint(GLuint &base, const char * fmt, ...)
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf_s(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

void glTranslatef(const Vect3f& vect)
{
	//GLuint texture;
	//LoadGLTextures("asdasd", &texture);
	glTranslatef(vect.getX(), vect.getY(), vect.getZ());
}

void glRotatef(const Vect3f& vect)
{
	glRotatef(vect.getX(), 1.0f, 0.0f, 0.0f);
	glRotatef(vect.getY(), 0.0f, 1.0f, 0.0f);
	glRotatef(vect.getZ(), 0.0f, 0.0f, 1.0f);
}

void glTexCoord2f(const Vect2f& vect)
{
	glTexCoord2f(vect.getX(), vect.getY());
}

void glColor4f(const Vect4f & vect)
{
	glColor4f(vect.getX(), vect.getY(), vect.getZ(), vect.getAlpha());
}

void glColor4ub(const Vect4f & vect)
{
	glColor4ub(vect.getX(), vect.getY(), vect.getZ(), vect.getAlpha());
}

void glColor3f(const Vect3f & vect)
{
	glColor3f(vect.getX(), vect.getY(), vect.getZ());
}

void glColor3ub(const vector3ub vect)
{
	glColor3ub(vect.v[0], vect.v[1], vect.v[2]);
}

void glVertex3f(const Vect3f vect)
{
	glVertex3f(vect.getX(), vect.getY(), vect.getZ());
}

/*
void useMoveRotMatrix(std::vector<Vect3f*>* vertexes, Vect3f* shift, Vect3f* rot)
{
	GLfloat sum = 0;
	//std::vector<Vect3f*>::iterator it_shift, it_rot;

	//(*it_shift)



	for (auto it_vert = vertexes->begin(); it_vert != vertexes->end(); ++it_vert)
	{
		std::vector<std::vector<int>> mat1;// = { mat[3][3] = 1 };
		mat1.resize(4);
		for (int i = 0; i < mat1.size(); ++i)
			mat1[i].resize(4);


		mat1[0][0] = (*it_vert)->getX();
		mat1[1][1] = (*it_vert)->getY();
		mat1[2][2] = (*it_vert)->getZ();
		mat1[3][3] = 1;


		if (shift != nullptr) {
			std::vector<std::vector<int>> mat2;

			mat2.resize(4);
			for (int i = 0; i < mat2.size(); ++i)
				mat2[i].resize(4);

			mat2[0][0] = 1;
			mat2[3][1] = 1;
			mat2[2][2] = 1;
			mat2[3][3] = 1;

			mat2[3][0] = (*it_shift)->getX();
			mat2[3][1] = (*it_shift)->getY();
			mat2[3][2] = (*it_shift)->getZ();

			multiplieMatrix(mat1, mat2);
		}

		if (rot != nullptr) {
			std::vector<std::vector<int>> mat3;

			mat3 = getRotationMatrix(0, (*it_rot)->getX());
			multiplieMatrix(mat1, mat3);
			mat3 = getRotationMatrix(1, (*it_rot)->getY());
			multiplieMatrix(mat1, mat3);
			mat3 = getRotationMatrix(2, (*it_rot)->getZ());
			multiplieMatrix(mat1, mat3);


		}
	}

}

//
void useMoveMatrix(std::vector<Vect3f*>* vertexes, Vect3f* shift)
{
	for (auto it_vert = vertexes->begin(); it_vert != vertexes->end(); ++it_vert)
	{
		std::vector<std::vector<int>> mat1;// = { mat[3][3] = 1 };
		mat1.resize(4);
		for (int i = 0; i < mat1.size(); ++i)
			mat1[i].resize(4);


		mat1[0][0] = (*it_vert)->getX();
		mat1[1][1] = (*it_vert)->getY();
		mat1[2][2] = (*it_vert)->getZ();
		mat1[3][3] = 1;


		if (shift != nullptr) {
			std::vector<std::vector<int>> mat2;

			mat2.resize(4);
			for (int i = 0; i < mat2.size(); ++i)
				mat2[i].resize(4);

			mat2[0][0] = 1;
			mat2[3][1] = 1;
			mat2[2][2] = 1;
			mat2[3][3] = 1;

			mat2[3][0] = (*it_shift)->getX();
			mat2[3][1] = (*it_shift)->getY();
			mat2[3][2] = (*it_shift)->getZ();

			multiplieMatrix(mat1, mat2);
		}
	}
}
*/
//
std::vector<std::vector<int>> getRotationMatrix(int index, float alpha)
{
	std::vector<std::vector<int>> mat3;
	mat3.resize(4);
	for (int i = 0; i < mat3.size(); ++i)
		mat3[i].resize(4);
	mat3[0][0] = 1;
	mat3[3][1] = 1;
	mat3[2][2] = 1;
	mat3[3][3] = 1;

	switch (index)
	{
	case 0: 
	{
		mat3[1][1] = cos(alpha * aglToRad);
		mat3[1][2] = -sin(alpha * aglToRad);
		mat3[2][1] = cos(alpha * aglToRad);
		mat3[2][2] = sin(alpha * aglToRad);
	}
	break;
	case 1:
	{
		mat3[0][0] = cos(alpha * aglToRad);
		mat3[0][2] = -sin(alpha * aglToRad);
		mat3[2][0] = cos(alpha * aglToRad);
		mat3[2][2] = sin(alpha * aglToRad);

	}
	break;
	case 2:
	{
		mat3[0][0] = cos(alpha * aglToRad);
		mat3[0][1] = -sin(alpha * aglToRad);
		mat3[1][0] = cos(alpha * aglToRad);
		mat3[1][1] = sin(alpha * aglToRad);

	}
	break;
	default:
		break;
	}
	return mat3;
}

Vect3f rotateVect3f(int index, Vect3f& vect, float alpha)
{
	auto mat = getRotationMatrix(index, alpha);

	Vect3f resultVect;
	float x = 0, y = 0, z = 0;

	x += mat[0][0] * vect.getX() +
		mat[0][1] * vect.getY() +
		mat[0][2] * vect.getZ();

	y += mat[1][0] * vect.getX() +
		mat[1][1] * vect.getY() +
		mat[1][2] * vect.getZ();

	z += mat[2][0] * vect.getX() +
		mat[2][1] * vect.getY() +
		mat[2][2] * vect.getZ();

	return Vect3f(x, y, z);
}

float calcSquare(Vect3f a, Vect3f b, Vect3f c)
{
	float len_a = calcLen(a, b);
	float len_b = calcLen(a, c);
	float len_c = calcLen(b, c);

	float p = (len_a + len_b + len_c) / 2;

	return sqrt(p*(p - len_a)*(p - len_b)*(p - len_c));
}

float calcLen(Vect3f a, Vect3f b)
{
	auto asd = sqr(a.getX() - b.getX());
	auto asdf = sqr(a.getY() - b.getY());
	auto asdfg = sqr(a.getZ() - b.getZ());
	return sqrt(sqr(a.getX() - b.getX()) + sqr(a.getY() - b.getY()) + sqr(a.getZ() - b.getZ()));
}

float sqr(GLfloat a)
{
	return a * a;
}

bool isUniqueNumPressed(bool key[][256], int val)
{
	int num = 0;
	bool isValExist = false;

	for (int i = 48; i < 48 + 10; ++i) {
		if ((*key)[i] == TRUE)
			num++;
		if ((*key)[val] == TRUE)
			isValExist = true;
	}

	if (num == 1 && isValExist)
		return true;
	return false;

}

vector3ub getColorForSimpleDraw(std::map<int, vector3ub>& map)
{
	if (map.size() == 0)
		return { 0,0,1 };

	long new_map_size = map.size() + 1;
	std::vector<bool> temp;

	temp.resize(new_map_size, false);
	temp[0] = true;

	for (auto it = map.begin(); it != map.end(); ++it) {
		if (it->second < new_map_size)
			temp[(long)it->second] = true;
	}

	auto it = std::find(temp.begin(), temp.end(), false);


	if (it != temp.end())
		return to_vector3ub(long(it - temp.begin()));
	else
		return to_vector3ub(new_map_size);
}

vector3ub getColorForVertColorMap(std::map<int, std::map<Vect3f*, vector3ub>> &map)
{
	static long maxSize = 0;

	if (map.size() == 0) {
		maxSize = 1;
		return{ 0,0,1 };
	}

	std::vector<bool> temp;
	temp.resize(maxSize + 1, false);
	temp[0] = true;

	for (auto it_id = map.begin(); it_id != map.end(); ++it_id) {
		for (auto it = it_id->second.begin(); it != it_id->second.end(); ++it) {
			if (it->second < maxSize + 1)
				temp[(long)it->second] = true;
		}
	}


	auto it = std::find(temp.begin(), temp.end(), false);



	if (it != temp.end()) {
		return to_vector3ub(long(it - temp.begin()));
	}
	else {
		maxSize++;
		return to_vector3ub(maxSize + 1);
	}
}

vector3ub to_vector3ub(long val)
{
	vector3ub vect;

	vect.v[2] = val % 256;
	val /= 256;
	vect.v[1] = val % 256;
	val /= 256;
	vect.v[0] = val % 256;

	return vect;
}

std::vector<std::string> getKeys(std::map<std::string, std::map<int, int>>& map)
{
	std::vector<std::string> vect;

	for (auto obj : map)
		vect.push_back(obj.first);

	return vect;
}

int getNumOfPointsByMode(int mode)
{
	switch (mode)
	{
	case 4: return 3;
	}
}

std::pair<int, OPENFILENAME> getOFN(HWND hWnd, char * filter)
{
	OPENFILENAME ofn;
	const int count = 1024;
	char path[count] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = path;
	ofn.nMaxFile = count;
	ofn.lpstrFilter = (filter == NULL ? "All Files\0*.*\0" : filter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
		return{ 1, ofn };

	return{ -1, OPENFILENAME() };
}

Vect2f * genColor2f(Vect2f * color)
{
	if (color == nullptr)
		return nullptr;

	Vect2f *newColor = new Vect2f();

	newColor->setX(color->getX());
	newColor->setY(color->getY());

	return newColor;
}

std::vector<Vect2f*>* genColorVect2f(std::vector<Vect2f*>* vect)
{
	if (vect == nullptr)
		return nullptr;

	std::vector<Vect2f*> *newVect = new std::vector<Vect2f*>();

	for (auto it = vect->begin(); it != vect->end(); ++it)
		newVect->push_back(genColor2f(*it));

	return newVect;
}

Vect4f * genColor4f(Vect4f * color)
{
	if (color == nullptr)
		return nullptr;

	Vect4f *newColor = new Vect4f();

	for (char key = 'x'; key <= 'z'; ++key)
		newColor->set(key, color->get(key));

	newColor->setAlpha(color->getAlpha());

	return newColor;
}

std::vector<Vect4f*>* genColorVect4f(std::vector<Vect4f*>* vect)
{
	if (vect == nullptr)
		return nullptr;

	std::vector<Vect4f*> *newVect = new std::vector<Vect4f*>();

	for (auto it = vect->begin(); it != vect->end(); ++it)
		newVect->push_back(genColor4f(*it));

	return newVect;
}


bool operator<(const VertexKey l, const VertexKey r)
{
	if (l.id < r.id)
		return true;
	else if (l.id > r.id)
		return false;

	Vect3f* t_pos = l.pos;

	auto func = [t_pos, &r](char key)-> int {
		if (t_pos->get(key) < r.pos->get(key))
			return true;
		else if (t_pos->get(key) > r.pos->get(key))
			return false;

		return 2;
	};

	//--------
	bool res = func('x');
	if (res != 2)
		return res;

	res = func('y');
	if (res != 2)
		return res;

	res = func('z');
	if (res != 2)
		return res;
	//--------
	return false;

}


