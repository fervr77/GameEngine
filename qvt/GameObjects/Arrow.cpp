#include "Arrow.h"
#include "Components/Components.h"

namespace qvt
{
    Arrow::Arrow(glm::vec3 pos)
    {
        entity = scene->CreateEntity("Arrow");
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        tr.scale = glm::vec3(70,70,70);
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        entity.AddComponent<MeshComponent>("arrow");
        entity.AddComponent<GameObjectNameComponent>("Arrow");

    }

    Arrow::~Arrow()
    {

    }
}