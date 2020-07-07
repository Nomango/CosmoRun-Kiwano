#pragma once
#include "Common.h"
#include "Cube.h"

#define CUBE_QUEUE_LENGTH 10

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	GameLayer(ColorEnum color, Size size);

	void SetColor(ColorEnum color);

	void StartGame();

private:
	void BuildCubes(int length);

	void AddCube(CubePtr cube);

private:
	ColorEnum color_;
	std::list<Cube*> cubes_;
};
