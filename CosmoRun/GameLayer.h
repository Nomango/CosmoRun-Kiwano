#pragma once
#include "Common.h"
#include "Cube.h"
#include "Ball.h"
#include "CubeGroup.h"

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	GameLayer(ColorEnum color, Size size);

	void InitGame();

	void StartGame();

	void Move(Vec2 trans);

	void SetColor(ColorEnum color);

private:
	void OnUpdate(Duration dt) override;

private:
	float side_length_;
	float speed_scale_;
	float v_angle_;
	int score_;
	ColorEnum color_;
	BallPtr ball_;
	CubeGroupPtr cube_group_;
};
