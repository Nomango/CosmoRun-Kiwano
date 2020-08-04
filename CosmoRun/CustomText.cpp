#include "CustomText.h"

CustomText::CustomText(const String& text, float size, bool bold)
{
	uint32_t weight = bold ? FontWeight::Bold : FontWeight::Normal;
	FontPtr font = new Font("Gill Sans MT", size, weight, FontPosture::Normal, FontStretch::Condensed);

	this->SetFillColor(Color::White);
	this->SetAnchor(0.5f, 0.5f);
	this->SetFont(font);
	this->SetText(text);

	StrokeStylePtr stroke = new StrokeStyle(4, CapStyle::Round, LineJoinStyle::Round);
	this->SetOutlineStrokeStyle(stroke);
	this->SetOutlineColor(Color::Rgba(0, 0, 0, 0.3f));
}
