#pragma once
#include "Common.h"
#include "CubeFace.h"

KGE_DECLARE_SMART_PTR(Cube);

struct CubeDesc
{
    CubeFace::Type type;
    Direction direction;
};

class Cube :
    public Actor
{
public:
    Cube(int x, int y, int z, float side_length);

    const std::array<int, 3>& GetPos() const;

    Cube* GetNext() const;

    void SetNext(Cube* next);

    CubeFace* GetFace() const;

    CubeFace* SetFace(CubeFace::Type type, Direction d);

    void SetColor(ColorEnum color);

private:
    float side_length_;
    ColorEnum color_;
    CubeFace* face_;
    Cube* next_;
    std::array<int, 3> pos_;
};
