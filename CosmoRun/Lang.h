#pragma once
#include "Common.h"

class Lang
{
public:
	enum class Type
	{
		EN,
		CN,
	};

	static String Get(const String& section, const String& key);

	static void Switch(Type type);

	static Type Current();
};
