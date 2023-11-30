#pragma once

template <class T>
class CPoint
{
public:
			  CPoint() : x(0), y(0), z(0) {}
			  CPoint( T _x, T _y, T _z ) : x(_x), y(_y), z(_z) {}

	T		  GetX() { return x; }
	T		  GetY() { return y; }
	T		  GetZ() { return z; }

	void	  SetX( T _x ) { x = _x; }
	void	  SetY( T _y ) { y = _y; }
	void	  SetZ( T _z ) { z = _z; }

public: //Allow direct access to private members for use convenience
	T		  x;
	T		  y;
	T		  z;

};