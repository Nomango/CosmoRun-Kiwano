#pragma once
#include "Common.h"
#include "CubeFace.h"

KGE_DECLARE_SMART_PTR(Cubes);

class Cubes : public PolygonActor
{
public:
	static CubesPtr Create(ColorMode mode, float side_length, int depth);

	void Init(ColorMode mode, float side_length, int depth);

	void SetColor(ColorMode mode);

private:
	void BuildFaces(ColorMode mode, float side_length, int depth);

	void RangeFaces(Function<void(CubeFace*)> func);

private:
	CubeFace* center_ = nullptr;
};
