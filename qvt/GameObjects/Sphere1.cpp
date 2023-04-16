#include "Sphere1.h"
#include "Components/Components.h"

namespace qvt
{
    Sphere1::Sphere1(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Sphere1");
        entity.AddComponent<MeshComponent>("s1");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("Sphere1");


    }

    Sphere1::~Sphere1()
    {
        
    }
}