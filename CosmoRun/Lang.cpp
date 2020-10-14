#include "Lang.h"

namespace
{
	Lang::Type global_current_lang = Lang::Type::EN;
	ConfigIni global_lang_ini;
}

String Lang::Get(const String& section, const String& key)
{
	if (!global_lang_ini.HasKey(section, key))
		throw RuntimeError("Language key \"" + key + "\" not found!");
	return global_lang_ini.GetString(section, key);
}

void Lang::Switch(Type type)
{
	String lang_file;
	switch (type)
	{
	case Lang::Type::EN:
		lang_file = "en";
		break;
	case Lang::Type::CN:
		lang_file = "zh-cn";
		break;
	}
	lang_file = "lang/" + lang_file + ".ini";
	String fullpath = FileSystem::GetInstance().GetFullPathForFile(lang_file);
	if (fullpath.empty())
		throw RuntimeError("Language configuration not found!");

	if (!global_lang_ini.Load(fullpath))
		throw RuntimeError("Language configuration load failed!");

	global_current_lang = type;
}

Lang::Type Lang::Current()
{
	return global_current_lang;
}
