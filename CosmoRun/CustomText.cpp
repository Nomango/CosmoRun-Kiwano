#include "CustomText.h"
#include "Lang.h"

CustomText::CustomText(const String& text, float size)
	: font_size_(size)
{
	this->SetFillColor(Color::White);
	this->SetAnchor(0.5f, 0.5f);
	this->SetText(text);

	shadow_ = new TextActor;
	shadow_->SetFillColor(Color(Color::Black, 0.2f));
	shadow_->SetText(text);
	this->AddChild(shadow_, -1);

	OnUnitChanged(Config::Unit());
}

void CustomText::SetText(const String& text)
{
	TextActor::SetText(text);
	if (shadow_)
	{
		shadow_->SetText(text);
	}
}

FontPtr CustomText::GetFont(float size) const
{
	String font_name;
	switch (Lang::Current())
	{
	case Lang::Type::EN:
		font_name = "Gill Sans MT";
		break;
	case Lang::Type::CN:
		font_name = "Microsoft YaHei UI Light";
		break;
	}
	FontPtr font = new Font(font_name, size, FontWeight::Bold, FontPosture::Normal, FontStretch::Condensed);
	return font;
}

void CustomText::OnUnitChanged(float)
{
	float unit = Config::NormalizeUnit();

	FontPtr font = GetFont(font_size_ * unit);
	this->SetFont(font);
	shadow_->SetFont(font);
	shadow_->MoveTo(font_size_ / 20 * unit, font_size_ / 20 * unit);
}
