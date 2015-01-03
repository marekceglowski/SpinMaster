#include "mathLib.h"
#include <math.h>

point2D::point2D()
{
	x = 0;
	y = 0;
}

point2D::point2D(float inX, float inY)
{
	x = inX;
	y = inY;
}

vec2D::vec2D()
{
	x = 0;
	y = 0;
}

vec2D::vec2D(float inX, float inY)
{
	x = inX;
	y = inY;
}

float vec2D::magnitude()
{
	return sqrt(pow(x, 2) + pow(y, 2));
}

colour::colour()
{
    r = 0;
    g = 0;
    b = 0;
}

colour::colour(float inR, float inG, float inB)
{
    r = inR;
    g = inG;
    b = inB;
}

_2DMathLib::_2DMathLib()
{
}

float _2DMathLib::distance(point2D start, point2D end)
{
	return sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
}

float _2DMathLib::fastDistance(point2D p1, point2D p2)
{
    return pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2);
}

float _2DMathLib::length(vec2D v)
{
    return v.magnitude();
}

vec2D _2DMathLib::normalize(vec2D v)
{
    vec2D vReturn = vec2D();
    vReturn = vec2D( (v.x)/(v.magnitude()), (v.y)/(v.magnitude()));
    return vReturn;
}

vec2D _2DMathLib::vectorMultiply(vec2D vect, float scal)
{
    return vec2D((scal * vect.x), (scal * vect.y));
}

vec2D _2DMathLib::createVector(point2D p1, point2D p2)
{
    return vec2D((p2.x - p1.x),(p2.y - p1.y));
}

point2D _2DMathLib::movePoint(point2D p, vec2D v)
{
    point2D pReturn = point2D();
    pReturn = point2D((p.x + v.x),(p.y + v.y));
    return pReturn;
}

