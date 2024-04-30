#ifndef _entity_h
#define _entity_h

#include "../common.h"
#include "../sig/Signatures.h"
#include <public/mathlib/vector.h>
#include "../sdk/entity/CBaseModelEntity.h"

class Entity
{
private:
    CBaseModelEntity *worldEntity = nullptr;
    Z_CBaseEntity *entity = nullptr;
    std::string model;
    bool spawned = false;
    bool player = false;

public:
    Entity(std::string classname);
    Entity(CEntityInstance *entityInstance);

    Vector GetCoords();
    QAngle GetAngle();
    Color GetColor();

    void SetCoords(float x, float y, float z);
    void SetAngle(float x, float y, float z);
    void SetColor(int r, int g, int b, int a);

    void SetModel(std::string model);
    void AcceptInput(const char *pInputName, CEntityInstance *pActivator = nullptr, CEntityInstance *pCaller = nullptr, double *value = nullptr);
    CEntityInstance *GetEntityInstance() { return this->worldEntity; }

    void Spawn();
    void Destroy();

    void SetCollisionGroup(Collision_Group_t collisionGroup);
    void SetSolidType(SolidType_t solid_type);
    Collision_Group_t GetCollisionGroup(uint32_t entityID);
};

#endif