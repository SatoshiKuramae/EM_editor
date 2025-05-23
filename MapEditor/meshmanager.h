#pragma once
#include "pch.h"
#include <d3dx9.h>

class MeshManager {
public:
    static LPD3DXMESH LoadMesh(const std::string& filename, LPDIRECT3DDEVICE9 pDevice);
    static void ReleaseAll();

private:
    static std::map<std::string, LPD3DXMESH> s_meshes;
};