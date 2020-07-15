#include "GameLayer.h"

#define MAX_CUBE_NUMBER		12		// 方块数量
#define MAX_SPEED_PER_SEC	160.0f	// 最大速度（每秒)
#define MAX_SPEED_SCORE		300		// 得分300分时游戏速度达到最大

GameLayer::GameLayer(ColorEnum color, Size size)
	: color_(color)
	, speed_scale_(0.0f)
	, score_(0)
	, started_(false)
{
	SetSize(size);
	side_length_ = GetWidth() * 0.08f;

	// 创建背景
	background_ = new Background(color_, GetSize());
	AddChild(background_);

	// 创建方块层
	cube_group_ = new CubeGroup(side_length_);
	cube_group_->SetPosition(GetSize() / 2);
	cube_group_->Move(0, side_length_ / 2);
	this->AddChild(cube_group_);

	// 创建小球
	ball_ = new Ball(side_length_ * 0.18f);
	ball_->SetPosition(GetSize() / 2);
	this->AddChild(ball_);
}

void GameLayer::InitGame()
{
	// 初始变量
	started_ = false;
	speed_scale_ = 0.5f;
	score_ = 0;

	// 初始化方块层
	cube_group_->InitCubes(MAX_CUBE_NUMBER);

	auto tail = cube_group_->GetTail();
	ball_->SetOn(tail);
}

void GameLayer::StartGame()
{
	started_ = true;
}

void GameLayer::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		cube_group_->SetColor(color);
		background_->SetColor(color);
	}
}

void GameLayer::Move(Vec2 trans)
{
	background_->Move(trans);
	ball_->Move(trans);
	cube_group_->Move(trans);
}

void GameLayer::OnUpdate(Duration dt)
{
	//AddRandomFace();
	//RemoveTailFace();

	if (!started_)
		return;

	if (Input::GetInstance().WasPressed(KeyCode::Space)
		|| Input::GetInstance().WasPressed(MouseButton::Left))
	{
		ball_->Turn();
	}

	if (Input::GetInstance().WasPressed(KeyCode::I))
	{
		ball_->SetDirection(Direction::Up);
	}
	else if (Input::GetInstance().WasPressed(KeyCode::K))
	{
		ball_->SetDirection(Direction::Down);
	}
	else if (Input::GetInstance().WasPressed(KeyCode::U))
	{
		ball_->SetDirection(Direction::LeftUp);
	}
	else if (Input::GetInstance().WasPressed(KeyCode::J))
	{
		ball_->SetDirection(Direction::LeftDown);
	}
	else if (Input::GetInstance().WasPressed(KeyCode::O))
	{
		ball_->SetDirection(Direction::RightUp);
	}
	else if (Input::GetInstance().WasPressed(KeyCode::L))
	{
		ball_->SetDirection(Direction::RightDown);
	}

	// 沿小球的方向移动
	float v_angle = ball_->GetVelocityAngle();
	Vec2 v = Vec2(-math::Cos(v_angle), math::Sin(v_angle));
	v *= MAX_SPEED_PER_SEC * speed_scale_ * dt.Seconds();
	Move(v);
}
