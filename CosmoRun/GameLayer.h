#pragma once
#include "Common.h"
#include "Cube.h"
#include "Ball.h"

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	GameLayer(ColorEnum color, Size size);

	void InitGame();

	void StartGame();

	void SetColor(ColorEnum color);

	void RemoveFace(Actor* face);

	void Move(Vec2 trans);

private:
	void InitCubes(int length);

	void OnUpdate(Duration dt) override;

	void AddRandomFace();

	CubeFace* AppendCubeFace(FaceDesc desc);

	void RemoveTailFace();

	void RemoveHeadFace();

	std::vector<FaceDesc> GetRandomChoices();

	CubePos GetNewCubePos(FaceDesc desc);

private:
	float side_length_;
	ColorEnum color_;
	CubeFace* tail_;
	BallPtr ball_;
	ActorPtr cube_group_;
	CubeMap cube_map_;
	std::list<CubeFace*> hide_faces_;
};
