#include "Cube.h"
#include "Components/Components.h"

namespace qvt
{
    Cube::Cube(glm::vec3 pos)
    {
        entity = scene->CreateEntity("cube");
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        entity.AddComponent<MeshComponent>("cube");
        entity.AddComponent<GameObjectNameComponent>("Cube");

    }

    Cube::~Cube()
    {

    }
}