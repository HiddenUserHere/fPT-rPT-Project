#pragma once

namespace DX
{

class Color
{
public:
								  Color() { d3dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 ); }
								  Color( D3DCOLOR c ) { d3dColor = c; }
								  Color( int r, int g, int b ) { d3dColor = D3DCOLOR_XRGB( r, g, b ); }
								  Color( int a, int r, int g, int b ) { d3dColor = D3DCOLOR_ARGB( a, r, g, b ); }
								  Color( const Color & c ) { d3dColor = c.d3dColor; }

	D3DCOLOR					  Get() const { return d3dColor; }

	int							  GetAlpha() const { return Alpha; }
	int							  GetRed() const { return Red; }
	int							  GetGreen() const { return Green; }
	int							  GetBlue() const { return Blue; }

	void						  Set( D3DCOLOR c ) { d3dColor = c; }

	void						  SetAlpha( int a ) { a > 255 ? Alpha = 255 : Alpha = a; }
	void						  SetRed( int r ) { r > 255 ? Red = 255 : Red = r; }
	void						  SetGreen( int g ) { g > 255 ? Green = 255 : Green = g; }
	void						  SetBlue( int b ) { b > 255 ? Blue = 255 : Blue = b; }

	void						  AddAlpha( int a ) { SetAlpha( Alpha + a ); }
	void						  AddRed( int r ) { SetRed( Red + r ); }
	void						  AddGreen( int g ) { SetGreen( Green + g ); }
	void						  AddBlue( int b ) { SetBlue( Blue + b ); }

	std::unique_ptr<float[]>	  ToFloatArray() const;

private:
	union
	{
		D3DCOLOR				  d3dColor;

		struct 
		{
			BYTE				  Blue;
			BYTE				  Green;
			BYTE				  Red;
			BYTE				  Alpha;
		};
	};
};

}