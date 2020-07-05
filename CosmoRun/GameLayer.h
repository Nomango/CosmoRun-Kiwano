#pragma once
#include "Common.h"
#include "Cubes.h"

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	static GameLayerPtr Create(ColorEnum color, Size size);

	void Init(ColorEnum color, Size size);

	void SetColor(ColorEnum color);

	void StartGame();

private:
	CubesPtr cubes_;
};
