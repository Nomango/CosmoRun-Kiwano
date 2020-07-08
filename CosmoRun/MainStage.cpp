#include "MainStage.h"

class DragListener
	: public EventListener
{
public:
	typedef Function<void(Vec2 /* translation */)> Func;

	DragListener(Func func) : func_(func), is_dragging_(false)
	{
		SetCallback(Closure(this, &DragListener::Handle));
	}

	bool ShouldHandle(Event* evt) const override
	{
		if (evt->IsType<MouseDownEvent>()
			|| evt->IsType<MouseUpEvent>()
			|| evt->IsType<MouseMoveEvent>())
			return true;
		return false;
	}

private:
	void Handle(Event* evt)
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
	background_ = new Background(color_, GetSize());
	AddChild(background_);

	game_layer_ = new GameLayer(color_, GetSize());
	game_layer_->Restart();
	game_layer_->SetPosition(GetSize() / 2);
	AddChild(game_layer_);

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &MainStage::OnKeyDown));

	EventListenerPtr lis = new DragListener(Closure(this, &MainStage::Move));
	AddListener(lis);
}

void MainStage::SetColor(ColorEnum color)
{
	background_->SetColor(color);
	game_layer_->SetColor(color);
}

void MainStage::OnKeyDown(Event* evt)
{
	auto key_evt = evt->SafeCast<KeyDownEvent>();

	// 空格键开始游戏
	if (key_evt->code == KeyCode::Space)
	{
		game_layer_->StartGame();
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
}

void MainStage::Move(Vec2 trans)
{
	background_->Move(trans);
	game_layer_->Move(trans);
}
