#pragma once
#include "Common.h"
#include "CubeFace.h"

KGE_DECLARE_SMART_PTR(Cube);

class Cube :
    public Actor
{
public:
    Cube(const CubePos& pos, float side_length);

    const CubePos& GetPos() const;

    int GetFacesCount() const;

    std::vector<CubeFace*> GetFaces() const;

    CubeFace* GetFace(FaceType type) const;

    CubeFace* AddFace(FaceDesc desc);

    void SetColor(ColorEnum color);

    void RemoveFace(Actor* face);

private:
    float side_length_;
    ColorEnum color_;
    CubePos pos_;
    std::vector<CubeFace*> faces_;
};

class CubeMap
{
public:
    CubePtr CreateCube(const CubePos& pos, float side_length);

    Cube* GetCubeFromMap(const CubePos& pos);

    void RemoveCubeInMap(Cube* cube);

    void RemoveCubeFaceInMap(CubeFace* face);

    bool IsCollidedWith(const CubePos& pos, FaceDesc desc, CubeFace* head);

    void SetColor(ColorEnum color);

    void Clear();

private:
    std::map<int, Cube*> cube_map_;
};
