#include "CustomText.h"
#include "Lang.h"

CustomText::CustomText(const String& text, float size, bool bold)
	: font_size_(size)
	, bold_(bold)
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

FontPtr CustomText::GetFont(float size, bool bold) const
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
	uint32_t weight = bold ? FontWeight::Bold : FontWeight::Normal;
	FontPtr font = new Font(font_name, size, weight, FontPosture::Normal, FontStretch::Condensed);
	return font;
}

void CustomText::OnUnitChanged(float)
{
	float unit = Config::NormalizeUnit();

	FontPtr font = GetFont(font_size_ * unit, bold_);
	this->SetFont(font);
	shadow_->SetFont(font);
	shadow_->MoveTo(0, 5 * unit);
}
