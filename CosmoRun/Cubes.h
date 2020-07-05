#pragma once
#include "Common.h"
#include "CubeFace.h"

#define CUBE_QUEUE_LENGTH 10

KGE_DECLARE_SMART_PTR(Cubes);

class Cubes : public PolygonActor
{
public:
	Cubes(ColorEnum color, float side_length);

	void SetColor(ColorEnum color);

private:
	void BuildCubes(int length);

	void CreateRandomCube();

	CubeFace::Type GetRandomType();

	void AddCube(CubeFacePtr face);

	void RemoveTail();

	CubeFace* Head() const;

private:
	std::list<CubeFace*> cubes_;
	ColorEnum color_;
	float side_length_ = 0;
};
