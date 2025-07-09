//============================================================
//
//gameobject.h
//
// AUther : SatoshiKuramae
//============================================================
 
#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_
#include "main.h"
#include "objectX.h"
#include "pch.h"
constexpr int NUMTEXTURE(5);
constexpr int NUMOBJECTTYPE(5);
constexpr double ARROWSCALE(1.0f);
constexpr double ARROWSCALE_HOLE(0.5f);


using json = nlohmann::json; // 省略しないなら nlohmann::json を毎回使ってもOK

class GameObject : public CObjectX {
public:

    enum class GameObjectType {
        SafeZone,
        Obstacle,
		HoleObstacle
    };

    GameObject();
    ~GameObject();

    HRESULT Init();
    virtual void Load() = 0;
    GameObject* Loadjson(const json& objData);
    void Update() override;
    void Draw() override;

    void SetObjectType(GameObjectType type) { m_type = type; }
    GameObjectType GetObjectType() const { return m_type; }

    const char* GetTypeString();
    GameObjectType FromTypeString(const std::string& str);

    void SetSummonCount(int summoncnt) { m_nSummonCnt = summoncnt; }
    int GetSummonCount()const { return m_nSummonCnt;}

    //向きではなく回転量を操作するから親クラスのrotじゃなくて独自の数値が必要なんすよﾎﾞｯｼｬｧｧｧﾝ
    void SetLogicRotation(const D3DXVECTOR3& rot) { m_logicRotation = rot; }
    //向きではなく回転量を操作するから親クラスのrotじゃなくて独自の数値が必要なんすよﾎﾞｯｼｬｧｧｧﾝ
    D3DXVECTOR3 GetLogicRotation() const { return m_logicRotation; }

    // モデル差し替え関数（派生クラスで必要に応じてオーバーライド）
    virtual void ChangeModel(const std::string& modelPath) {}
    // モデルパスのGetter/Setter
    std::string GetModelPath() const { return m_modelPath; }
    void SetModelPath(const std::string& path) { m_modelPath = path; }
    
protected:
	//オブジェクトのパラメータ用変数
    std::string m_name;				
    GameObjectType m_type;
    int m_nSummonCnt;
    std::string m_modelPath;		//モデル情報のパス
    D3DXVECTOR3 m_logicRotation;    //回転量
};

//種類をもったゲージオブジェクト
class CGenericObject : public GameObject {
public:
    CGenericObject() {};
    virtual ~CGenericObject() {};
    HRESULT Init();
    void Load() override;
    void Draw() override;
    static CGenericObject* Create(const std::string& modelPath);
    void ChangeModel(const std::string& modelPath) override;
    void ReleaseModelResources();
protected:
    LPD3DXMESH m_pMesh_Gn_Object = nullptr;
    LPD3DXBUFFER m_pBuffMat_Gn_Object = nullptr;
    DWORD m_dwNumMat_Gn_Object = 0;
    D3DXMATERIAL* m_pMaterial_Gn_Object = nullptr;
    LPDIRECT3DTEXTURE9 m_pTexture_Gn_Object[NUMTEXTURE] = { nullptr };
};

//穴あきオブジェクトクラス
class HoleObject : public CGenericObject {
	
public:
	struct HoleMarkerData {
		D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 scale = { 1.0f, 1.0f, 1.0f };
	};

	void SetHoleOffset(D3DXVECTOR3 offset) { m_holeOffset = offset; }
	D3DXVECTOR3 GetHoleOffset() const { return m_holeOffset; }

	void SetHoleRot(D3DXVECTOR3 rot) { m_holerot = rot; }
	D3DXVECTOR3 GetHoleRot() const { return m_holerot; }

	void SetHoleScale(D3DXVECTOR3 scale) { m_holescale = scale; }
	D3DXVECTOR3 GetHoleScale() const { return m_holescale; }

	void SetHoleVisibleObject(GameObject* obj) { m_visualObj = obj; }
	GameObject* GetHoleVisibleObject() const { return m_visualObj; }
private:
	D3DXVECTOR3 m_holeOffset = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 m_holerot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 m_holescale = { 1.0f, 1.0f,1.0f };
	GameObject* m_visualObj = nullptr;

};

//矢印オブジェクト
class ArrowObject : public GameObject
{
public:
    ArrowObject() {};
    ~ArrowObject() {};

    HRESULT Init();
    void Load() override;
    void Draw() override;
    static ArrowObject* Create(bool isoffset);		
    void SetVisible(bool visible) { m_isVisible = visible; }	//可視化フラグ切り替え
    bool IsVisible() const { return m_isVisible; }

private:
    // ArrowObject固有のメッシュ、マテリアル、テクスチャ
    LPD3DXMESH m_pMesh_arrow = nullptr;
    LPD3DXBUFFER m_pBuffMat_arrow = nullptr;
    DWORD m_dwNumMat_arrow = 0;
    D3DXMATERIAL* m_pMaterial_arrow = nullptr;
    LPDIRECT3DTEXTURE9 m_pTexture_arrow[NUMTEXTURE] = { nullptr };
    bool m_isVisible = false;
};

//穴可視化オブジェクト
class HoleMarkerObject : public GameObject {
public:
	HoleMarkerObject() = default;
	~HoleMarkerObject() override = default;

	HRESULT Init() override;
	void Draw() override;
	void Load() override;
	static HoleMarkerObject* Create(HoleObject* parent);
	void SetVisible(bool visible) { m_isVisible = visible; }
	bool IsVisible() const { return m_isVisible; }

	void ApplyHoleParameters(HoleObject* holeObj) { m_parent = holeObj; }
private:
	// ArrowObject固有のメッシュ、マテリアル、テクスチャ
	LPD3DXMESH m_pMesh_Holemarker = nullptr;
	LPD3DXBUFFER m_pBuffMat_Holemarker = nullptr;
	DWORD m_dwNumMat_Holemarker = 0;
	D3DXMATERIAL* m_pMaterial_Holemarker = nullptr;
	LPDIRECT3DTEXTURE9 m_pTexture_Holemarker[NUMTEXTURE] = { nullptr };
	bool m_isVisible = false;
	HoleObject* m_parent = nullptr;
	
};
#endif
