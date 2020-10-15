#pragma once
#include "Common.h"
#include "CubeFace.h"

// ·½¿é×ø±ê
typedef std::array<int, 3> CubePos;

KGE_DECLARE_SMART_PTR(Cube);

class Cube : public Actor , public SizeSensor
{
public:
    Cube(const CubePos& pos);

    const CubePos& GetPos() const;

    int GetFacesCount() const;

    std::vector<CubeFace*> GetFaces() const;

    CubeFace* GetFace(FaceType type) const;

    CubeFace* AddFace(FaceDesc desc, BrushCreator* brush_creator);

    void RemoveFace(Actor* face);

    void OnUnitChanged(float unit) override;

private:
    CubePos pos_;
    std::vector<CubeFace*> faces_;
};

class CubeMap
{
public:
    CubePtr CreateCube(const CubePos& pos);

    Cube* GetCubeFromMap(const CubePos& pos);

    void RemoveCubeInMap(Cube* cube);

    void RemoveCubeFaceInMap(CubeFace* face);

    bool IsCollidedWith(const CubePos& pos, FaceDesc desc, CubeFace* head);

    void Clear();

private:
    std::map<int, Cube*> cube_map_;
};
