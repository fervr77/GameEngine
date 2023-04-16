#include "Line.h"
#include "Components/Components.h"

namespace qvt
{
    Line::Line(glm::vec3 ap,glm::vec3 bp,glm::vec3 c)
    {
        entity = scene->CreateEntity("Line");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<LinePrimitiveComponent>(ap,bp,c);
       
        tr.position = (ap+bp)/2.0f;


        entity.AddComponent<GameObjectNameComponent>("Line");



    }

    Line::~Line()
    {

    }

   
}