#include "Point.h"
#include "Components/Components.h"

namespace qvt
{
    Point::Point(glm::vec3 p,glm::vec3 c)
    {
        entity = scene->CreateEntity("Point");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<PointPrimitiveComponent>(c);

        tr.position = p;

        entity.AddComponent<GameObjectNameComponent>("Point");
        
    }

    Point::~Point()
    {

    }
}