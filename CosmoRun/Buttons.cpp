#include "Buttons.h"
#include "Lang.h"
#include "Font.h"

CanvasButton::CanvasButton(float side_length, float width)
	: status_(Status::Normal)
{
	Size size(side_length * (1 + width), side_length);
	ResizeAndClear(size);

	SetAnchor(0.5f, 0.5f);

	button_ = new Button(Closure(this, &CanvasButton::OnButtonEvent));
	AddComponent(button_);
}

void CanvasButton::Hide()
{
	StartAnimation(animation::FadeOut(150_msec));
	button_->SetEnabled(false);
}

void CanvasButton::Show()
{
	StartAnimation(animation::FadeIn(300_msec));
	button_->SetEnabled(true);
}

void CanvasButton::SetCallback(Function<void()> click)
{
	click_ = click;
}

void CanvasButton::Redraw()
{
	auto ctx = GetContext2D();

	ctx->BeginDraw();
	ctx->Clear();
	DrawBackground(ctx, status_);
	DrawButtonText(ctx, status_);
	ctx->EndDraw();
}

void CanvasButton::OnButtonEvent(Button* btn, Button::Event evt)
{
	switch (evt)
	{
	case Button::Event::MouseEntered:
		status_ = Status::Hover;
		break;
	case Button::Event::MouseExited:
		status_ = Status::Normal;
		break;
	case Button::Event::Pressed:
		status_ = Status::Pressed;
		break;
	case Button::Event::Released:
		status_ = Status::Hover;
		break;
	case Button::Event::Clicked:
		if (click_)
		{
			click_();
		}
		break;
	}
	Redraw();
}

PlayButton::PlayButton(float side_length)
	: CanvasButton(side_length, 2.0f)
{
	Redraw();

	// Ö´ÐÐ¶¯»­
	auto action = animation::ScaleBy(1_sec, Vec2(0.15f, 0.15f));
	auto group = animation::Group({ action, action.Reverse() }).Loops(-1);
	StartAnimation(group);
}

void PlayButton::DrawBackground(CanvasRenderContextPtr ctx, Status status)
{
	switch (status)
	{
	case Status::Normal:
		ctx->SetFillColor(Color::Rgba(0, 171, 255, 0.3f));
		break;
	case Status::Hover:
		ctx->SetFillColor(Color::Rgba(0, 141, 225, 0.3f));
		break;
	case Status::Pressed:
		ctx->SetFillColor(Color::Rgba(0, 80, 175, 0.3f));
		break;
	}

	float w = GetWidth();
	float h = GetHeight();

	ShapePtr polygon = Shape::CreatePolygon({
		Point(0, h / 2),
		Point(h / 2, 0),
		Point(w - h / 2, 0),
		Point(w, h / 2),
		Point(w - h / 2, h),
		Point(h / 2, h),
	});
	ctx->FillShape(polygon);
}

void PlayButton::DrawButtonText(CanvasRenderContextPtr ctx, Status status)
{
	TextStyle style(GillSansFont(48, true));
	style.fill_brush = new Brush(Color::White);

	TextLayoutPtr layout = new TextLayout(Lang::Get("main", "play"), style);

	Point pos((GetSize() - layout->GetSize()) / 2);
	ctx->DrawTextLayout(layout, pos);
}
