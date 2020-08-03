#include "GameLayer.h"

#define MAX_CUBE_NUMBER		12		// 方块数量
#define MAX_SPEED_PER_SEC	220.0f	// 最大速度（每秒)
#define MAX_SPEED_SCORE		300		// 得分300分时游戏速度达到最大

GameLayer::GameLayer(ColorEnum color, Size size)
	: color_(color)
	, speed_scale_(0.0f)
	, score_(0)
	, started_(false)
{
	SetPosition(size / 2);
	side_length_ = size.x * 0.08f;

	// 创建背景
	background_ = new Background(color_, size);
	AddChild(background_);

	// 创建方块层
	cube_group_ = new CubeGroup(side_length_);
	cube_group_->SetPosition(0, side_length_ / 2);
	this->AddChild(cube_group_);

	// 创建小球
	ball_ = new Ball(side_length_ * 0.2f);
	this->AddChild(ball_);
}

void GameLayer::InitGame()
{
	// 初始变量
	started_ = false;
	speed_scale_ = 0.7f;
	score_ = 0;

	// 初始化方块层
	cube_group_->InitCubes(MAX_CUBE_NUMBER);

	auto tail = cube_group_->GetTail();
	ball_->SetOn(tail);
	ball_->SetDirection(tail->GetDirection());
}

void GameLayer::StartGame()
{
	KGE_LOG("Game start!");
	started_ = true;
}

void GameLayer::GameOver()
{
	KGE_LOG("Game over!");
	started_ = false;

	ball_->Die();
}

void GameLayer::Restart()
{
	KGE_LOG("Game restart!");
	InitGame();

	// 用一个动画回到初始位置
	background_->ResetTriangles();
	ball_->ResetParticles();
	cube_group_->SetPosition(Point(0, side_length_ / 2));
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
	background_->MoveTriangles(trans);
	ball_->MoveParticles(trans);
	cube_group_->MoveBy(trans);
}

void GameLayer::OnUpdate(Duration dt)
{
	if (Input::GetInstance().WasPressed(KeyCode::Space)
		|| Input::GetInstance().WasPressed(MouseButton::Left))
	{
		if (!started_)
		{
			started_ = true;
		}
		else
		{
			ball_->Turn();
		}
	}

	if (!started_)
		return;

	// 沿小球的方向移动
	Vec2 v_angle = ball_->GetVelocityAngle();
	Vec2 v = v_angle * MAX_SPEED_PER_SEC * speed_scale_ * dt.Seconds();
	Move(v);

	// 判断小球是否出界
	if (!ball_->IsSafe())
	{
		auto current_face = ball_->GetOn();

		ball_->ToNext();
		if (!ball_->IsSafe())
		{
			this->GameOver();
		}
		else
		{
			// 加分
			score_++;

			// 调整游戏速度
			speed_scale_ = std::min(1.0f, float(score_) / float(MAX_SPEED_SCORE));
			speed_scale_ = 0.7f + speed_scale_ * 0.3f;

			// 生成新方块面
			cube_group_->AddRandomFace();

			// 移除尾方块面
			if (current_face != cube_group_->GetTail())
			{
				cube_group_->RemoveTailFace();
			}
		}
	}
}
