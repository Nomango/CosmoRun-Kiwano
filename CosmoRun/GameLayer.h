#pragma once
#include "Common.h"
#include "Cube.h"

#define CUBE_QUEUE_LENGTH 10

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	GameLayer(ColorEnum color, Size size);

	void Restart();

	void StartGame();

	void SetColor(ColorEnum color);

private:
	void InitCubes(int length);

	void CreateRandomCube();

	CubeDesc GetRandomCubeDesc() const;

	CubeFace* CreateCubeFace(CubeFace::Type type, Direction d);

	CubeFace* AddCubeFace(int x, int y, int z, CubeFace::Type type, Direction d);

private:
	CubeMap cube_map_;
	ColorEnum color_;
	std::vector<CubeFace*> cube_faces_;
};
