#pragma once
#include "Common.h"

class CanvasButton : public Canvas
{
public:
	CanvasButton(float side_length, float width);

	void SetCallback(Function<void()> click);

protected:
	enum class Status
	{
		Normal,
		Hover,
		Pressed,
	};

	void Redraw();

	virtual void DrawBackground(RenderContextPtr ctx, Status status);

	virtual void DrawButtonText(RenderContextPtr ctx, Status status) = 0;

	void OnButtonEvent(Button* btn, Button::Event evt);

private:
	ButtonPtr button_;
	Status status_;
	Function<void()> click_;
};

class PlayButton : public CanvasButton
{
public:
	PlayButton(float side_length);
};
