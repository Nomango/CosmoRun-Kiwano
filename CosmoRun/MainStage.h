#pragma once
#include <kiwano/kiwano.h>

using namespace kiwano;

KGE_DECLARE_SMART_PTR(MainStage);
class MainStage : public Stage
{
public:
	static MainStagePtr Create();

private:
	CanvasPtr canvas_;
};
