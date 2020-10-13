#include "GameLayer.h"

#define MAX_CUBE_NUMBER		12		// 方块数量
#define MAX_SPEED_PER_SEC	220.0f	// 最大速度（每秒)
#define MAX_SPEED_SCORE		300		// 得分300分时游戏速度达到最大

GameLayer::GameLayer(Size size)
	: speed_scale_(0.0f)
	, score_(0)
	, best_score_(0)
	, status_(GameStatus::Ready)
{
	SetPosition(size / 2);

	float unit = Config::Unit();

	// 创建背景
	background_ = new Background(size);
	AddChild(background_);

	// 创建方块层
	cube_group_ = new CubeGroup;
	cube_group_->SetPosition(0, unit / 2);
	this->AddChild(cube_group_);

	// 创建小球
	ball_ = new Ball(unit * 0.2f);
	this->AddChild(ball_);

	// 创建开始按钮
	play_button_ = new PlayButton;
	play_button_->SetPosition(0, unit * 1.5f);
	play_button_->SetCallback(Closure(this, &GameLayer::StartGame));
	this->AddChild(play_button_);

	// 创建游戏结束面板
	gameover_panel_ = new GameOverPanel(size);
	gameover_panel_->SetVisible(false);
	this->AddChild(gameover_panel_);

	// 创建重新开始按钮
	try_again_button_ = new TryAgainButton;
	try_again_button_->SetCallback(Closure(this, &GameLayer::Restart));
	try_again_button_->SetPosition(0, size.y / 2 * 0.7f);
	try_again_button_->Disable();
	gameover_panel_->AddChild(try_again_button_);
}

void GameLayer::InitGame()
{
	// 初始变量
	status_ = GameStatus::Ready;
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
	status_ = GameStatus::Running;
	play_button_->Hide();
}

void GameLayer::GameOver()
{
	KGE_LOG("Game over!");
	status_ = GameStatus::Gameover;

	bool is_best = false;
	if (score_ > best_score_)
	{
		best_score_ = score_;
		is_best = true;
	}

	// 更新得分
	gameover_panel_->SetScore(score_, best_score_, is_best);

	ball_->Die();
	gameover_panel_->Show();
	try_again_button_->Enable();
}

void GameLayer::Restart()
{
	KGE_LOG("Game restart!");
	InitGame();

	float unit = Config::Unit();
	// 用一个动画回到初始位置
	background_->ResetTriangles();
	ball_->ResetParticles();
	cube_group_->SetPosition(Point(0, unit / 2));

	play_button_->Show();
	gameover_panel_->Hide();
	try_again_button_->Disable();
}

void GameLayer::SetColor(ColorEnum color)
{
	cube_group_->SetColor(color);
	background_->SetColor(color);
}

void GameLayer::Move(Vec2 trans)
{
	background_->MoveTriangles(trans);
	ball_->MoveParticles(trans);
	cube_group_->MoveBy(trans);
}

void GameLayer::OnUpdate(Duration dt)
{
	if (status_ == GameStatus::Gameover)
		return;

	if (status_ == GameStatus::Ready)
	{
		if (Input::GetInstance().WasPressed(KeyCode::Space))
		{
			StartGame();
		}
	}
	else if (status_ == GameStatus::Running)
	{
		if (Input::GetInstance().WasPressed(KeyCode::Space)
			|| Input::GetInstance().WasPressed(MouseButton::Left))
		{
			ball_->Turn();
		}
	}

	if (status_ != GameStatus::Running)
		return;

	// 沿小球的方向移动
	Vec2 v_angle = ball_->GetVelocityAngle();
	Vec2 v = v_angle * MAX_SPEED_PER_SEC * speed_scale_ * dt.GetSeconds();
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
