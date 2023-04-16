#include "HoverCar.h"
#include "Components/Components.h"

namespace qvt
{
    HoverCar::HoverCar(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("HoverCar");
        entity.AddComponent<MeshComponent>("hovercar");
        entity.AddComponent<PbrRendererComponent>("hovercar",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("HoverCar");


    }

    HoverCar::~HoverCar()
    {
        
    }
}