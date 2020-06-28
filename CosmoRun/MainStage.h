#pragma once
#include "Common.h"
#include "Background.h"
#include "GameLayer.h"

KGE_DECLARE_SMART_PTR(MainStage);

class MainStage : public Stage
{
public:
	static MainStagePtr Create();

	void Init();

	void SetColor(ColorMode mode);

	void OnKeyDown(Event* evt);

private:
	ColorMode mode_ = ColorMode::Blue;
	BackgroundPtr background_;
	GameLayerPtr game_layer_;
};
