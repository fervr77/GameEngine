#include "HoverBike.h"
#include "Components/Components.h"

namespace qvt
{
    HoverBike::HoverBike(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("HoverBike");
        entity.AddComponent<MeshComponent>("hoverbike");
        entity.AddComponent<PbrRendererComponent>("hoverbike",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("HoverBike");


    }

    HoverBike::~HoverBike()
    {
        
    }
}