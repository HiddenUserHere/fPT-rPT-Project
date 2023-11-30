#include "stdafx.h"
#include "DXColor.h"

namespace DX
{

std::unique_ptr<float[]> Color::ToFloatArray() const
{
	std::unique_ptr<float[]> arr(new float[4]);

	arr.get()[0] = (float)Red / 255.0f;
	arr.get()[1] = (float)Green / 255.0f;
	arr.get()[2] = (float)Blue / 255.0f;
	arr.get()[3] = (float)Alpha / 255.0f;

	return arr;
}

}