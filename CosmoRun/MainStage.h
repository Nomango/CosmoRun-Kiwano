#pragma once
#include "Common.h"
#include "Background.h"
#include "GameLayer.h"

KGE_DECLARE_SMART_PTR(MainStage);

class MainStage : public Stage
{
public:
	MainStage();

	void SetColor(ColorEnum color);

	void OnKeyDown(Event* evt);

	void Move(Vec2 trans);

private:
	ColorEnum color_ = ColorEnum::Blue;
	BackgroundPtr background_;
	GameLayerPtr game_layer_;
};
