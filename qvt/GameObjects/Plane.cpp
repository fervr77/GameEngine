#include "Plane.h"
#include "Components/Components.h"

namespace qvt
{
    Plane::Plane(glm::vec3 p,glm::vec3 n,glm::vec3 c)
    {
        entity = scene->CreateEntity("Plane");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<PlanePrimitiveComponent>(n,c);
        
        tr.position = p;
        tr.scale.x = 10;
        tr.scale.z = 10;
        

        entity.AddComponent<GameObjectNameComponent>("Plane");
        
    }

    Plane::~Plane()
    {

    }
}