#include "Grid.h"

//�R���X�g���N�^
CGrid::CGrid()
    : m_pLine(nullptr), m_gridSize(9), m_spacing(50.0f) {}

//�f�X�g���N�^
CGrid::~CGrid() {
    Release();
}

//����������
void CGrid::Init(LPDIRECT3DDEVICE9 device) {
    if (!device) return;

    D3DXCreateLine(device, &m_pLine);

    // XZ���ʂɃO���b�h���\�z
    for (int i = -m_gridSize; i <= m_gridSize; ++i) {
        m_vertices.push_back(D3DXVECTOR3(i * m_spacing, 0, -m_gridSize * m_spacing));
        m_vertices.push_back(D3DXVECTOR3(i * m_spacing, 0, m_gridSize * m_spacing));
    }

    for (int j = -m_gridSize; j <= m_gridSize; ++j) {
        m_vertices.push_back(D3DXVECTOR3(-m_gridSize * m_spacing, 0, j * m_spacing));
        m_vertices.push_back(D3DXVECTOR3(m_gridSize * m_spacing, 0, j * m_spacing));
    }
}

//�`�揈��
void CGrid::Draw(const D3DXMATRIX& viewProj) {
    if (!m_pLine) return;
    
    m_pLine->SetWidth(1.0f);
    m_pLine->Begin();
    m_pLine->DrawTransform(&m_vertices[0], (UINT)m_vertices.size(), &viewProj, D3DCOLOR_XRGB(120, 120, 120));
    m_pLine->End();
}

//���
void CGrid::Release() {
    if (m_pLine) {
        m_pLine->Release();
        m_pLine = nullptr;
    }
}