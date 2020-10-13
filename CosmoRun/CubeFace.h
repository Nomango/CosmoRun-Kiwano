#pragma once
#include "Common.h"

class Cube;

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

KGE_DECLARE_SMART_PTR(CubeFace);

class CubeFace : public PolygonActor
{
public:
	CubeFace(FaceType type, Direction d);

	~CubeFace();

	ActorPtr GetShadow();

	FaceType GetType() const;

	Direction GetDirection() const;

	FaceDesc GetDesc() const;

	bool IsIn(const std::initializer_list<FaceType>& list);

	void SetColor(ColorEnum color);

	void Show();

	CubeFace* GetNext() const;

	void SetNext(CubeFace* face);

	Cube* GetCube() const;

	void SetCube(Cube* cube);

	void ResetBrush(ColorEnum color);

private:
	void CreateVertices();

	BrushPtr GetFillBrush(ColorEnum color);

	BrushPtr GetTopFillBrush(Color light, Color dark);

	BrushPtr GetStrokeBrush(ColorEnum color);

	BrushPtr GetStrokeBrush(Color light, Color dark);

private:
	FaceDesc desc_;
	CubeFace* next_ = nullptr;
	Cube* cube_ = nullptr;
	ActorPtr shadow_;
	ResourceCache brush_cache_;
};
