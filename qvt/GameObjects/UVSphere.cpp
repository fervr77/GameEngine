#include "UVSphere.h"
#include "Components/Components.h"

namespace qvt
{
    UVSphere::UVSphere(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("UVSphere");
        entity.AddComponent<MeshComponent>("uvsphere");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("UVSphere");


    }

    UVSphere::~UVSphere()
    {
        
    }
}