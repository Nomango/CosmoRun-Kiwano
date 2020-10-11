#pragma once
#include "Common.h"

class CanvasButton : public Canvas
{
public:
	CanvasButton(float side_length, float width);

	void Hide();

	void Show();

	void SetCallback(Function<void()> click);

protected:
	enum class Status
	{
		Normal,
		Hover,
		Pressed,
	};

	void Redraw();

	virtual void DrawBackground(CanvasRenderContextPtr ctx, Status status) = 0;

	virtual void DrawButtonText(CanvasRenderContextPtr ctx, Status status) = 0;

	void OnButtonEvent(Button* btn, Button::Event evt);

private:
	Status status_;
	ButtonPtr button_;
	Function<void()> click_;
};

class PlayButton : public CanvasButton
{
public:
	PlayButton(float side_length);

	void DrawBackground(CanvasRenderContextPtr ctx, Status status) override;

	void DrawButtonText(CanvasRenderContextPtr ctx, Status status) override;
};
