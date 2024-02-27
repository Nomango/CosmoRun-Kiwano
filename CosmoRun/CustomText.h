#pragma once
#include "Common.h"

class CustomText : public TextActor, public SizeSensor
{
public:
	CustomText(const String& text, float size);

	void SetText(const String& text);

	RefPtr<Font> GetFont(float size) const;

	void OnUnitChanged(float unit) override;

private:
	float font_size_;
	RefPtr<TextActor> shadow_;
};

