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

	FaceDesc GetRandomChoice();

	std::vector<FaceDesc> GetRandomChoices();

	void FilterChoices(std::vector<FaceDesc>& choices);

	CubePos GetNewCubePos(FaceDesc desc);

	CubeFace* AppendCubeFace(FaceDesc desc);

private:
	CubeMap cube_map_;
	ColorEnum color_;
	std::vector<CubeFace*> cube_faces_;
};
