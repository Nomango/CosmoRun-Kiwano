#include "CustomText.h"

CustomText::CustomText(const String& text, float size, bool bold)
{
	uint32_t weight = bold ? FontWeight::Bold : FontWeight::Normal;
	FontPtr font = new Font("Gill Sans MT", size, weight, FontPosture::Normal, FontStretch::Condensed);

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
