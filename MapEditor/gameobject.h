//============================================================
//
//gameobject.h
//
//============================================================
 
#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_
#include "main.h"
#include "objectX.h"
#include "pch.h"
#define NUMTEXTURE (5)
#define NUMOBJECTTYPE (5)
#define JUMP_DEC (2.2f)		//重力


using json = nlohmann::json; // 省略しないなら nlohmann::json を毎回使ってもOK

class GameObject : public CObjectX {
public:

    enum class GameObjectType {
        SafeZone,
        Obstacle
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

protected:
    std::string m_name;
    GameObjectType m_type;
    int m_nSummonCnt;
    std::string m_modelPath;

};

//現在表示しているキューブ 
class CubeObject : public GameObject
{
public:
    CubeObject() {};
    ~CubeObject() {};

    HRESULT Init();
    void Load() override;
    static CubeObject* Create();

private:
    // Cube固有のメッシュ、マテリアル、テクスチャ
    LPD3DXMESH m_pMesh_cube = nullptr;
    LPD3DXBUFFER m_pBuffMat_cube = nullptr;
    DWORD m_dwNumMat_cube = 0;
    D3DXMATERIAL* m_pMaterial_cube = nullptr;
    LPDIRECT3DTEXTURE9 m_pTexture_cube[NUMTEXTURE] = { nullptr };
};

//種類をもったゲージオブジェクト　TODO 配置するオブジェに種類を追加、配置後に種類を変更できるようにすると良い
//class GenericObject : public GameObject {
//public:
//    GenericObject(const std::string& path) {
//        m_modelPath = path;
//    }
//    ~GenericObject() {};
//
//
//    HRESULT Init();
//    void Load() override;
//    void Draw() override;
//    static ArrowObject* Create();
//
//};

//矢印オブジェクト
class ArrowObject : public GameObject
{
public:
    ArrowObject() {};
    ~ArrowObject() {};

    HRESULT Init();
    void Load() override;
    void Draw() override;
    static ArrowObject* Create();
    void SetVisible(bool visible) { m_isVisible = visible; }
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
#endif
