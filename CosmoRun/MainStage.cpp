#include "MainStage.h"

#define MAX_CUBE_NUMBER		12		// ��������
#define MAX_SPEED_PER_SEC	220.0f	// ����ٶȣ�ÿ��)
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

	// ������ʼ��ť
	play_button_ = new PlayButton;
	play_button_->SetCallback(Closure(this, &MainStage::StartGame));
	wrapper_->AddChild(play_button_);

	// ������Ϸ�������
	gameover_panel_ = new GameOverPanel(size);
	gameover_panel_->SetVisible(false);
	wrapper_->AddChild(gameover_panel_);

	// �������¿�ʼ��ť
	try_again_button_ = new TryAgainButton;
	try_again_button_->SetCallback(Closure(this, &MainStage::Restart));
	try_again_button_->Disable();
	gameover_panel_->AddChild(try_again_button_);

	// ��ʼ����Ϸ
	InitGame();

	// ����Ԫ��λ��
	Resize(size);

	// ���ڴ�С�仯����
	AddListener<WindowResizedEvent>([=](Event* evt) {
		auto win_evt = evt->Cast<WindowResizedEvent>();
		this->Resize(Size(float(win_evt->width), float(win_evt->height)));
	});

	// ÿ��15��ı�һ����ɫ
	AddTask([=](Task*, Duration) { this->SetColor(Config::RandomColor()); }, 15_sec, -1);

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
	// ��һ�������ص���ʼλ��
	background_->ResetTriangles();
	ball_->ResetParticles();
	cube_group_->SetPosition(Point(0, unit / 2));

	play_button_->Show();
	gameover_panel_->Hide();
	try_again_button_->Disable();
}

void MainStage::SetColor(ColorEnum color)
{
	Config::Color(color);
	cube_group_->SetColor(color);
	background_->SetColor(color);
}

void MainStage::Resize(Size size)
{
	Config::SetWindowSize(size);
	this->SetSize(size);

	float unit = Config::Unit();

	background_->Resize(size);
	gameover_panel_->Resize(size);

	wrapper_->SetPosition(size / 2);
	cube_group_->SetPosition(0, unit / 2);
	play_button_->SetPosition(0, size.y / 2 - unit * 2);
	try_again_button_->SetPosition(0, size.y / 2 * 0.7f);
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
		}
		else
		{
			// �ӷ�
			score_++;

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
}
