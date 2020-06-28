#include "GameLayer.h"

#define DEFAULT_CUBES_DEPTH 10

GameLayerPtr GameLayer::Create(ColorMode mode, Size size)
{
	GameLayerPtr ptr = memory::New<GameLayer>();
	if (ptr)
	{
		ptr->Init(mode, size);
	}
	return ptr;
}

void GameLayer::Init(ColorMode mode, Size size)
{
	SetSize(size);

	cubes_ = Cubes::Create(mode, GetWidth() * 0.1f, DEFAULT_CUBES_DEPTH);
	AddChild(cubes_);
}

void GameLayer::SetColor(ColorMode mode)
{
	cubes_->SetColor(mode);
}

void GameLayer::StartGame()
{
}
