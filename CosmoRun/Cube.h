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

    int GetFacesCount() const;

    CubeFace* GetFace(CubeFace::Type type) const;

    CubeFace* AddFace(CubeFace::Type type, Direction d);

    void SetColor(ColorEnum color);

private:
    float side_length_;
    ColorEnum color_;
    std::vector<CubeFace*> faces_;
    std::array<int, 3> pos_;
};

class CubeMap
{
public:
    CubePtr CreateCube(int x, int y, int z, float side_length);

    Cube* GetCubeFromMap(int x, int y, int z);

    void RemoveCubeInMap(int x, int y, int z);

    void SetColor(ColorEnum color);

    void Clear();

private:
    std::map<int, Cube*> cube_map_;
};
