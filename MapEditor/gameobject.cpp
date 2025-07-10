//===============================================================================
//
//gameObject.cpp
//
// Auther : SatoshiKuramae
//===============================================================================
#include "main.h"
#include "manager.h"
#include "gameobject.h"

//===================================================================
//
//親クラス
//
//===================================================================

//コンストラクタ
GameObject::GameObject()
{
    // 位置・スケールなど初期化
    SetPos(D3DXVECTOR3(0, 0, 0));
    SetScale(D3DXVECTOR3(1, 1, 1));
    SetRot(D3DXVECTOR3(0, 0, 0));
    SetSummonCount(0);
}

//デストラクタ
GameObject::~GameObject()
{
    if (m_pMesh) {
        m_pMesh->Release();
        m_pMesh = nullptr;
    }

    if (m_pBuffMat) {
        m_pBuffMat->Release();
        m_pBuffMat = nullptr;
    }

    // テクスチャも忘れずに解放（必要であれば）
    for (int i = 0; i < NUMTEXTURE; ++i) {
        if (m_pTexture[i]) {
            m_pTexture[i]->Release();
            m_pTexture[i] = nullptr;
        }
    }
}

//初期化
HRESULT GameObject::Init()
{
    CObjectX::Init();

    return S_OK;
}

//更新処理
void GameObject::Update()
{
    // CObjectX の基本更新
    CObjectX::Update();
}

void GameObject::Draw()
{
    // CObjectX に描画任せて問題なければそのまま
    CObjectX::Draw();
}

//ファイル読み込み
GameObject* GameObject::Loadjson(const json& objData)
{
    this->Init();

	//JSONに書き出されたデータ名と同じになるように
	if (objData.contains("Move") && objData["Move"].is_array() && objData["Move"].size() == 3) {
		auto m = objData["Move"];
		this->SetMove(D3DXVECTOR3(m[0], m[1], m[2]));
	}
	else {
		std::cout << "[警告] Move が存在しないため、(0,0,0) を設定します\n";
		this->SetMove(D3DXVECTOR3(0, 0, 0));
	}

	if (objData.contains("Name")) {
		std::string typeStr = objData["Name"];
		this->SetObjectType(GameObject::FromTypeString(typeStr));
	}
	else {
		std::cout << "[警告] Name が存在しないため、デフォルトタイプを設定します\n";
		this->SetObjectType(GameObject::FromTypeString("Default"));  // 適切な初期型に
	}

	if (objData.contains("Pos") && objData["Pos"].is_array() && objData["Pos"].size() == 3) {
		auto p = objData["Pos"];
		this->SetPos(D3DXVECTOR3(p[0], p[1], p[2]));
	}
	else {
		std::cout << "[警告] Pos が存在しないため、(0,0,0) を設定します\n";
		this->SetPos(D3DXVECTOR3(0, 0, 0));
	}

	if (objData.contains("Rot") && objData["Rot"].is_array() && objData["Rot"].size() == 3) {
		auto r = objData["Rot"];
		this->SetLogicRotation(D3DXVECTOR3(r[0], r[1], r[2]));
	}
	else {
		std::cout << "[警告] Rot が存在しないため、(0,0,0) を設定します\n";
		this->SetLogicRotation(D3DXVECTOR3(0, 0, 0));
	}

	if (objData.contains("Rotation") && objData["Rotation"].is_array() && objData["Rotation"].size() == 3) {
		auto ro = objData["Rotation"];
		this->SetRot(D3DXVECTOR3(ro[0], ro[1], ro[2]));
	}
	else {
		std::cout << "[警告] Rotation が存在しないため、(0,0,0) を設定します\n";
		this->SetRot(D3DXVECTOR3(0, 0, 0));
	}

	if (objData.contains("Scale") && objData["Scale"].is_array() && objData["Scale"].size() == 3) {
		auto s = objData["Scale"];
		this->SetScale(D3DXVECTOR3(s[0], s[1], s[2]));
	}
	else {
		std::cout << "[警告] Scale が存在しないため、(1,1,1) を設定します\n";
		this->SetScale(D3DXVECTOR3(1, 1, 1));
	}

	if (objData.contains("SummonFrame")) {
		int summonframe = objData["SummonFrame"];
		this->SetSummonCount(summonframe);
	}
	else {
		std::cout << "[警告] SummonFrame が存在しないため、0 を設定します\n";
		this->SetSummonCount(0);
	}

	if (objData.contains("ModelName")) {
		std::string path = objData["ModelName"];
		this->SetModelPath(path);
	}
	else {
		std::cout << "[警告] ModelName が存在しないため、空文字を設定します\n";
		this->SetModelPath("");
	}
	if (objData.contains("ModelTag")) {
		std::string path = objData["ModelTag"];
		this->SetModelTag(path);
	}
	else {
		std::cout << "[警告] ModelName が存在しないため、空文字を設定します\n";
		this->SetModelTag("");
	}
	if (objData.contains("HoleOffset")) {
		auto o = objData["HoleOffset"];
		if (HoleObject* hole = dynamic_cast<HoleObject*>(this)) {
			hole->SetHoleOffset(D3DXVECTOR3(o[0], o[1], o[2]));
		}
		else
		{
			hole->SetHoleOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (objData.contains("HoleRot")) {
		auto o = objData["HoleRot"];
		if (HoleObject* hole = dynamic_cast<HoleObject*>(this)) {
			hole->SetHoleRot(D3DXVECTOR3(o[0], o[1], o[2]));
		}
		else
		{
			hole->SetHoleRot(D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
	}
	else if (objData.contains("HoleScale")) {
		auto o = objData["HoleOffset"];
		if (HoleObject* hole = dynamic_cast<HoleObject*>(this)) {
			hole->SetHoleScale(D3DXVECTOR3(o[0], o[1], o[2]));
		}
		else
		{
			hole->SetHoleScale(D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
	}
	else
	{
		std::cout << "[警告] HoleOffset が存在しないため、0 を設定します\n";
	}
    return this;
}

//オブジェクトの種類取得
const char* GameObject::GetTypeString() {
    switch (m_type) {
    case GameObjectType::SafeZone: return "SafeZone";
    case GameObjectType::Obstacle: return "Obstacle";
	case GameObjectType::HoleObstacle: return "HoleObstacle";
    default: return "Unknown";
    }
}

//読み込みデータと比較しGameObjectTypeを決める
GameObject::GameObjectType GameObject::FromTypeString(const std::string& str) {
    if (str == "SafeZone") return GameObjectType::SafeZone;
    if (str == "Obstacle") return GameObjectType::Obstacle;
	if (str == "HoleObstacle") return GameObjectType::HoleObstacle;
    return GameObjectType::SafeZone; // デフォルト
}


//===================================================================
//
//矢印オブジェクトクラス
//
//===================================================================
//初期化
HRESULT ArrowObject::Init()
{
    Load();

    CObjectX::Init();

    return S_OK;
}

//ロード
void ArrowObject::Load()
{
    CObject::SetType(TYPE::BLOCK);
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //Xファイルの読み込み
    D3DXLoadMeshFromX("data\\model\\Nodisplaymodel\\arrow.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_arrow,
        NULL,
        &m_dwNumMat_arrow,
        &m_pMesh_arrow);

    // モデルのテクスチャファイル
    m_pMaterial_arrow = (D3DXMATERIAL*)m_pBuffMat_arrow->GetBufferPointer();
    for (DWORD i = 0; i < m_dwNumMat_arrow; ++i)
    {
        if (m_pMaterial_arrow[i].pTextureFilename)
        {
            D3DXCreateTextureFromFile(pDevice, m_pMaterial_arrow[i].pTextureFilename, &m_pTexture_arrow[i]);
        }
    }
    // BindMesh呼び出し
    BindMesh(m_pMesh_arrow, m_pBuffMat_arrow, m_dwNumMat_arrow, m_pMaterial_arrow, m_pTexture_arrow);

}

//描画処理
void ArrowObject::Draw()
{
    if (!m_isVisible) {
        return;  // 非表示なら描画しない
    }

    // 通常の描画処理
    CObjectX::Draw();
}

//生成
//引数のisoffsetは穴の位置に配置する矢印オブジェクトか判定する
ArrowObject* ArrowObject::Create(bool isoffset)
{
    ArrowObject* obj = new ArrowObject();
    obj->Init();
    obj->SetMove(D3DXVECTOR3(0, 0, 0));
    obj->SetPos(D3DXVECTOR3(0, 0, 0));
    obj->SetRot(D3DXVECTOR3(0, 0, 0));

	if (isoffset) {
		obj->SetScale(D3DXVECTOR3(ARROWSCALE_HOLE, ARROWSCALE_HOLE, ARROWSCALE_HOLE));
	}
	else
	{
		obj->SetScale(D3DXVECTOR3(ARROWSCALE, ARROWSCALE, ARROWSCALE));
	}
	obj->SetVisible(false);
    obj->SetSummonCount(0);
    return obj;
}

//===================================================================
//
//複数対応型オブジェクトクラス
//
//===================================================================

//初期化
HRESULT CGenericObject::Init()
{
    CObjectX::Init();

    return S_OK;
}

//ロード
void CGenericObject::Load()
{
    ReleaseModelResources();

    CObject::SetType(TYPE::BLOCK);
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //Xファイルの読み込み
    D3DXLoadMeshFromX(m_modelPath.c_str(),
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_Gn_Object,
        NULL,
        &m_dwNumMat_Gn_Object,
        &m_pMesh_Gn_Object);

    // モデルのテクスチャファイル
    m_pMaterial_Gn_Object = (D3DXMATERIAL*)m_pBuffMat_Gn_Object->GetBufferPointer();

    for (DWORD i = 0; i < m_dwNumMat_Gn_Object; ++i)
    {
        if (m_pMaterial_Gn_Object[i].pTextureFilename)
        {
            D3DXCreateTextureFromFile(pDevice, m_pMaterial_Gn_Object[i].pTextureFilename, &m_pTexture_Gn_Object[i]);
        }
    }
    // BindMesh呼び出し
    BindMesh(m_pMesh_Gn_Object, m_pBuffMat_Gn_Object, m_dwNumMat_Gn_Object, m_pMaterial_Gn_Object, m_pTexture_Gn_Object);

}

//描画処理
void CGenericObject::Draw()
{
    // 通常の描画処理
    CObjectX::Draw();
}

//生成
CGenericObject* CGenericObject::Create(const std::string& modelPath)
{
    CGenericObject* obj = new CGenericObject();
    obj->SetModelPath(modelPath);
    obj->Init();
    obj->SetMove(D3DXVECTOR3(0, 0, 0));
    obj->SetPos(D3DXVECTOR3(0, 0, 0));
    obj->SetLogicRotation(D3DXVECTOR3(0, 0, 0));
	obj->SetRot(D3DXVECTOR3(0, 0, 0));
    obj->SetScale(D3DXVECTOR3(1, 1, 1));
    obj->SetSummonCount(0);
    
    return obj;
}

//モデル差し替え
void CGenericObject::ChangeModel(const std::string& modelPath) {
    SetModelPath(modelPath);  // モデルパス更新
    Load();                   // 再読み込み
}

//モデル情報の解放
void CGenericObject::ReleaseModelResources() {
    if (m_pMesh) { m_pMesh->Release(); m_pMesh = nullptr; }
    if (m_pBuffMat) { m_pBuffMat->Release(); m_pBuffMat = nullptr; }
    for (int i = 0; i < NUMTEXTURE; ++i) {
        if (m_pTexture[i]) { m_pTexture[i]->Release(); m_pTexture[i] = nullptr; }
    }
}


//================================================================
//
//穴の位置を可視化するオブジェクト
//
//================================================================

//初期化
HRESULT HoleMarkerObject::Init()
{
	Load();

	CObjectX::Init();

	return S_OK;
}

//ロード
void HoleMarkerObject::Load()
{
	CObject::SetType(TYPE::BLOCK);
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\model\\Nodisplaymodel\\Cylynder_000.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		NULL,
		&m_pBuffMat_Holemarker,
		NULL,
		&m_dwNumMat_Holemarker,
		&m_pMesh_Holemarker);

	// モデルのテクスチャファイル
	m_pMaterial_Holemarker = (D3DXMATERIAL*)m_pBuffMat_Holemarker->GetBufferPointer();
	for (DWORD i = 0; i < m_dwNumMat_Holemarker; ++i)
	{
		if (m_pMaterial_Holemarker[i].pTextureFilename)
		{
			D3DXCreateTextureFromFile(pDevice, m_pMaterial_Holemarker[i].pTextureFilename, &m_pTexture_Holemarker[i]);
		}
	}
	// BindMesh呼び出し
	BindMesh(m_pMesh_Holemarker, m_pBuffMat_Holemarker, m_dwNumMat_Holemarker, m_pMaterial_Holemarker, m_pTexture_Holemarker);

}

//描画処理
void HoleMarkerObject::Draw()
{
	if (!m_isVisible) {
		return;  // 非表示なら描画しない
	}

	// 通常の描画処理
	CObjectX::Draw();
}

//生成

HoleMarkerObject* HoleMarkerObject::Create(HoleObject* parent)
{
	HoleMarkerObject* obj = new HoleMarkerObject();
	obj->Init();
	obj->SetMove(D3DXVECTOR3(0, 0, 0));
	obj->SetPos(D3DXVECTOR3(0, 0, 0));
	obj->SetRot(D3DXVECTOR3(0, 0, 0));
	obj->SetScale(D3DXVECTOR3(1, 1, 1));
	obj->SetVisible(false);
	obj->SetSummonCount(0);
	return obj;
}
