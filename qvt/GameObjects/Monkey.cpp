#include "Monkey.h"
#include "Components/Components.h"

namespace qvt
{
    Monkey::Monkey(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Monkey");
        entity.AddComponent<MeshComponent>("monkey");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("Monkey");


    }

    Monkey::~Monkey()
    {
        
    }
}