#pragma once
#include "Common.h"
#include "CubeFace.h"

KGE_DECLARE_SMART_PTR(Cube);

typedef std::array<int, 3> CubePos;

struct CubeDesc
{
    CubeFace::Type type;
    Direction direction;

    bool operator==(const CubeDesc& rhs) const
    {
        return type == rhs.type && direction == rhs.direction;
    }
};

class Cube :
    public Actor
{
public:
    Cube(const CubePos& pos, float side_length);

    const CubePos& GetPos() const;

    int GetFacesCount() const;

    CubeFace* GetFace(CubeFace::Type type) const;

    CubeFace* AddFace(CubeDesc desc);

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
    CubePtr CreateCube(const CubePos& pos, float side_length);

    Cube* GetCubeFromMap(const CubePos& pos);

    void RemoveCubeInMap(const CubePos& pos);

    void SetColor(ColorEnum color);

    void Clear();

private:
    std::map<int, Cube*> cube_map_;
};
