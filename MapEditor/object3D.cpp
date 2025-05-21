#include "object3D.h"
#include "main.h"
#include "manager.h"

//�R���X�g���N�^
CObject3D::CObject3D()
{

}
// �f�X�g���N�^
CObject3D::~CObject3D()
{

}

//������
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	pDevice->CreateVertexBuffer(sizeof(CObject3D::VERTEX_3D) 
								* 4 * NUM_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, 
								D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D* pVtx; //���_���W�ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���W�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - 1000.0f, m_pos.y, m_pos.z + 1000.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + 1000.0f, m_pos.y, m_pos.z + 1000.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - 1000.0f, m_pos.y, m_pos.z - 1000.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + 1000.0f, m_pos.y, m_pos.z - 1000.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[1].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[2].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[3].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_NowTexpos_y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), (1.0f / m_NowTexpos_y));


	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//�`�揈��
void CObject3D::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//�X�V����
void CObject3D::Update()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	VERTEX_3D* pVtx; //���_���W�ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���W�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�


	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[1].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[2].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[3].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_NowTexpos_y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), (1.0f / m_NowTexpos_y));


	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//�I������
void CObject3D::Uninit()
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}


//Object3D����
CObject3D* CObject3D::Create()
{
	CObject3D* pObject3D = new CObject3D;
	pObject3D->Init();
	return pObject3D;
}
//�e�N�X�`���ݒ�
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR2 fTexpos)
{
	m_pTexture = pTex;
	m_NowTexpos_x = (float)fTexpos.x;
	m_NowTexpos_y = (float)fTexpos.y;
}
