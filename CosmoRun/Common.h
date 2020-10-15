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
	Up,
	RightUp,
	RightDown,
	Down,
	LeftDown,
	LeftUp
};

// 方块面描述
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

// 方块面枚举
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

// 配置
struct Config
{
	static float Unit();

	static float NormalizeUnit();

	static void SetWindowSize(Size size);

	static ColorEnum Color();

	static void Color(ColorEnum color);

	static ColorEnum RandomColor();
};

// 窗口大小变化接收器
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
