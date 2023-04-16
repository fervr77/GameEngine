#include "Cylinder.h"
#include "Components/Components.h"

namespace qvt
{
    Cylinder::Cylinder(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Cylinder");
        entity.AddComponent<MeshComponent>("cylinder");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("Cylinder");


    }

    Cylinder::~Cylinder()
    {
        
    }

}