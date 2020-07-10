#pragma once
#include "Common.h"
#include "Cube.h"

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

	void RemoveHeadFace();

	CubeDesc GetRandomChoice();

	std::vector<CubeDesc> GetRandomChoices();

	void FilterChoices(std::vector<CubeDesc>& choices);

	CubePos GetNewCubePos(CubeDesc desc);

	CubeFace* AppendCubeFace(CubeDesc desc);

private:
	CubeMap cube_map_;
	ColorEnum color_;
	std::vector<CubeFace*> cube_faces_;
};
