#include "CustomText.h"

CustomText::CustomText(const String& text, float size, bool bold)
{
	FontPtr font = new Font("Gill Sans MT", size, bold ? FontWeight::Bold : FontWeight::Normal);

	this->SetFillColor(Color::White);
	this->SetAnchor(0.5f, 0.5f);
	this->SetFont(font);
	this->SetText(text);

	StrokeStylePtr stroke = new StrokeStyle(4);
	this->SetOutlineStrokeStyle(stroke);
	this->SetOutlineColor(Color::Rgba(0, 0, 0, 0.3f));
}
