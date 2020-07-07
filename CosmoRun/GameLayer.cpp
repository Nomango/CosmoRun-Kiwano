#include "GameLayer.h"

#define DEFAULT_CUBES_DEPTH 10

GameLayer::GameLayer(ColorEnum color, Size size)
{
	color_ = color;
	SetSize(size);

	BuildCubes(CUBE_QUEUE_LENGTH);
}

void GameLayer::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		for (auto cube : cubes_)
		{
			cube->SetColor(color);
		}
	}
}

void GameLayer::StartGame()
{
}

void GameLayer::BuildCubes(int length)
{
	cubes_.clear();
	this->RemoveAllChildren();

	int x = 0, y = 0, z = 0;

	float side_length = GetWidth() * 0.08f;
	for (int i = 0; i < 3; i++)
	{
		y--;
		CubePtr cube = new Cube(x, y, z, side_length);
		cube->AddFace(CubeFace::Type::Top, Direction::RightUp);
		this->AddCube(cube);
	}

	for (int i = 0; i < 2; i++)
	{
		x--;
		CubePtr cube = new Cube(x, y, z, side_length);
		cube->AddFace(CubeFace::Type::Top, Direction::LeftUp);
		this->AddCube(cube);
	}

	for (int i = 0; i < 4; i++)
	{
		y++;
		CubePtr cube = new Cube(x, y, z, side_length);
		cube->AddFace(CubeFace::Type::Top, Direction::LeftDown);
		this->AddCube(cube);
	}

	for (int i = 0; i < 2; i++)
	{
		x++;
		CubePtr cube = new Cube(x, y, z, side_length);
		cube->AddFace(CubeFace::Type::Top, Direction::RightDown);
		this->AddCube(cube);
	}

	for (int i = 0; i < 2; i++)
	{
		CubePtr cube = new Cube(x, y, z, side_length);
		cube->AddFace(CubeFace::Type::Left, Direction::Down);
		cube->AddFace(CubeFace::Type::Right, Direction::Down);
		this->AddCube(cube);

		z--;
	}
}

void GameLayer::AddCube(CubePtr cube)
{
	cubes_.push_back(cube.Get());
	this->AddChild(cube);
}
