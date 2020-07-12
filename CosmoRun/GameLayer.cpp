#include "GameLayer.h"

#define MAX_CUBE_NUMBER 12
#define PREPARE_CUBE_NUMBER 2

std::map<FaceType, String> type_map = {
		{ FaceType::Top, "top   " },
		{ FaceType::Left, "left  " },
		{ FaceType::Right, "right " },
};

std::map<Direction, String> d_map = {
	{ Direction::Up, "up" },
	{ Direction::Down, "down" },
	{ Direction::LeftDown, "left down" },
	{ Direction::LeftUp, "left up" },
	{ Direction::RightDown, "right down" },
	{ Direction::RightUp, "right up" },
};

GameLayer::GameLayer(ColorEnum color, Size size)
	: color_(color)
{
	SetSize(size);
}

void GameLayer::StartGame()
{
	InitCubes(MAX_CUBE_NUMBER);
}

void GameLayer::Restart()
{
	// 调试用
	InitCubes(MAX_CUBE_NUMBER);
}

void GameLayer::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		cube_map_.SetColor(color);
	}
}

void GameLayer::InitCubes(int length)
{
	KGE_LOG();
	KGE_LOG();

	cube_faces_.clear();
	cube_map_.Clear();
	this->RemoveAllChildren();

	CircleActorPtr c = CircleActor::Create(3);
	c->SetFillColor(Color::Red);
	this->AddChild(c, 1);

	// 创建第一个方块
	std::vector<Direction> choices = { Direction::LeftUp, Direction::LeftDown, Direction::RightUp, Direction::RightDown };
	int choice = math::Random(0, int(choices.size() - 1));

	AppendCubeFace({ FaceType::Top, choices[choice] });

	// 创建几个相同类型的方块，让玩家在刚开始游戏时适应游戏速度
	for (int i = 0; i < PREPARE_CUBE_NUMBER; i++)
	{
		AppendCubeFace({ FaceType::Top, choices[choice] });
	}

	// 随机生成后面的方块
	for (int i = 0; i < length - PREPARE_CUBE_NUMBER; i++)
	{
		// 创建随机方块
		AddRandomFace();
	}

	KGE_LOG();
	KGE_LOG();
}

void GameLayer::AddRandomFace()
{
	try
	{
		// 获取随机的方块类型
		auto desc = GetRandomChoice();

		// 创建新方块
		auto face = AppendCubeFace(desc);
	}
	catch (Exception&)
	{
		// 获取随机方块失败，需要删掉之前的方块重新生成
		if (cube_faces_.size() >= 3 && cube_faces_.size() <= 5)
		{
			RemoveHeadFace();

			AddRandomFace();
			AddRandomFace();
		}
		else
		{
			// 方块数量较多时，可能存在圈性路径，需要多删几个
			RemoveHeadFace();
			RemoveHeadFace();

			AddRandomFace();
			AddRandomFace();
			AddRandomFace();
		}
	}
}

void GameLayer::RemoveHeadFace()
{
	KGE_LOG("-remove- ", type_map[cube_faces_.back()->GetType()], d_map[cube_faces_.back()->GetDirection()]);

	cube_faces_.back()->RemoveSelf();
	//auto head = cube_faces_.back();
	//head->GetCube()->RemoveFace(head);
	cube_faces_.pop_back();

	if (cube_faces_.empty())
		KGE_LOG("??");
}

CubeDesc GameLayer::GetRandomChoice()
{
	auto choices = GetRandomChoices();
	if (choices.empty())
	{
		throw Exception("choices is empty");
	}

	// 随机选择一种可能
	int choice = math::Random(0, int(choices.size() - 1));

	// 不允许视觉上相邻的方块
	auto pos = GetNewCubePos(choices[choice]);
	auto head = cube_faces_.back();

	for (auto face : cube_faces_)
	{
		if (face == head)
			continue;

		CubePos pos2 = face->GetCube()->GetPos();
		CubePos offset = { pos[0] - pos2[0], pos[1] - pos2[1], pos[2] - pos2[2] };

		if (face->GetType() == choices[choice].type)
		{
			if (offset == CubePos{ 0, 0, 0 }
				|| offset == CubePos{ 1, 0, 0 }
				|| offset == CubePos{ -1, 0, 0 }
				|| offset == CubePos{ 0, 1, 0 }
				|| offset == CubePos{ 0, -1, 0 }
				|| offset == CubePos{ 0, 0, 1 }
				|| offset == CubePos{ 0, 0, -1 })
			{
				throw Exception("choice is collided");
			}
		}
	}
	return choices[choice];
}

std::vector<CubeDesc> GameLayer::GetRandomChoices()
{
	CubeFace* head = cube_faces_.back();

	// 找出下一个方块的可能类型和方向
	std::vector<CubeDesc> choices;
	switch (head->GetType())
	{
	case FaceType::Top:

		switch (head->GetDirection())
		{
		case Direction::LeftUp:
			choices.assign({
				{ FaceType::Top, Direction::LeftUp },
				{ FaceType::Top, Direction::LeftDown },
				{ FaceType::Top, Direction::RightUp },
				{ FaceType::Right, Direction::Up },
				{ FaceType::Right, Direction::RightUp },
				{ FaceType::Right, Direction::LeftDown },
				});
			break;
		case Direction::LeftDown:
			choices.assign({
				{ FaceType::Top, Direction::LeftUp },
				{ FaceType::Top, Direction::LeftDown },
				{ FaceType::Top, Direction::RightDown },
				{ FaceType::Left, Direction::Down },
				{ FaceType::Left, Direction::LeftUp },
				{ FaceType::Left, Direction::RightDown },
				});
			break;
		case Direction::RightUp:
			choices.assign({
				{ FaceType::Top, Direction::LeftUp },
				{ FaceType::Top, Direction::RightUp },
				{ FaceType::Top, Direction::RightDown },
				{ FaceType::Left, Direction::Up },
				{ FaceType::Left, Direction::LeftUp },
				{ FaceType::Left, Direction::RightDown },
				});
			break;
		case Direction::RightDown:
			choices.assign({
				{ FaceType::Top, Direction::LeftDown },
				{ FaceType::Top, Direction::RightUp },
				{ FaceType::Top, Direction::RightDown },
				{ FaceType::Right, Direction::Down },
				{ FaceType::Right, Direction::RightUp },
				{ FaceType::Right, Direction::LeftDown },
				});
			break;
		}
		break;

	case FaceType::Left:

		switch (head->GetDirection())
		{
		case Direction::Up:
			choices.assign({
				{ FaceType::Left, Direction::Up },
				{ FaceType::Left, Direction::LeftUp },
				{ FaceType::Left, Direction::RightDown },
				{ FaceType::Top, Direction::RightUp },
				{ FaceType::Top, Direction::RightDown },
				{ FaceType::Top, Direction::LeftUp },
				});
			break;
		case Direction::Down:
			choices.assign({
				{ FaceType::Left, Direction::Down },
				{ FaceType::Left, Direction::LeftUp },
				{ FaceType::Left, Direction::RightDown },
				{ FaceType::Top, Direction::RightDown },
				{ FaceType::Top, Direction::LeftUp },
				{ FaceType::Top, Direction::LeftDown },
				});
			break;
		case Direction::LeftUp:
			choices.assign({
				{ FaceType::Left, Direction::LeftUp },
				{ FaceType::Left, Direction::Up },
				{ FaceType::Left, Direction::Down },
				{ FaceType::Right, Direction::LeftDown },
				{ FaceType::Right, Direction::Up },
				{ FaceType::Right, Direction::Down },
				});
			break;
		case Direction::RightDown:
			choices.assign({
				{ FaceType::Left, Direction::RightDown },
				{ FaceType::Left, Direction::Up },
				{ FaceType::Left, Direction::Down },
				// 应该不存在下面的情况，注释掉
				/*{ FaceType::Right, Direction::RightUp },
				{ FaceType::Right, Direction::Up },
				{ FaceType::Right, Direction::Down },*/
				});
			break;
		}
		break;

	case FaceType::Right:

		switch (head->GetDirection())
		{
		case Direction::Up:
			choices.assign({
				{ FaceType::Right, Direction::Up },
				{ FaceType::Right, Direction::LeftDown },
				{ FaceType::Right, Direction::RightUp },
				{ FaceType::Top, Direction::LeftUp },
				{ FaceType::Top, Direction::LeftDown },
				{ FaceType::Top, Direction::RightUp },
				});
			break;
		case Direction::Down:
			choices.assign({
				{ FaceType::Right, Direction::Down },
				{ FaceType::Right, Direction::LeftDown },
				{ FaceType::Right, Direction::RightUp },
				{ FaceType::Top, Direction::LeftDown },
				{ FaceType::Top, Direction::RightDown },
				{ FaceType::Top, Direction::RightUp },
				});
			break;
		case Direction::RightUp:
			choices.assign({
				{ FaceType::Left, Direction::RightDown },
				{ FaceType::Left, Direction::Up },
				{ FaceType::Left, Direction::Down },
				{ FaceType::Right, Direction::RightUp },
				{ FaceType::Right, Direction::Up },
				{ FaceType::Right, Direction::Down },
				});
			break;
		case Direction::LeftDown:
			choices.assign({
				{ FaceType::Right, Direction::LeftDown },
				{ FaceType::Right, Direction::Up },
				{ FaceType::Right, Direction::Down },
				// 应该不存在下面的情况，注释掉
				/*{ FaceType::Left, Direction::LeftUp },
				{ FaceType::Left, Direction::Up },
				{ FaceType::Left, Direction::Down },*/
				});
			break;
		}
		break;
	}

	// 筛选掉不合理的选择
	FilterChoices(choices);
	// KGE_LOG("choices", choices.size());
	return choices;
}

void GameLayer::FilterChoices(std::vector<CubeDesc>& choices)
{
	std::vector<CubeDesc> to_be_removed;
	CubeFace* head = cube_faces_.back();

	// 需要去除几种视觉上受影响的情况
	// 1. 头方块已经被占用了两个面，不允许再占用最后一个面
	bool must_new_cube = (head->GetCube()->GetFacesCount() == 2);
	switch (head->GetType())
	{
	case FaceType::Top:
		switch (head->GetDirection())
		{
		case Direction::LeftDown:
			if (must_new_cube)
			{
				to_be_removed = {
					{ FaceType::Left, Direction::Down },
					{ FaceType::Left, Direction::LeftUp },
					{ FaceType::Left, Direction::RightDown },
				};
			 }
			break;
		case Direction::RightDown:
			if (must_new_cube)
			{
				to_be_removed = {
					{ FaceType::Right, Direction::Down },
					{ FaceType::Right, Direction::RightUp },
					{ FaceType::Right, Direction::LeftDown },
				};
			}
			break;
		}
		break;

	case FaceType::Left:
		switch (head->GetDirection())
		{
		case Direction::Up:
			if (must_new_cube)
			{
				to_be_removed = {
					{ FaceType::Top, Direction::RightUp },
					{ FaceType::Top, Direction::RightDown },
					{ FaceType::Top, Direction::LeftUp },
				};
			}
			break;
		}
		break;

	case FaceType::Right:
		switch (head->GetDirection())
		{
		case Direction::Up:
			if (must_new_cube)
			{
				to_be_removed = {
					{ FaceType::Top, Direction::LeftUp },
					{ FaceType::Top, Direction::LeftDown },
					{ FaceType::Top, Direction::RightUp },
				};
			}
			break;
		}
		break;
	}

	// 2. 不允许与其他方块面占用同一方块
	for (auto iter = choices.begin(); iter != choices.end();)
	{
		auto pos = GetNewCubePos(*iter);

		if (auto cube = cube_map_.GetCubeFromMap(pos))
		{
			if (head->GetCube() != cube)
			{
				iter = choices.erase(iter);
				continue;
			}
		}
		++iter;
	}

	// 移除不允许的情况
	if (!to_be_removed.empty())
	{
		auto need_to_remove = [&](const CubeDesc& desc)
		{
			auto iter = std::find(to_be_removed.begin(), to_be_removed.end(), desc);
			return iter != to_be_removed.end();
		};

		choices.erase(
			std::remove_if(choices.begin(), choices.end(), need_to_remove),
			choices.end()
		);
	}

	for (auto iter = choices.begin(); iter != choices.end();)
	{
		auto pos = GetNewCubePos(*iter);

		bool need_remove = false;
		for (auto face : cube_faces_)
		{
			if (face == head)
				continue;

			if (face->IsCollidedWith(pos, *iter))
			{
				need_remove = true;
				break;
			}
		}

		if (need_remove)
			iter = choices.erase(iter);
		else
			++iter;
	}
}

CubePos GameLayer::GetNewCubePos(CubeDesc desc)
{
	if (cube_faces_.empty())
		return CubePos{ 0, 0, 0 };

	CubeFace* head = cube_faces_.back();

	// 计算相对位置
	CubePos offset = { 0 };
	switch (head->GetType())
	{
	case FaceType::Top:

		switch (head->GetDirection())
		{
		case Direction::LeftUp:
			if (desc.type == FaceType::Top)
				offset = { -1, 0, 0 };
			else
				offset = { -1, 0, 1 };
			break;
		case Direction::LeftDown:
			if (desc.type == FaceType::Top)
				offset = { 0, 1, 0 };
			else
				offset = { 0, 0, 0 };
			break;
		case Direction::RightUp:
			if (desc.type == FaceType::Top)
				offset = { 0, -1, 0 };
			else
				offset = { 0, -1, 1 };
			break;
		case Direction::RightDown:
			if (desc.type == FaceType::Top)
				offset = { 1, 0, 0 };
			else
				offset = { 0, 0, 0 };
			break;
		}
		break;

	case FaceType::Left:

		switch (head->GetDirection())
		{
		case Direction::Up:
			if (desc.type == FaceType::Top)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 0, 1 };
			break;
		case Direction::Down:
			if (desc.type == FaceType::Top)
				offset = { 0, 1, -1 };
			else
				offset = { 0, 0, -1 };
			break;
		case Direction::LeftUp:
			if (desc.type == FaceType::Left)
				offset = { -1, 0, 0 };
			else
				offset = { -1, 1, 0 };
			break;
		case Direction::RightDown:
			if (desc.type == FaceType::Left)
				offset = { 1, 0, 0 };
			else
				offset = { 0, 0, 0 };
			break;
		}
		break;

	case FaceType::Right:

		switch (head->GetDirection())
		{
		case Direction::Up:
			if (desc.type == FaceType::Top)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 0, 1 };
			break;
		case Direction::Down:
			if (desc.type == FaceType::Top)
				offset = { 1, 0, -1 };
			else
				offset = { 0, 0, -1 };
			break;
		case Direction::RightUp:
			if (desc.type == FaceType::Left)
				offset = { 1, -1, 0 };
			else
				offset = { 0, -1, 0 };
			break;
		case Direction::LeftDown:
			if (desc.type == FaceType::Left)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 1, 0 };
			break;
		}
		break;
	}

	// 计算新方块的坐标
	auto pos = head->GetCube()->GetPos();
	for (int i = 0; i < 3; i++)
	{
		pos[i] += offset[i];
	}
	return pos;
}

CubeFace* GameLayer::AppendCubeFace(CubeDesc desc)
{
	// 获取新方块位置
	auto pos = GetNewCubePos(desc);
	CubePtr cube = cube_map_.GetCubeFromMap(pos);
	if (!cube)
	{
		float side_length = GetWidth() * 0.08f;
		cube = cube_map_.CreateCube(pos, side_length);
		this->AddChild(cube);
	}

	KGE_LOG(type_map[desc.type], d_map[desc.direction]);

	auto face = cube->AddFace(desc);

	if (!cube_faces_.empty())
	{
		CubeFace* head = cube_faces_.back();
		head->SetNext(face);
	}
	cube_faces_.push_back(face);
	return face;
}
