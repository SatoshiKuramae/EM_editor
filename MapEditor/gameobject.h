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
    GameObject();
    ~GameObject() override = default;

    HRESULT Init();
    void Load();
    GameObject* Loadjson(const json& objData);
    void Update() override;
    void Draw() override;
    static GameObject* Create();

private:

};
#endif
