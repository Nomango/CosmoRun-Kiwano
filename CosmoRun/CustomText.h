#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(CustomText);

class CustomText : public TextActor, public SizeSensor
{
public:
	CustomText(const String& text, float size);

	void SetText(const String& text);

	FontPtr GetFont(float size) const;

	void OnUnitChanged(float unit) override;

private:
	float font_size_;
	TextActorPtr shadow_;
};

