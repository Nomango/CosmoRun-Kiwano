#pragma once
#include "Common.h"

class Cube;

KGE_DECLARE_SMART_PTR(CubeFace);

class CubeFace : public PolygonActor
{
public:
	CubeFace(FaceType type, Direction d, float side_length);

	FaceType GetType() const;

	Direction GetDirection() const;

	void SetColor(ColorEnum color);

	bool IsCollidedWith(const CubePos& pos, CubeDesc desc);

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
	FaceType type_;
	Direction direction_;
	float side_length_;
	CubeFace* next_ = nullptr;
	Cube* cube_ = nullptr;
};
