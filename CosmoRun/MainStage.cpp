#include "MainStage.h"

#define MAX_CUBE_NUMBER		12		// 方块数量
#define MAX_SPEED_PER_SEC	220.0f	// 最大速度（每秒)
#define MAX_SPEED_SCORE		300		// 得分300分时游戏速度达到最大

MainStage::MainStage()
	: speed_scale_(0.0f)
	, score_(0)
	, best_score_(0)
	, status_(GameStatus::Ready)
{
	float unit = Config::Unit();
	Size size = GetSize();

	ActorPtr wrapper = new Actor;
	wrapper->SetPosition(size / 2);
	this->AddChild(wrapper);

	// 创建背景
	background_ = new Background(size);
	wrapper->AddChild(background_);

	// 创建方块层
	cube_group_ = new CubeGroup;
	cube_group_->SetPosition(0, unit / 2);
	wrapper->AddChild(cube_group_);

	// 创建小球
	ball_ = new Ball(unit * 0.2f);
	wrapper->AddChild(ball_);

	// 创建开始按钮
	play_button_ = new PlayButton;
	play_button_->SetPosition(0, unit * 1.5f);
	play_button_->SetCallback(Closure(this, &MainStage::StartGame));
	wrapper->AddChild(play_button_);

	// 创建游戏结束面板
	gameover_panel_ = new GameOverPanel(size);
	gameover_panel_->SetVisible(false);
	wrapper->AddChild(gameover_panel_);

	// 创建重新开始按钮
	try_again_button_ = new TryAgainButton;
	try_again_button_->SetCallback(Closure(this, &MainStage::Restart));
	try_again_button_->SetPosition(0, size.y / 2 * 0.7f);
	try_again_button_->Disable();
	gameover_panel_->AddChild(try_again_button_);

	// 初始化游戏
	InitGame();

	// 窗口大小变化监听
	AddListener<WindowResizedEvent>([](Event* evt) {
		auto win_evt = evt->Cast<WindowResizedEvent>();
		Config::SetWindowSize(win_evt->width, win_evt->height);
	});

#ifdef KGE_DEBUG
	// 下面时调试时使用的一些工具

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &MainStage::OnKeyDown));

	// 调试状态下支持右键拖动
	class DragListener : public MouseDragEventListener
	{
		MainStage* stage_;
	public:
		DragListener(MainStage* stage) : stage_(stage) {}

		void OnDragging(const Point& offset, MouseButton button) override
		{
			if (button == MouseButton::Right)
				stage_->Move(offset);
		}
	};

	EventListenerPtr lis = new DragListener(this);
	AddListener(lis);
#endif
}

void MainStage::InitGame()
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

void MainStage::StartGame()
{
	KGE_LOG("Game start!");
	status_ = GameStatus::Running;
	play_button_->Hide();
}

void MainStage::GameOver()
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

void MainStage::Restart()
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

void MainStage::SetColor(ColorEnum color)
{
	cube_group_->SetColor(color);
	background_->SetColor(color);
}

void MainStage::OnKeyDown(Event* evt)
{
	auto key_evt = evt->Cast<KeyDownEvent>();

	if (key_evt->code == KeyCode::R)
	{
		Restart();
	}

	if (key_evt->code == KeyCode::Num1)
	{
		Config::Color(ColorEnum::Blue);
		SetColor(ColorEnum::Blue);
	}
	else if (key_evt->code == KeyCode::Num2)
	{
		Config::Color(ColorEnum::Purple);
		SetColor(ColorEnum::Purple);
	}
	else if (key_evt->code == KeyCode::Num3)
	{
		Config::Color(ColorEnum::Gold);
		SetColor(ColorEnum::Gold);
	}
}

void MainStage::Move(Vec2 trans)
{
	background_->MoveTriangles(trans);
	ball_->MoveParticles(trans);
	cube_group_->MoveBy(trans);
}

void MainStage::OnUpdate(Duration dt)
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
	float unit = Config::NormalizeUnit();
	Vec2 v_angle = ball_->GetVelocityAngle();
	Vec2 v = v_angle * MAX_SPEED_PER_SEC * unit * speed_scale_ * dt.GetSeconds();
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
