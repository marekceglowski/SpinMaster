#ifndef _H_2DMATHLIB_
#define _H_2DMATHLIB_

class point2D {
	public:
		point2D();
		point2D(float inX, float inY);
		float x;
		float y;
};

class vec2D{
	public:
		vec2D();
		vec2D(float inX, float inY);
		float x;
		float y;
		float magnitude();
};

class colour{
	public:
		colour();
		colour(float inR, float inG, float inB);
		float r;
		float g;
		float b;
};

class _2DMathLib {
	public:
		_2DMathLib();
		static float distance(point2D start, point2D end);
		static float fastDistance(point2D p1, point2D p2);
		static float length(vec2D v);
        static vec2D normalize(vec2D v);
        static vec2D vectorMultiply(vec2D vect, float scal);
        static vec2D createVector(point2D p1, point2D p2);
        static point2D movePoint(point2D p, vec2D v);

};


#endif
