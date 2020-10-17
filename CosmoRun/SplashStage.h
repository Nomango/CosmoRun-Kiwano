#pragma once
#include "AutoResizeStage.h"

class SplashStage : public BaseStage
{
public:
	SplashStage();

	void OnEnter() override;

	void LoadResources();

	void OnUnitChanged(float unit) override;

private:
	SpritePtr logo_;
};
