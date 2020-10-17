#include "MainStage.h"
#include "Lang.h"
#include "resource.h"

#define MAX_CUBE_NUMBER		12		// ��������
#define MAX_SPEED_PER_SEC	260.0f	// ����ٶȣ�ÿ��)
#define MAX_SPEED_SCORE		300		// �÷�300��ʱ��Ϸ�ٶȴﵽ���

MainStage::MainStage()
	: speed_scale_(0.0f)
	, score_(0)
	, best_score_(0)
	, status_(GameStatus::Ready)
{
	float unit = Config::Unit();
	Size size = GetSize();

	wrapper_ = new Actor;
	this->AddChild(wrapper_);

	// ��������
	background_ = new Background(size);
	wrapper_->AddChild(background_);

	// ���������
	cube_group_ = new CubeGroup;
	wrapper_->AddChild(cube_group_);

	// ����С��
	ball_ = new Ball;
	wrapper_->AddChild(ball_);

	// ��������
	title_ = new CustomText(Lang::Get("main", "title"), 120);
	wrapper_->AddChild(title_);

	// ������ʼ��ť
	play_button_ = new PlayButton;
	play_button_->SetCallback(Closure(this, &MainStage::StartGame));
	wrapper_->AddChild(play_button_);

	// �����Ʒ�����
	score_text_ = new CustomText(" ", 48);
	score_text_->SetOpacity(0);
	wrapper_->AddChild(score_text_);

	// ������߷�����
	best_score_text_ = new CustomText(" ", 48);
	best_score_text_->SetOpacity(0);
	wrapper_->AddChild(best_score_text_);

	// �����Ʒ����
	score_board_ = new ScoreBoard(size);
	score_board_->SetVisible(false);
	wrapper_->AddChild(score_board_);

	// �������¿�ʼ��ť
	try_again_button_ = new TryAgainButton;
	try_again_button_->SetCallback(Closure(this, &MainStage::Restart));
	try_again_button_->Disable();
	score_board_->AddChild(try_again_button_);

	// ��ʼ����Ϸ
	InitGame();

	// ÿ��15��ı�һ����ɫ
	AddTask([=](Task*, Duration) { this->SetColor(Config::RandomColor()); }, 15_sec, -1);

	OnUnitChanged(Config::Unit());

#ifdef KGE_DEBUG
	// �����ǵ���ʱʹ�õ�һЩ����

	// ��������
	AddListener<KeyDownEvent>(Closure(this, &MainStage::OnKeyDown));

	// ����״̬��֧���Ҽ��϶�
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
	// ��ʼ����
	status_ = GameStatus::Ready;
	speed_scale_ = 0.7f;
	score_ = 0;

	// ��ʼ�������
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
	title_->StartAnimation(animation::FadeOut(300_msec));
	best_score_text_->StartAnimation(animation::FadeOut(300_msec));
	score_text_->StartAnimation(animation::FadeIn(300_msec).Delay(300_msec));
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

	// ���µ÷�
	score_board_->SetScore(score_, best_score_, is_best);

	// ������������
	Music::GetInstance().PlayDie();

	// ��С����
	this->StartAnimation(Music::GetInstance().LowVolume());

	ball_->Die();
	score_board_->Show();
	score_text_->StartAnimation(animation::FadeOut(300_msec));

	AddTask([=](Task*, Duration) {
		try_again_button_->Enable();
	}, 1500_msec, 1);
}

void MainStage::Restart()
{
	KGE_LOG("Game restart!");
	status_ = GameStatus::Restarting;
	score_text_->SetText(" ");
	best_score_text_->SetText(Lang::Get("gameover", "best_score") + " " + std::to_string(best_score_));

	// ����1���������з���
	cube_group_->HideAllCubes();
	score_board_->Hide();
	try_again_button_->Disable();

	// ����2����ʾ�µķ���
	AddTask([=](Task*, Duration) {
		InitGame();
		float unit = Config::Unit();
		cube_group_->SetPosition(Point(0, unit / 2));
	}, 500_msec, 1);

	// ����3����ʾ������
	AddTask([=](Task*, Duration) {
		background_->ResetTriangles();
		ball_->ResetParticles();
		play_button_->Show();
		title_->StartAnimation(animation::FadeIn(300_msec));
		best_score_text_->StartAnimation(animation::FadeIn(300_msec));
		status_ = GameStatus::Ready;
	}, 1000_msec, 1);

	// ��������
	this->StartAnimation(Music::GetInstance().HighVolume());
}

void MainStage::SetColor(ColorEnum color)
{
	Config::Color(color);
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
		SetColor(ColorEnum::Blue);
	}
	else if (key_evt->code == KeyCode::Num2)
	{
		SetColor(ColorEnum::Purple);
	}
	else if (key_evt->code == KeyCode::Num3)
	{
		SetColor(ColorEnum::Gold);
	}
	else if (key_evt->code == KeyCode::Num4)
	{
		SetColor(Config::RandomColor());
	}
}

void MainStage::OnEnter()
{
	// ��Ϸ��������
	Music::GetInstance().PlayBackground();
}

void MainStage::Move(Vec2 trans)
{
	background_->MoveTriangles(trans);
	ball_->MoveParticles(trans);
	cube_group_->MoveBy(trans);
}

void MainStage::OnUpdate(Duration dt)
{
	bool space_pressed = Input::GetInstance().WasPressed(KeyCode::Space);
	bool mouse_pressed = Input::GetInstance().WasPressed(MouseButton::Left);
	if (status_ == GameStatus::Ready && space_pressed)
	{
		StartGame();
	}
	else if (status_ == GameStatus::Running)
	{
		if (space_pressed || mouse_pressed)
		{
			ball_->Turn();
		}
	}
	else if (status_ == GameStatus::Gameover && space_pressed)
	{
		Restart();
	}

	if (status_ != GameStatus::Running)
		return;

	// ��С��ķ����ƶ�
	float unit = Config::NormalizeUnit();
	Vec2 v_angle = ball_->GetVelocityAngle();
	Vec2 v = v_angle * MAX_SPEED_PER_SEC * unit * speed_scale_ * dt.GetSeconds();
	Move(v);

	// �ж�С���Ƿ����
	if (!ball_->IsSafe())
	{
		auto current_face = ball_->GetOn();

		ball_->ToNext();
		if (!ball_->IsSafe())
		{
			this->GameOver();
			return;
		}
		// �ӷ�
		score_++;
		score_text_->SetText(std::to_string(score_));

		// ������Ϸ�ٶ�
		speed_scale_ = std::min(1.0f, float(score_) / float(MAX_SPEED_SCORE));
		speed_scale_ = 0.7f + speed_scale_ * 0.3f;

		// �����·�����
		cube_group_->AddRandomFace();

		// �Ƴ�β������
		if (current_face != cube_group_->GetTail())
		{
			cube_group_->RemoveTailFace();
		}
	}
}

void MainStage::OnUnitChanged(float unit)
{
	Size size = GetSize();

	background_->Resize(size);
	score_board_->Resize(size);

	wrapper_->SetPosition(size / 2);
	title_->SetPosition(0, unit * 2 - size.y / 2);
	score_text_->SetPosition(0, unit - size.y / 2);
	best_score_text_->SetPosition(0, -unit);
	cube_group_->SetPosition(0, unit / 2);
	play_button_->SetPosition(0, size.y / 2 - unit * 2);
	try_again_button_->SetPosition(0, size.y / 2 * 0.7f);
}
