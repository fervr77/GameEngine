#include "Sphere2.h"
#include "Components/Components.h"

namespace qvt
{
    Sphere2::Sphere2(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Sphere2");
        entity.AddComponent<MeshComponent>("s2");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("Sphere2");


    }

    Sphere2::~Sphere2()
    {
        
    }
}