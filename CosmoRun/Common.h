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
	Up,
	RightUp,
	RightDown,
	Down,
	LeftDown,
	LeftUp
};

// ����������
struct FaceDesc
{
	FaceType type;
	Direction direction;

	inline bool operator==(const FaceDesc& rhs) const
	{
		return type == rhs.type && direction == rhs.direction;
	}

	inline bool operator<(const FaceDesc& rhs) const
	{
		if (type == rhs.type)
			return direction < rhs.direction;
		return type < rhs.type;
	}

	friend std::ostream& operator<< (std::ostream& out, const FaceDesc& desc);
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

// ����
struct Config
{
	static float Unit();

	static float NormalizeUnit();

	static void SetWindowSize(Size size);

	static ColorEnum Color();

	static void Color(ColorEnum color);

	static ColorEnum RandomColor();
};

// ���ڴ�С�仯������
class SizeSensor
{
public:
	SizeSensor();

	virtual ~SizeSensor();

	virtual void OnUnitChanged(float unit) = 0;

	static void Notify(float unit);

private:
	static Set<SizeSensor*> sensors_;
};
