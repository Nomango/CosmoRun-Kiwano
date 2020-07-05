#include "CubeFace.h"

CubeFace::CubeFace(ColorEnum color, Type type, float side_length)
	: type_(type)
	, side_length_(side_length)
{
	SetColor(color);
}

CubeFace::Type CubeFace::GetType() const
{
	return type_;
}

void CubeFace::SetColor(ColorEnum color)
{
}
