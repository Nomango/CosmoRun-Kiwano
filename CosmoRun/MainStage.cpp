#include "MainStage.h"

MainStagePtr MainStage::Create()
{
	MainStagePtr ptr = memory::New<MainStage>();
	if (ptr)
	{
		ptr->Init();
	}
	return ptr;
}

void MainStage::Init()
{
	background_ = Background::Create(mode_, GetSize());
	AddChild(background_);

	game_layer_ = GameLayer::Create(mode_, GetSize());
	AddChild(game_layer_);

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &MainStage::OnKeyDown));
}

void MainStage::SetColor(ColorMode mode)
{
	background_->SetColor(mode);
	game_layer_->SetColor(mode);
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
		SetColor(ColorMode::Blue);
	}
	else if (key_evt->code == KeyCode::Num2)
	{
		SetColor(ColorMode::Purple);
	}
	else if (key_evt->code == KeyCode::Num3)
	{
		SetColor(ColorMode::Gold);
	}
}
