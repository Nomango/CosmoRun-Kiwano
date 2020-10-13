#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(Background);

class Background : public Actor
{
public:
	Background(Size size);

	void Resize(Size size);

	void SetColor(ColorEnum color);

	void MoveTriangles(Vec2 trans);

	void ResetTriangles();

private:
	BrushPtr GetCurrentBrush();

	BrushPtr GetBackgroundBrush(Color top, Color bottom);

	BrushPtr GetShadowBrush();

	void SpawnTriangles(Task* task, Duration dt);

private:
	RectActorPtr bg_rect_;
	RectActorPtr bg_shadow_;
	ActorPtr dynamic_layer_;
	BrushPtr triangle_brush_;
};
