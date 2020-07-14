#pragma once
#include "Common.h"
#include "Cube.h"

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	GameLayer(ColorEnum color, Size size);

	void StartGame();

	void SetColor(ColorEnum color);

	void RemoveFace(Actor* face);

private:
	void InitCubes(int length);

	void TickTest(Task* task, Duration dt);

	void AddRandomFace();

	CubeFace* AppendCubeFace(FaceDesc desc);

	void RemoveTailFace();

	void RemoveHeadFace();

	std::vector<FaceDesc> GetRandomChoices();

	CubePos GetNewCubePos(FaceDesc desc);

private:
	CubeMap cube_map_;
	ColorEnum color_;
	CubeFace* current_face_;
	std::list<CubeFace*> hide_faces_;
};
