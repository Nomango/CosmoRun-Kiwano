#include "CubeFace.h"

CubeFacePtr CubeFace::Create(ColorMode mode, Type type, float side_length)
{
	CubeFacePtr ptr = memory::New<CubeFace>();
	if (ptr)
	{
		ptr->Init(mode, type, side_length);
	}
	return ptr;
}

void CubeFace::Init(ColorMode mode, Type type, float side_length)
{
}

CubeFace::Type CubeFace::GetType() const
{
	return type_;
}

void CubeFace::SetColor(ColorMode mode)
{
}

void CubeFace::SetBorder(int index, CubeFace* face)
{
	assert(index >= 0 && index < 4);
	borders_[index] = face;
}

CubeFace* CubeFace::GetBorder(int index) const
{
	assert(index >= 0 && index < 4);
	return borders_[index];
}
