#include "Buttons.h"

CanvasButton::CanvasButton(float side_length, float width)
	: Canvas(Size(side_length * (1 + width), side_length))
	, status_(Status::Normal)
{
	button_ = new Button(Closure(this, &CanvasButton::OnButtonEvent));
	AddComponent(button_);

	Redraw();
}

void CanvasButton::SetCallback(Function<void()> click)
{
	click_ = click;
}

void CanvasButton::Redraw()
{
	auto ctx = GetContext2D();
	ctx->Clear();

	DrawBackground(ctx, status_);
	DrawButtonText(ctx, status_);
}

void CanvasButton::DrawBackground(RenderContextPtr ctx, Status status)
{
}

void CanvasButton::OnButtonEvent(Button* btn, Button::Event evt)
{
	switch (evt)
	{
	case Button::Event::Click:
		if (click_)
		{
			click_();
		}
		status_ = Status::Hover;
		break;
	case Button::Event::Pressed:
		status_ = Status::Pressed;
		break;
	case Button::Event::MouseOver:
		status_ = Status::Hover;
		break;
	case Button::Event::MouseOut:
		status_ = Status::Normal;
		break;
	}
	Redraw();
}

PlayButton::PlayButton(float side_length)
	: CanvasButton(side_length, 2.0f)
{
}
