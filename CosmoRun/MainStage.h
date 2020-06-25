#pragma once
#include "Common.h"
#include "Background.h"

KGE_DECLARE_SMART_PTR(MainStage);

class MainStage : public Stage
{
public:
	static MainStagePtr Create();

	void Init();

private:
	BackgroundPtr background_;
};
