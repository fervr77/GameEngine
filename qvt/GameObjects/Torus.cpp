#include "Torus.h"
#include "Components/Components.h"

namespace qvt
{
    Torus::Torus(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Torus");
        entity.AddComponent<MeshComponent>("torus");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("Torus");


    }

    Torus::~Torus()
    {
        
    }

}