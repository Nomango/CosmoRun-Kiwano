#include "Buttons.h"
#include "CustomText.h"
#include "Lang.h"

HexagonButton::HexagonButton(float width)
	: status_(Status::Normal)
{
	float unit = Config::Unit();

	SetAnchor(0.5f, 0.5f);
	SetSize(Size(unit * (1 + width), unit));

	SetVertices(GetHexagonVertices(width, unit, GetSize() / 2));

	button_ = new Button(Closure(this, &HexagonButton::OnButtonEvent));
	AddComponent(button_);
}

void HexagonButton::Hide()
{
	StartAnimation(animation::FadeOut(150_msec));
	Disable();
}

void HexagonButton::Show()
{
	StartAnimation(animation::FadeIn(300_msec));
	Enable();
}

void HexagonButton::Enable()
{
	button_->SetEnabled(true);
}

void HexagonButton::Disable()
{
	button_->SetEnabled(false);
}

void HexagonButton::SetCallback(Function<void()> click)
{
	click_ = click;
}

void HexagonButton::OnButtonEvent(Button* btn, Button::Event evt)
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
	OnStatusChanged(status_);
}

Vector<Point> HexagonButton::GetHexagonVertices(float width, float side, Point offset)
{
	float w = side * width;
	float h = side;
	Vector<Point> vertices = {
		Point(-w / 2 - h / 2, 0),
		Point(-w / 2, -h / 2),
		Point(w / 2, -h / 2),
		Point(w / 2 + h / 2, 0),
		Point(w / 2, h / 2),
		Point(-w / 2, h / 2),
	};
	for (auto& v : vertices)
		v += offset;
	return vertices;
}

PlayButton::PlayButton()
	: HexagonButton(2.0f)
{
	// ÎÄ×Ö
	CustomTextPtr text = new CustomText(Lang::Get("main", "play"), 48, true);
	text->SetAnchor(0.5f, 0.5f);
	text->SetPosition(this->GetSize() / 2);
	AddChild(text);

	// Ö´ÐÐ¶¯»­
	auto action = animation::ScaleBy(1_sec, Vec2(0.15f, 0.15f));
	auto group = animation::Group({ action, action.Reverse() }).Loops(-1);
	StartAnimation(group);

	OnStatusChanged(Status::Normal);
}

void PlayButton::OnStatusChanged(Status status)
{
	switch (status)
	{
	case Status::Normal:
		this->SetFillColor(Color::Rgba(0, 171, 255, 0.5f));
		break;
	case Status::Hover:
		this->SetFillColor(Color::Rgba(0, 141, 225, 0.5f));
		break;
	case Status::Pressed:
		this->SetFillColor(Color::Rgba(0, 80, 175, 0.5f));
		break;
	}
}

SpecialHexButton::SpecialHexButton()
	: HexagonButton(1.0f)
{
	float size = GetHeight();

	inner_polygon_ = new PolygonActor;
	inner_polygon_->SetVertices(GetHexagonVertices(1.0f, size * 0.85f, GetSize() / 2));
	AddChild(inner_polygon_);

	OnStatusChanged(Status::Normal);
}

void SpecialHexButton::OnStatusChanged(Status status)
{
	Color inner_color, outter_color;
	switch (status)
	{
	case Status::Normal:
		inner_color = Color::Rgb(0, 136, 215);
		outter_color = Color::Rgb(0, 161, 255);
		break;
	case Status::Hover:
		inner_color = Color::Rgb(0, 146, 225);
		outter_color = Color::Rgb(0, 181, 255);
		break;
	case Status::Pressed:
		inner_color = Color::Rgb(0, 116, 195);
		outter_color = Color::Rgb(0, 141, 235);
		break;
	}
	inner_polygon_->SetFillColor(inner_color);
	this->SetFillColor(outter_color);
}

TryAgainButton::TryAgainButton()
	: SpecialHexButton()
{
}
