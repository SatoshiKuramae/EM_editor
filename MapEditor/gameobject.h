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
#define JUMP_DEC (2.2f)		//èdóÕ
#define OBJ_MAX (5) // NUMOBJECTTYPEÇ…çáÇÌÇπÇÈ

using json = nlohmann::json; // è»ó™ÇµÇ»Ç¢Ç»ÇÁ nlohmann::json ÇñàâÒégÇ¡ÇƒÇ‡OK

class GameObject : public CObjectX {
public:

    enum class GameObjectType {
        SafeZone,  // Sã…
        Obstacle   // Nã…
    };

    GameObject();
    ~GameObject();

    HRESULT Init();
    void Load();
    GameObject* Loadjson(const json& objData);
    void Update() override;
    void Draw() override;
    static GameObject* Create();

    void SetObjectType(GameObjectType type) { m_type = type; }
    GameObjectType GetObjectType() const { return m_type; }

    const char* GetTypeString();
    GameObjectType FromTypeString(const std::string& str);

    void SetSummonCount(int summoncnt) { m_nSummonCnt = summoncnt; }
    int GetSummonCount()const { return m_nSummonCnt;}

    
    void MarkForDelete() { m_markedForDelete = true; }
    bool IsMarkedForDelete() const { return m_markedForDelete; }

private:
    std::string m_name;
    GameObjectType m_type;
    int m_nSummonCnt;
    bool m_markedForDelete = false;
    
};
#endif
