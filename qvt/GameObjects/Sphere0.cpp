#include "Sphere0.h"
#include "Components/Components.h"

namespace qvt
{
    Sphere0::Sphere0(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Sphere0");
        entity.AddComponent<MeshComponent>("s0");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("Sphere0");


    }

    Sphere0::~Sphere0()
    {
        
    }
}