#pragma once
#include "Common.h"
#include "GameLayer.h"

KGE_DECLARE_SMART_PTR(MainStage);

class MainStage : public Stage
{
public:
	MainStage();

	void OnKeyDown(Event* evt);

	void Move(Vec2 trans);

private:
	GameLayerPtr game_layer_;
};
