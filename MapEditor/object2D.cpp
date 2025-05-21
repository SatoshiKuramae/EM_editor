//============================================================
//
//object2D.cpp
//
//============================================================
#include "object2D.h"
#include "main.h"
#include "object2D.h"
#include "manager.h"
//�R���X�g���N�^
CObject2D::CObject2D(int nPriority) :CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_texcnt = D3DXVECTOR2(0.0f, 0.0f);
	
}

//�f�X�g���N�^
CObject2D::~CObject2D()
{

}

//�������i���_�o�b�t�@�����j
HRESULT CObject2D::Init()
{
	
	//LPDIRECT3DDEVICE9 pDevice;

	////�f�o�C�X�̎擾
	//pDevice = CManager::GetRenderer()->GetDevice();

	////���_�o�b�t�@�̐���
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//VERTEX_2D* pVtx;
	////���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	////���_���W�̐ݒ�
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[0].pos.z = 0.0f;

	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[1].pos.z = 0.0f;

	//pVtx[2].pos.x = m_pos.x - sinf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[2].pos.y = m_pos.y - cosf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[2].pos.z = 0.0f;

	//pVtx[3].pos.x = m_pos.x - sinf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[3].pos.y = m_pos.y - cosf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	//pVtx[3].pos.z = 0.0f;

	//pVtx[0].rhw = 1.0f;
	//pVtx[1].rhw = 1.0f;
	//pVtx[2].rhw = 1.0f;
	//pVtx[3].rhw = 1.0f;


	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_NowTexpos_y));
	//pVtx[3].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), (1.0f / m_NowTexpos_y));

	//pVtx[0].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	//pVtx[1].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	//pVtx[2].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	//pVtx[3].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	////���_�o�b�t�@���A�����b�N����
	//m_pVtxBuff->Unlock();

	return S_OK;
}

//�I��(���_�o�b�t�@�j��)
void CObject2D::Uninit()
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//�X�V����(���_�o�b�t�@�̍X�V)
void CObject2D::Update()
{

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�

	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x - sinf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[2].pos.y = m_pos.y - cosf(m_rot.z + (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x - sinf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[3].pos.y = m_pos.y - cosf(m_rot.z - (D3DX_PI - m_fAnglePolygon)) * m_fLengthPolygon;
	pVtx[3].pos.z = 0.0f;

	
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_NowTexpos_y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), (1.0f / m_NowTexpos_y));

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//�`��i�|���S���`��j
void CObject2D::Draw()
{
	//LPDIRECT3DDEVICE9 pDevice;
	////�f�o�C�X�̓ǂݍ���
	//pDevice = CManager::GetRenderer()->GetDevice();
	////���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	////���_�t�H�[�}�b�g�̐ݒ�
	//pDevice->SetFVF(FVF_VERTEX_2D);
	////�e�N�X�`���̐ݒ�
	//pDevice->SetTexture(0, m_pTexture);
	////�|���S���̕`��
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//Object2D����
CObject2D* CObject2D::Create()
{
	CObject2D* pObject2D = new CObject2D;
	pObject2D->Init();
	return pObject2D;
}

//�e�N�X�`���ݒ�
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR2 fTexpos)
{
	m_pTexture = pTex;
	m_NowTexpos_x = (float)fTexpos.x;
	m_NowTexpos_y = (float)fTexpos.y;
}


void CObject2D::SetParam(D3DXVECTOR3 fpos, float wide, float height)
{
	m_pos = fpos;
	//�Ίp���̒������Z�o
	m_fLengthPolygon = sqrtf(wide * wide + height * height) / 2.0f;
	//�Ίp���̊p�x���Z�o
	m_fAnglePolygon = atan2f(wide, height);
}

void CObject2D::SetAnim(D3DXVECTOR2 AnimCnt)
{
	m_texcnt = AnimCnt;
}