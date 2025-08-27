//===============================================================================
//
//meshmanager.cpp
//
//Author:Kuramaesatoshi
// 今は使ってません
//===============================================================================
#include "MeshManager.h"

std::map<std::string, LPD3DXMESH> MeshManager::s_meshes;

LPD3DXMESH MeshManager::LoadMesh(const std::string& filename, LPDIRECT3DDEVICE9 pDevice) {
    if (s_meshes.find(filename) == s_meshes.end()) {
        LPD3DXMESH mesh = nullptr;
        D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, NULL, NULL, NULL, &mesh);
        s_meshes[filename] = mesh;
    }
    return s_meshes[filename];
}

void MeshManager::ReleaseAll() {
    for (auto& pair : s_meshes) {
        if (pair.second) {
            pair.second->Release();
        }
    }
    s_meshes.clear();
}
