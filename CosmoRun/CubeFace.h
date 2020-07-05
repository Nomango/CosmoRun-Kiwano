#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(CubeFace);

class CubeFace : public PolygonActor
{
public:
	enum class Type
	{
		Top,
		Left,
		Right,
	};

	enum class Direction
	{
		Up,
		Down,
		LeftUp,
		LeftDown,
		RightUp,
		RightDown
	};

	CubeFace(ColorEnum color, Type type, float side_length);

	void SetParent(CubeFace* parent);

	Type GetType() const;

	void SetColor(ColorEnum color);

	void RemoveSelf();

private:
	Type type_;
	float side_length_;
};
