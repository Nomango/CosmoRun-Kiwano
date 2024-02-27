#pragma once
#include "Common.h"

class Background : public Actor
{
public:
	Background(Size size);

	void Resize(Size size);

	void SetColor(ColorEnum color);

	void MoveTriangles(Vec2 trans);

	void ResetTriangles();

private:
	LinearGradientStyle GetBrushStyle(ColorEnum color);

	RefPtr<Brush> GetShadowBrush();

	void SpawnTriangles(Task* task, Duration dt);

private:
	RefPtr<RectActor> bg_rect_;
	RefPtr<RectActor> bg_shadow_;
	RefPtr<Actor> dynamic_layer_;
	RefPtr<Brush> triangle_brush_;

	LinearGradientStyle current_style_;
};
