#include "Rectangle.h"
#include "Components/Components.h"

namespace qvt
{
    Rectangle::Rectangle(glm::vec3 p,glm::vec3 c)
    {
        entity = scene->CreateEntity("Rectangle");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<Rectangle3DPrimitiveComponent>(c);
        
        tr.position = p;
        
        

        entity.AddComponent<GameObjectNameComponent>("Rectangle");
        
    }

    Rectangle::~Rectangle()
    {

    }
}