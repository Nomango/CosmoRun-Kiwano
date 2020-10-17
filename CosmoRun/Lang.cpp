#include "Lang.h"
#include "resource.h"

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
	Resource lang_res;
	switch (type)
	{
	case Lang::Type::EN:
		lang_res = Resource(IDR_INI_EN, "INI");
		break;
	case Lang::Type::CN:
		lang_res = Resource(IDR_INI_EN, "INI");  // TODO: 支持中文
		break;
	}

	BinaryData data = lang_res.GetData();
	if (!data.IsValid())
		throw RuntimeError("Language configuration is invalid!");

	StringStream ss;
	ss.write((char*)data.buffer, (std::streamsize)data.size);
	if (!global_lang_ini.Load(ss))
		throw RuntimeError("Language configuration load failed!");

	global_current_lang = type;
}

Lang::Type Lang::Current()
{
	return global_current_lang;
}
