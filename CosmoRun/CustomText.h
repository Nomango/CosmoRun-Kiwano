#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(CustomText);

class CustomText : public TextActor
{
public:
	CustomText(const String& text, float size, bool bold);

	void SetText(const String& text);

	FontPtr GetFont(float size, bool bold) const;

private:
	TextActorPtr shadow_;
};

