#pragma once

namespace DX
{

typedef std::shared_ptr<class Lens> Lens_ptr;

class Lens
{
public:
	void				  SetFOVY( float f ) { fFOVY = f; }
	float				  GetFOVY() const { return fFOVY; }

	void				  SetRatio( float f ) { fRatio = f; }
	float				  GetRatio() const { return fRatio; }

	void				  SetZNear( float f ) { fZNear = f; }
	float				  GetZNear() const { return fZNear; }

	void				  SetZFar( float f ) { fZFar = f; }
	float				  GetZFar() const { return fZFar; }

private:
	float				  fFOVY;
	float				  fRatio;
	float				  fZNear;
	float				  fZFar;
};

}