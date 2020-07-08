#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(Background);

class Background : public Actor
{
public:
	Background(ColorEnum color, Size size);

	void Resize(Size size);

	void SetColor(ColorEnum color);

	void Move(Vec2 trans);

private:
	BrushPtr GetCurrentBrush();

	BrushPtr GetBackgroundBrush(Color top, Color bottom);

	BrushPtr GetShadowBrush();

	void SpawnTriangles(Task* task, Duration dt);

private:
	ColorEnum color_;
	RectActorPtr bg_rect_;
	RectActorPtr bg_shadow_;
	ActorPtr dynamic_layer_;
	BrushPtr triangle_brush_;
};
