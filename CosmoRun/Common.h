#pragma once
#include <kiwano/kiwano.h>

using namespace kiwano;

// ��ɫģʽ
enum class ColorEnum
{
	Blue,		// ��ɫ
	Purple,		// ��ɫ
	Gold,		// ��ɫ
};

// ��������
typedef std::array<int, 3> CubePos;

// ����������
enum class FaceType
{
	Top,
	Left,
	Right,
};

// �����淽��
enum class Direction
{
	LeftUp,
	LeftDown,
	RightUp,
	RightDown,
	Up,
	Down,
};

// ��������
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
