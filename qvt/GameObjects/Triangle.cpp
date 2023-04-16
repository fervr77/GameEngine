#include "Triangle.h"
#include "Components/Components.h"

namespace qvt
{
    Triangle::Triangle(glm::vec3 p,glm::vec3 c)
    {
        entity = scene->CreateEntity("Triangle");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<TrianglePrimitiveComponent>(c);
        
        tr.position = p;
        
        

        entity.AddComponent<GameObjectNameComponent>("Triangle");
        
    }

    Triangle::~Triangle()
    {

    }
}