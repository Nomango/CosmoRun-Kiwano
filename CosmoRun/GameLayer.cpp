#include "GameLayer.h"

#define DEFAULT_CUBES_DEPTH 10

GameLayerPtr GameLayer::Create(ColorEnum color, Size size)
{
	GameLayerPtr ptr = memory::New<GameLayer>();
	if (ptr)
	{
		ptr->Init(color, size);
	}
	return ptr;
}

void GameLayer::Init(ColorEnum color, Size size)
{
	SetSize(size);

	cubes_ = new Cubes(color, GetWidth() * 0.1f);
	AddChild(cubes_);
}

void GameLayer::SetColor(ColorEnum color)
{
	cubes_->SetColor(color);
}

void GameLayer::StartGame()
{
}
