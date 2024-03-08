#include "Entity.h"

#include <thread>

typedef void *(*UTIL_CreateEntityByName)(const char *, int);
CEntityInstance *FetchInstanceByInput(std::string input);

Entity::Entity(std::string classname)
{
    CEntityInstance *instance = FetchInstanceByInput(classname);
    if (starts_with(classname, "player:"))
    {
        this->worldEntity = reinterpret_cast<CBaseModelEntity *>(instance);
        this->player = true;
        this->spawned = true;
    }
    else if (starts_with(classname, "entity:"))
    {
        this->worldEntity = nullptr;
        PRINTF("Entity::Entity", "You can't create an entity from another entity.\n");
    }
    else
    {
        this->worldEntity = reinterpret_cast<CBaseModelEntity *>(g_Signatures->FetchSignature<UTIL_CreateEntityByName>("UTIL_CreateEntityByName")(classname.c_str(), -1));
    }

    if (this->worldEntity == nullptr)
        PRINTF("Entity::Entity", "Failed to create entity.\n");
}

Entity::Entity(CEntityInstance *entityInstance)
{
    this->worldEntity = (CBaseModelEntity *)entityInstance;
}

Vector Entity::GetCoords()
{
    return this->worldEntity->m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin();
}

QAngle Entity::GetAngle()
{
    return this->worldEntity->m_CBodyComponent->m_pSceneNode->m_angAbsRotation();
}

void Entity::SetCoords(float x, float y, float z)
{
    Vector vec(x, y, z);

    this->worldEntity->m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin = vec;
}

void Entity::SetAngle(float x, float y, float z)
{
    QAngle angle(x, y, z);
    this->worldEntity->m_CBodyComponent->m_pSceneNode->m_angAbsRotation = angle;
}

void Entity::SetSolidType(SolidType_t solid_type)
{
    if (this->player)
        return;

    this->worldEntity->m_Collision.Get().m_nSolidType = solid_type;
}

void Entity::SetModel(std::string model)
{
    bool hasModel = (g_precacher->HasModelInList(model));
    if (!hasModel)
    {
        g_precacher->AddModel(model);
        PRINTF("Precacher", "Model '%s' was not precached before and it was added to the list.\n", model.c_str());
        PRINT("Precacher", "It may work on the second map change if the model is valid.\n");
    }

    this->model = model;

    if (hasModel && this->spawned)
        this->SetModel(model.c_str());
}

void Entity::Spawn()
{
    if (this->spawned)
        return;

    this->spawned = true;
    this->worldEntity->Spawn();

    CBaseModelEntity *ent = this->worldEntity;
    std::string mod = this->model;

    g_Plugin.NextFrame([ent, mod]() -> void
                       { ent->SetModel(mod.c_str()); });
}

void Entity::Destroy()
{
    this->worldEntity->Despawn();
    this->spawned = false;
}

Color Entity::GetColor()
{
    return this->worldEntity->m_clrRender();
}

void Entity::SetColor(int r, int g, int b, int a)
{
    Color color(r, g, b, a);

    this->worldEntity->m_clrRender = color;
}

void Entity::AcceptInput(const char *pInputName, CEntityInstance *pActivator, CEntityInstance *pCaller, double *value)
{
    if (this->spawned == false)
        return;

    g_Signatures->FetchSignature<CEntityInstance_AcceptInput>("CEntityInstance_AcceptInput")(this->worldEntity, pInputName, pActivator, pCaller, value, 0);
}

void Entity::SetCollisionGroup(Collision_Group_t collisionGroup)
{
    this->worldEntity->m_Collision().m_CollisionGroup = collisionGroup;
    this->worldEntity->m_Collision().m_collisionAttribute().m_nCollisionGroup = collisionGroup;

    this->worldEntity->OnCollisionRulesUpdate();
}