#include "CustomText.h"
#include "Lang.h"

CustomText::CustomText(const String& text, float size, bool bold)
{
	FontPtr font = GetFont(size, bold);

	this->SetFillColor(Color::White);
	this->SetFont(font);
	this->SetAnchor(0.5f, 0.5f);
	this->SetText(text);

	shadow_ = new TextActor;
	shadow_->SetFillColor(Color(Color::Black, 0.2f));
	shadow_->SetFont(font);
	shadow_->SetText(text);
	shadow_->MoveBy(0, 5);
	this->AddChild(shadow_, -1);
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
	size = size * Config::Unit() / 64;
	uint32_t weight = bold ? FontWeight::Bold : FontWeight::Normal;
	FontPtr font = new Font(font_name, size, weight, FontPosture::Normal, FontStretch::Condensed);
	return font;
}
