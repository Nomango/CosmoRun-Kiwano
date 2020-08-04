#include "Lang.h"

String Lang::GetText(const String& key1, const String& key2)
{
	String key = key1 + "-" + key2;
	if (!text_map_.count(key))
	{
		throw RuntimeError("Language key \"" + key + "\" not found!");
	}
	return text_map_.at(key);
}

void Lang::SetText(const String& key1, const String& key2, const String& text)
{
	String key = key1 + "-" + key2;
	text_map_.insert(std::make_pair(key, text));
}

void Lang::Switch(const String& lang)
{
	String lang_file = "lang/" + lang + ".ini";
	String fullpath = FileSystem::GetInstance().GetFullPathForFile(lang_file);
	if (fullpath.empty())
		throw RuntimeError("Language configuration not found!");

	ConfigIni ini;
	if (ini.Load(fullpath))
	{
		for (auto section : ini.GetSectionMap())
		{
			for (auto kv : section.second)
			{
				SetText(section.first, kv.first, kv.second);
			}
		}
	}
}
