#pragma once
#include "BaseStage.h"

class SplashStage : public BaseStage
{
public:
	SplashStage();

	void OnEnter() override;

	void LoadResources();

	void OnUnitChanged(float unit) override;

private:
	RefPtr<Sprite> logo_;
};
