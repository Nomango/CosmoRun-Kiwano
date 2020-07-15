#pragma once
#include "Cube.h"

KGE_DECLARE_SMART_PTR(CubeGroup);

class CubeGroup
    : public Actor
{
public:
    CubeGroup(float side_length);

    void InitCubes(int length);

    void AddRandomFace();

    CubeFace* AppendCubeFace(FaceDesc desc);

    void RemoveFace(Actor* face);

    CubeFace* GetTail() const;

    void SetColor(ColorEnum color);

private:
    void RemoveTailFace();

    void RemoveHeadFace();

    std::vector<FaceDesc> GetRandomChoices();

    CubePos GetNewCubePos(FaceDesc desc);

private:
    float side_length_;
    CubeFace* tail_;
    CubeMap cube_map_;
    std::list<CubeFace*> hide_faces_;
};
