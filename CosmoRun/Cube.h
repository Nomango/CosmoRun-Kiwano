#pragma once
#include "Common.h"
#include "CubeFace.h"

KGE_DECLARE_SMART_PTR(Cube);

class Cube :
    public Actor
{
public:
    Cube(int x, int y, int z, float side_length);

    const std::array<int, 3>& GetPos() const;

    CubeFace* GetFace(CubeFace::Type type) const;

    void AddFace(CubeFace::Type type, Direction d);

    void SetColor(ColorEnum color);

private:
    float side_length_;
    ColorEnum color_;
    std::array<int, 3> pos_;
    std::list<CubeFace*> faces_;
};
