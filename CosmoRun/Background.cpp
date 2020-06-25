#include "Background.h"

BackgroundPtr Background::Create(Size size)
{
	BackgroundPtr ptr = memory::New<Background>();
	if (ptr)
	{
		ptr->Resize(size);
	}
	return ptr;
}

void Background::Resize(Size size)
{
	canvas_ = Canvas::Create(size);
	AddChild(canvas_);

	Repaint();
}

void Background::SetMode(Mode mode)
{
	mode_ = mode;
	Repaint();
}

void Background::Repaint()
{
	auto ctx = canvas_->GetContext2D();

	ctx->BeginDraw();
	{
		BrushPtr brush = GetCurrentBrush();

		// äÖÈ¾±³¾°É«
		ctx->SetCurrentBrush(brush);
		ctx->FillRectangle(canvas_->GetBounds());

		// äÖÈ¾»ÒÉ«ÃÉ²ã
		Size size = canvas_->GetSize();
		RadialGradientStyle style = RadialGradientStyle(
			size / 2,
			Vec2{},
			Vec2{ size.x * 0.7f, size.y * 0.6f },
			{
				GradientStop(0.8f, Color::Rgba(Color::Black, 0.0f)),
				GradientStop(1.0f, Color::Rgba(Color::Black, 0.3f)),
			}
		);
		brush = Brush::Create(style);

		ctx->SetCurrentBrush(brush);
		ctx->FillRectangle(canvas_->GetBounds());
	}
	ctx->EndDraw();
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
	float height = canvas_->GetHeight();
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
	float height = canvas_->GetHeight();
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
	float height = canvas_->GetHeight();
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
