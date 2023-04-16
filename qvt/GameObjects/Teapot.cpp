#include "Teapot.h"
#include "Components/Components.h"

namespace qvt
{
    Teapot::Teapot(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Teapot");
        entity.AddComponent<MeshComponent>("teapot");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        tr.scale = glm::vec3(.01,.01,.01);
        entity.AddComponent<GameObjectNameComponent>("Teapot");


    }

    Teapot::~Teapot()
    {
        
    }

}