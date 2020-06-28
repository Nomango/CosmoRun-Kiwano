#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(Background);

class Background : public Actor
{
public:
	enum class Mode
	{
		Blue,
		Purple,
		Gold,
	};

	static BackgroundPtr Create(Size size);

	void Resize(Size size);

	void SetMode(Mode mode);

private:
	void Init(Size size);

	BrushPtr GetCurrentBrush();

	BrushPtr GetBlueBrush();

	BrushPtr GetPurpleBrush();

	BrushPtr GetGoldBrush();

	BrushPtr GetBackgroundBrush(Color top, Color bottom);

	BrushPtr GetShadowBrush();

	void SpawnTriangles(Task* task, Duration dt);

private:
	RectActorPtr bg_rect_;
	RectActorPtr bg_shadow_;
	ActorPtr dynamic_layer_;
	Mode mode_ = Mode::Blue;
};
