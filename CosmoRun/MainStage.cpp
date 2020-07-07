#include "MainStage.h"

MainStage::MainStage()
{
	background_ = new Background(color_, GetSize());
	AddChild(background_);

	game_layer_ = new GameLayer(color_, GetSize());
	game_layer_->SetPosition(GetSize() / 2);
	AddChild(game_layer_);

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &MainStage::OnKeyDown));
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
