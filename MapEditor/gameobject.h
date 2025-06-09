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
#define JUMP_DEC (2.2f)		//�d��


using json = nlohmann::json; // �ȗ����Ȃ��Ȃ� nlohmann::json �𖈉�g���Ă�OK

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

    void SetLogicRotation(const D3DXVECTOR3& rot) { m_logicRotation = rot; }
    D3DXVECTOR3 GetLogicRotation() const { return m_logicRotation; }

    // ���f�������ւ��֐��i�h���N���X�ŕK�v�ɉ����ăI�[�o�[���C�h�j
    virtual void ChangeModel(const std::string& modelPath) {}
    // ���f���p�X��Getter/Setter
    std::string GetModelPath() const { return m_modelPath; }
    void SetModelPath(const std::string& path) { m_modelPath = path; }
    
protected:
    std::string m_name;
    GameObjectType m_type;
    int m_nSummonCnt;
    std::string m_modelPath;
    D3DXVECTOR3 m_logicRotation;    //��]��
};



//��ނ��������Q�[�W�I�u�W�F�N�g�@TODO �z�u����I�u�W�F�Ɏ�ނ�ǉ��A�z�u��Ɏ�ނ�ύX�ł���悤�ɂ���Ɨǂ�
class CGenericObject : public GameObject {
public:
    CGenericObject() {};
    ~CGenericObject() {};


    HRESULT Init();
    void Load() override;
    void Draw() override;
    static CGenericObject* Create(const std::string& modelPath);
    void ChangeModel(const std::string& modelPath) override;
    void ReleaseModelResources();
private:
    LPD3DXMESH m_pMesh_Gn_Object = nullptr;
    LPD3DXBUFFER m_pBuffMat_Gn_Object = nullptr;
    DWORD m_dwNumMat_Gn_Object = 0;
    D3DXMATERIAL* m_pMaterial_Gn_Object = nullptr;
    LPDIRECT3DTEXTURE9 m_pTexture_Gn_Object[NUMTEXTURE] = { nullptr };
};

//���I�u�W�F�N�g
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
    // ArrowObject�ŗL�̃��b�V���A�}�e���A���A�e�N�X�`��
    LPD3DXMESH m_pMesh_arrow = nullptr;
    LPD3DXBUFFER m_pBuffMat_arrow = nullptr;
    DWORD m_dwNumMat_arrow = 0;
    D3DXMATERIAL* m_pMaterial_arrow = nullptr;
    LPDIRECT3DTEXTURE9 m_pTexture_arrow[NUMTEXTURE] = { nullptr };
    bool m_isVisible = false;
};
#endif
