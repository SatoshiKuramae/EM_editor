//==============================================================
//
// renderer.cpp
//
//==============================================================

#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "camera.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

//コンストラクタ
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}

//デストラクタ
CRenderer::~CRenderer()
{

}

//初期化
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;	//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//DirectXオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//高さ
	d3dpp.BackBufferFormat = d3ddm.Format;	//形式
	d3dpp.BackBufferCount = 1;				//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//デプスバッファを16bitとする
	d3dpp.Windowed = bWindow;	//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;


	//Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		//Direct3Dデバイスの生成（描画処理はハードウェア、頂点処理はCPUが行う）
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステート設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_Grid.Init(m_pD3DDevice);

	return S_OK;

}

//更新
void CRenderer::Update()	//全オブジェクトの更新
{
	CObject::UpdateAll();
}

//描画
void CRenderer::Draw()		//全オブジェクトの描画
{
	//画面クリア（バックバッファ＆Zバッファのクリア）
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(50, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功
		//カメラの設定
		CManager::GetCamera()->SetCamera();

		//各種オブジェクトの描画処理
		CObject::DrawAll();

		D3DXMATRIX view, proj, viewProj;
		LPDIRECT3DDEVICE9 device = CManager::GetRenderer()->GetDevice();

		device->GetTransform(D3DTS_VIEW, &view);
		device->GetTransform(D3DTS_PROJECTION, &proj);


		viewProj = view * proj;
		m_Grid.Draw(viewProj);


		// ImGui の描画処理
		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		//描画終了
		m_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//終了
void CRenderer::Uninit()
{
	//Direct3Dデバイスのの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
	m_Grid.Release();
}

//3Dデバイスの取得
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}
