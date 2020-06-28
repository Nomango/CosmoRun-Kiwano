#pragma once
#include "Common.h"
#include "Cubes.h"

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	static GameLayerPtr Create(ColorMode mode, Size size);

	void Init(ColorMode mode, Size size);

	void SetColor(ColorMode mode);

	void StartGame();

private:
	CubesPtr cubes_;
};
