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

	void AddRandomFace();

	std::vector<CubeDesc> GetRandomChoices();

	void FilterChoices(std::vector<CubeDesc>& choices);

	CubePos GetNewCubePos(CubeDesc desc);

	CubeFace* CreateCubeFace(CubeDesc desc);

	CubeFace* AddCubeFace(const CubePos& pos, CubeDesc desc);

private:
	CubeMap cube_map_;
	ColorEnum color_;
	std::vector<CubeFace*> cube_faces_;
};
