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

	FaceDesc GetDesc() const;

	bool IsIn(const std::initializer_list<FaceType>& list);

	void SetColor(ColorEnum color);

	void Show();

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
	FaceDesc desc_;
	float side_length_;
	CubeFace* next_ = nullptr;
	Cube* cube_ = nullptr;
	ResourceCache brush_cache_;
};
