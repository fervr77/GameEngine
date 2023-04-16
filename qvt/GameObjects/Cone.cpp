#include "Cone.h"
#include "Components/Components.h"

namespace qvt
{
    Cone::Cone(glm::vec3 pos)
    {
        entity = scene->CreateEntity("Cone");
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        entity.AddComponent<MeshComponent>("cone");
        entity.AddComponent<GameObjectNameComponent>("Cone");

    }

    Cone::~Cone()
    {

    }
}