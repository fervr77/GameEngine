#include "Statue.h"
#include "Components/Components.h"

namespace qvt
{
    Statue::Statue(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Statue");
        entity.AddComponent<MeshComponent>("statue");
        entity.AddComponent<PbrRendererComponent>("statue",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        tr.scale.x = .01;
        tr.scale.y = .01;
        tr.scale.z = .01;
        entity.AddComponent<GameObjectNameComponent>("Statue");


    }

    Statue::~Statue()
    {
        
    }
}