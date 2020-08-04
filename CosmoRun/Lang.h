#pragma once
#include "Common.h"

class Lang : public Singleton<Lang>
{
	friend Singleton<Lang>;

public:
	static inline String Get(const String& key1, const String& key2)
	{
		return Lang::GetInstance().GetText(key1, key2);
	}

	String GetText(const String& key1, const String& key2);

	void SetText(const String& key1, const String& key2, const String& text);

	void Switch(const String& lang);

private:
	Map<String, String> text_map_;
};
