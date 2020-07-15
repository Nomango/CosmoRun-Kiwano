#include "GameLayer.h"

#define MAX_CUBE_NUMBER		12		// 方块数量
#define MAX_SPEED_PER_SEC	160.0f	// 最大速度（每秒)
#define MAX_SPEED_SCORE		300		// 得分300分时游戏速度达到最大

GameLayer::GameLayer(ColorEnum color, Size size)
	: color_(color)
	, speed_scale_(0.0f)
	, v_angle_(0.0f)
	, score_(0)
{
	SetSize(size);
	side_length_ = GetWidth() * 0.08f;

	// 创建小球
	ball_ = new Ball(side_length_ * 0.18f);
	ball_->SetPositionY(-side_length_ / 2);
	this->AddChild(ball_, 1);

	// 创建方块层
	cube_group_ = new CubeGroup(side_length_);
	this->AddChild(cube_group_);
}

void GameLayer::InitGame()
{
	// 初始变量
	speed_scale_ = 0.5f;
	v_angle_ = 0.0f;
	score_ = 0;

	// 初始化方块层
	cube_group_->InitCubes(MAX_CUBE_NUMBER);

	auto tail = cube_group_->GetTail();
	ball_->SetOn(tail);
}

void GameLayer::StartGame()
{
}

void GameLayer::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		cube_group_->SetColor(color);
	}
}

void GameLayer::Move(Vec2 trans)
{
	cube_group_->Move(trans);
	ball_->Move(trans);
}

void GameLayer::OnUpdate(Duration dt)
{
	//AddRandomFace();
	//RemoveTailFace();

	/*Vec2 v = Vec2(math::Cos(v_angle_), math::Sin(v_angle_));
	v *= MAX_SPEED_PER_SEC * speed_scale_;
	Move(v);*/
}
