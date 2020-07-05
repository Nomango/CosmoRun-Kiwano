#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(Background);

class Background : public Actor
{
public:
	static BackgroundPtr Create(ColorEnum color, Size size);

	void Resize(Size size);

	void SetColor(ColorEnum color);

private:
	void Init(ColorEnum color, Size size);

	BrushPtr GetCurrentBrush();

	BrushPtr GetBackgroundBrush(Color top, Color bottom);

	BrushPtr GetShadowBrush();

	void SpawnTriangles(Task* task, Duration dt);

private:
	RectActorPtr bg_rect_;
	RectActorPtr bg_shadow_;
	ActorPtr dynamic_layer_;
	ColorEnum color_ = ColorEnum::Blue;
};
