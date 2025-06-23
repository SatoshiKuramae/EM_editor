//===============================================================================
//
//grid.h
//
//Author Kuramaesatoshi
//===============================================================================
#pragma once
#include "pch.h"


class CGrid {

public:
    CGrid();
    ~CGrid();

    void Init(LPDIRECT3DDEVICE9 device);
    void Draw(const D3DXMATRIX& viewProj);
    void Release();

private:
    ID3DXLine* m_pLine;
    std::vector<D3DXVECTOR3> m_vertices;
    int m_gridSize;
    float m_spacing;
};
