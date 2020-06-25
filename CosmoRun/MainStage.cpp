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
}
