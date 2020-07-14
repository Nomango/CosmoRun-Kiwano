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

// ����������
struct FaceDesc
{
	FaceType type;
	Direction direction;

	inline bool IsIn(const std::initializer_list<FaceDesc>& list)
	{
		return std::find(list.begin(), list.end(), *this) != list.end();
	}

	inline bool IsIn(const std::initializer_list<FaceType>& list)
	{
		for (auto type : list)
		{
			if (type == this->type)
			{
				return true;
			}
		}
		return false;
	}

	inline bool operator==(const FaceDesc& rhs) const
	{
		return type == rhs.type && direction == rhs.direction;
	}
};

// ������ö��
struct Face
{
	static FaceDesc Top_LeftDown;
	static FaceDesc Top_LeftUp;
	static FaceDesc Top_RightDown;
	static FaceDesc Top_RightUp;
	static FaceDesc Left_Up;
	static FaceDesc Left_Down;
	static FaceDesc Left_LeftUp;
	static FaceDesc Left_RightDown;
	static FaceDesc Right_Up;
	static FaceDesc Right_Down;
	static FaceDesc Right_LeftDown;
	static FaceDesc Right_RightUp;
};
