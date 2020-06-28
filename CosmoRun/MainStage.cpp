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
	background_ = Background::Create(GetSize());
	AddChild(background_);

	AddListener<KeyDownEvent>(Closure(this, &MainStage::OnKeyDown));
}

void MainStage::OnKeyDown(Event* evt)
{
	auto key_evt = evt->SafeCast<KeyDownEvent>();
	if (key_evt->code == KeyCode::Num1)
	{
		background_->SetMode(Background::Mode::Blue);
	}
	else if (key_evt->code == KeyCode::Num2)
	{
		background_->SetMode(Background::Mode::Purple);
	}
	else if (key_evt->code == KeyCode::Num3)
	{
		background_->SetMode(Background::Mode::Gold);
	}
}
