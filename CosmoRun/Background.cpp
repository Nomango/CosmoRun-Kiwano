#include "Background.h"

BackgroundPtr Background::Create(Size size)
{
	BackgroundPtr ptr = memory::New<Background>();
	if (ptr)
	{
		ptr->Init(size);
	}
	return ptr;
}

void Background::Init(Size size)
{
	bg_rect_ = RectActor::Create(size);
	AddChild(bg_rect_);

	dynamic_layer_ = Actor::Create();
	AddChild(dynamic_layer_);

	bg_shadow_ = RectActor::Create(size);
	AddChild(bg_shadow_);

	Resize(size);
}

void Background::Resize(Size size)
{
	SetSize(size);

	bg_rect_->SetRectSize(size);
	bg_rect_->SetFillBrush(GetCurrentBrush());

	bg_shadow_->SetRectSize(size);
	bg_shadow_->SetFillBrush(GetShadowBrush());
}

void Background::SetMode(Mode mode)
{
	if (mode_ != mode)
	{
		mode_ = mode;

		// µ­Èëµ­³öÊ½ÇÐ»»±³¾°É«
		auto action = Tween::Group({
			Tween::FadeOut(150_msec).DoneCallback([=](Actor*) { this->bg_rect_->SetFillBrush(GetCurrentBrush()); }),
			Tween::FadeIn(150_msec)
		});
		bg_rect_->StopAllActions();
		bg_rect_->AddAction(action);
	}
}

BrushPtr Background::GetShadowBrush()
{
	// »ÒÉ«ÃÉ²ã»­Ë¢
	Size size = this->GetSize();
	RadialGradientStyle style = RadialGradientStyle(
		size / 2,
		Vec2{},
		Vec2{ size.x * 0.7f, size.y * 0.6f },
		{
			GradientStop(0.8f, Color::Rgba(Color::Black, 0.0f)),
			GradientStop(1.0f, Color::Rgba(Color::Black, 0.3f)),
		}
	);
	return Brush::Create(style);
}

BrushPtr Background::GetCurrentBrush()
{
	switch (mode_)
	{
	case Background::Mode::Blue:
		return GetBlueBrush();
	case Background::Mode::Purple:
		return GetPurpleBrush();
	case Background::Mode::Gold:
		return GetGoldBrush();
	default:
		break;
	}
	return BrushPtr();
}

BrushPtr Background::GetBlueBrush()
{
	float height = this->GetHeight();
	LinearGradientStyle style = LinearGradientStyle(
		Point{ 0, 0 },
		Point{ 0, height },
		{
			GradientStop(0, Color::Rgb(8, 39, 110)),
			GradientStop(1, Color::Rgb(6, 37, 38)),
		}
	);
	BrushPtr brush = Brush::Create(style);
	return brush;
}

BrushPtr Background::GetPurpleBrush()
{
	float height = this->GetHeight();
	LinearGradientStyle style = LinearGradientStyle(
		Point{ 0, 0 },
		Point{ 0, height },
		{
			GradientStop(0, Color::Rgb(118, 40, 78)),
			GradientStop(1, Color::Rgb(45, 31, 66)),
		}
	);
	BrushPtr brush = Brush::Create(style);
	return brush;
}

BrushPtr Background::GetGoldBrush()
{
	float height = this->GetHeight();
	LinearGradientStyle style = LinearGradientStyle(
		Point{ 0, 0 },
		Point{ 0, height },
		{
			GradientStop(0, Color::Rgb(7, 35, 82)),
			GradientStop(1, Color::Rgb(55, 26, 19)),
		}
	);
	BrushPtr brush = Brush::Create(style);
	return brush;
}
