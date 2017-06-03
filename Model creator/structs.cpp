#include "structs.h"

bool vector3ub::operator==(vector3ub color2)
{
	if (v[0] == color2.v[0] &&
		v[1] == color2.v[1] &&
		v[2] == color2.v[2])
		return true;
	return false;
}

bool vector3ub::operator<(vector3ub & color2)
{
	if (v[0] < color2.v[0])
		return true;
	else if (v[0] > color2.v[0])
		return false;
	else
	{
		if (v[1] < color2.v[1])
			return true;
		else if (v[1] > color2.v[1])
			return false;
		else
		{
			if (v[2] < color2.v[2])
				return true;
			else
				return false;
		}
	}
}

vector3ub::operator long()
{
	return v[0] * 256 * 256 + v[1] * 256 + v[2];
}

MapOldNewScal::MapOldNewScal(std::map<Vect3f*, Vect3f>* shifts, Vect3f oldScale, Vect3f newScale) :
	shifts(shifts), oldScale(oldScale), newScale(newScale)
{
}

ColorTexture::ColorTexture(std::vector<Vect4f*>* colors, std::string pathToTex,
	GLuint *texture, std::vector<Vect2f*>* texCoord) :
	colors(colors), pathToTex(pathToTex), texture(texture), texCoord(texCoord)
{
}
