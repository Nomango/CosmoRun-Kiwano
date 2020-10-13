#include "MainStage.h"

class DragListener
	: public EventListener
{
public:
	typedef Function<void(Vec2 /* translation */)> Func;

	DragListener(Func func) : func_(func), is_dragging_(false)
	{
	}

	void Handle(Event* evt) override
	{
		if (evt->IsType<MouseDownEvent>())
		{
			is_dragging_ = true;
			prev_pos_ = evt->SafeCast<MouseEvent>()->pos;
		}

		if (is_dragging_ && evt->IsType<MouseMoveEvent>())
		{
			auto pos = evt->SafeCast<MouseEvent>()->pos;
			auto trans = pos - prev_pos_;

			if (func_)
			{
				func_(trans);
			}

			prev_pos_ = pos;
		}

		if (evt->IsType<MouseUpEvent>())
		{
			is_dragging_ = false;
			prev_pos_ = Point();
		}
	}

	bool is_dragging_;
	Point prev_pos_;
	Func func_;
};

MainStage::MainStage()
{
	game_layer_ = new GameLayer(GetSize());
	game_layer_->InitGame();
	AddChild(game_layer_);

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &MainStage::OnKeyDown));

	// 窗口大小变化监听
	AddListener<WindowResizedEvent>([](Event* evt) {
		auto win_evt = evt->SafeCast<WindowResizedEvent>();
		Config::SetWindowSize(win_evt->width, win_evt->height);
	});

	EventListenerPtr lis = new DragListener(Closure(this, &MainStage::Move));
	AddListener(lis);
}

void MainStage::OnKeyDown(Event* evt)
{
	auto key_evt = evt->SafeCast<KeyDownEvent>();

	if (key_evt->code == KeyCode::Num1)
	{
		Config::Color(ColorEnum::Blue);
		game_layer_->SetColor(ColorEnum::Blue);
	}
	else if (key_evt->code == KeyCode::Num2)
	{
		Config::Color(ColorEnum::Purple);
		game_layer_->SetColor(ColorEnum::Purple);
	}
	else if (key_evt->code == KeyCode::Num3)
	{
		Config::Color(ColorEnum::Gold);
		game_layer_->SetColor(ColorEnum::Gold);
	}
}

void MainStage::Move(Vec2 trans)
{
	game_layer_->Move(trans);
}
