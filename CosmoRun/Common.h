#pragma once
#include <kiwano/kiwano.h>

using namespace kiwano;

// 颜色模式
enum class ColorEnum
{
	Blue,		// 蓝色
	Purple,		// 紫色
	Gold,		// 金色
};

// 方块坐标
typedef std::array<int, 3> CubePos;

// 方块面类型
enum class FaceType
{
	Top,
	Left,
	Right,
};

// 方块面方向
enum class Direction
{
	LeftUp,
	LeftDown,
	RightUp,
	RightDown,
	Up,
	Down,
};

// 方块描述
struct CubeDesc
{
	FaceType type;
	Direction direction;

	inline bool IsIn(const std::initializer_list<CubeDesc>& list)
	{
		return std::find(list.begin(), list.end(), *this) != list.end();
	}

	inline bool operator==(const CubeDesc& rhs) const
	{
		return type == rhs.type && direction == rhs.direction;
	}
};
