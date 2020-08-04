#pragma once
#include "Cube.h"

KGE_DECLARE_SMART_PTR(CubeGroup);

class CubeGroup
    : public Actor
{
public:
    CubeGroup(float side_length);

    void InitCubes(int length);

    void AddRandomFace(int depth = 1);

    CubeFace* AppendCubeFace(FaceDesc desc);

    void RemoveFace(Actor* face);

    void RemoveTailFace();

    CubeFace* GetTail() const;

    void SetColor(ColorEnum color);

private:
    void RemoveHeadFace();

    std::vector<FaceDesc> GetRandomChoices();

    CubePos GetNewCubePos(FaceDesc desc);

    void DumpCubes();

private:
    float side_length_;
    CubeFace* tail_;
    CubeMap cube_map_;
    std::list<CubeFace*> hide_faces_;
};
