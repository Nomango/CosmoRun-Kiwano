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
