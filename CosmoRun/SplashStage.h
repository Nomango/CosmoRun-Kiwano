#pragma once
#include "Common.h"

class SplashStage : public Stage
{
public:
	SplashStage();

	void OnEnter() override;

	void LoadResources();
};
