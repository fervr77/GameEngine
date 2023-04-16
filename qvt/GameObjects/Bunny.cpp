#include "Bunny.h"
#include "Components/Components.h"

namespace qvt
{
    Bunny::Bunny(glm::vec3 pos)
    {
        entity = scene->CreateEntity("Bunny");
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        tr.scale = glm::vec3(15,15,15);
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        entity.AddComponent<MeshComponent>("bunny");
        entity.AddComponent<GameObjectNameComponent>("Bunny");

    }

    Bunny::~Bunny()
    {

    }
}