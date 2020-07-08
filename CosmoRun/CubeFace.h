#pragma once
#include "Common.h"

class Cube;

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

	CubeFace(Type type, Direction d, float side_length);

	Type GetType() const;

	Direction GetDirection() const;

	void SetColor(ColorEnum color);

	void RemoveSelf();

	CubeFace* GetNext() const;

	void SetNext(CubeFace* face);

	Cube* GetCube() const;

	void SetCube(Cube* cube);

	void ResetBrush(ColorEnum color);

private:
	void CreateVertices();

	BrushPtr GetFillBrush(ColorEnum color);

	BrushPtr GetTopFillBrush(Color light, Color dark);

	BrushPtr GetStrokeBrush(ColorEnum color);

	BrushPtr GetStrokeBrush(Color light, Color dark);

private:
	Type type_;
	Direction direction_;
	float side_length_;
	CubeFace* next_ = nullptr;
	Cube* cube_ = nullptr;
};
