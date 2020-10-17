#include "Common.h"

FaceDesc Face::Top_LeftDown = { FaceType::Top, Direction::LeftDown };
FaceDesc Face::Top_LeftUp = { FaceType::Top, Direction::LeftUp };
FaceDesc Face::Top_RightDown = { FaceType::Top, Direction::RightDown };
FaceDesc Face::Top_RightUp = { FaceType::Top, Direction::RightUp };
FaceDesc Face::Left_Up = { FaceType::Left, Direction::Up };
FaceDesc Face::Left_Down = { FaceType::Left, Direction::Down };
FaceDesc Face::Left_LeftUp = { FaceType::Left, Direction::LeftUp };
FaceDesc Face::Left_RightDown = { FaceType::Left, Direction::RightDown };
FaceDesc Face::Right_Up = { FaceType::Right, Direction::Up };
FaceDesc Face::Right_Down = { FaceType::Right, Direction::Down };
FaceDesc Face::Right_LeftDown = { FaceType::Right, Direction::LeftDown };
FaceDesc Face::Right_RightUp = { FaceType::Right, Direction::RightUp };

std::map<FaceType, String> type_map = {
	{ FaceType::Top, "[top]   " },
	{ FaceType::Left, "[left]  " },
	{ FaceType::Right, "[right] " },
};

std::map<Direction, String> d_map = {
	{ Direction::Up, "up" },
	{ Direction::Down, "down" },
	{ Direction::LeftDown, "left down" },
	{ Direction::LeftUp, "left up" },
	{ Direction::RightDown, "right down" },
	{ Direction::RightUp, "right up" },
};

std::ostream& operator<<(std::ostream& out, const FaceDesc& desc)
{
	out << type_map[desc.type] << d_map[desc.direction];
	return out;
}

namespace
{
	float global_unit_length = 0.0f;
	ColorEnum global_color = ColorEnum(-1);

	String GetConfigFile()
	{
		char temp_path[MAX_PATH];
		if (GetTempPathA(MAX_PATH, temp_path) == 0)
			return {};

		String dir_path = String(temp_path) + "cosmorun-kiwano\\";
		if (!FileSystem::GetInstance().IsFileExists(dir_path))
		{
			if (!CreateDirectoryA(dir_path.c_str(), nullptr))
			{
				KGE_ERROR("Create config file failed! err code:", GetLastError());
			}
		}

		String file_name = dir_path + "config.ini";
		KGE_NOTICE("Config file name:", file_name);
		return file_name;
	}
}

float Config::Unit()
{
	return global_unit_length;
}

float Config::NormalizeUnit()
{
	return global_unit_length / 64;
}

void Config::SetWindowSize(Size size)
{
	global_unit_length = std::min(std::max(size.x * 0.08f, 60.0f), 120.0f);
	KGE_NOTICE("Unit length changed to", global_unit_length);

	SizeSensor::Notify(global_unit_length);
}

ColorEnum Config::Color()
{
	return global_color;
}

void Config::Color(ColorEnum color)
{
	global_color = color;
}

ColorEnum Config::RandomColor()
{
	if (global_color == ColorEnum(-1))
		return ColorEnum(math::Random(0, 2));
	return ColorEnum((int(global_color) + math::Random(1, 2)) % 3);
}

int Config::ReadBestScore()
{
	String file = GetConfigFile();
	std::ifstream ifs(file);
	if (!ifs.is_open())
		return 0;

	ConfigIni ini;
	if (!ini.Load(ifs))
		return 0;
	if (ini.HasKey("user_data", "best_score"))
		return ini.GetInt("user_data", "best_score");
	return 0;
}

void Config::SaveBestScore(int best_score)
{
	String file = GetConfigFile();
	ConfigIni ini;

	std::ifstream ifs(file);
	if (ifs.is_open())
	{
		ini.Load(ifs);
	}

	std::ofstream ofs(file);
	if (ofs.is_open())
	{
		ini.SetInt("user_data", "best_score", best_score);
		ini.Save(ofs);
	}
}

Set<SizeSensor*> SizeSensor::sensors_;

SizeSensor::SizeSensor()
{
	sensors_.insert(this);
}

SizeSensor::~SizeSensor()
{
	sensors_.erase(this);
}

void SizeSensor::Notify(float unit)
{
	for (auto sensor : sensors_)
	{
		sensor->OnUnitChanged(unit);
	}
}
