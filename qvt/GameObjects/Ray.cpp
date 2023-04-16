#include "Ray.h"
#include "Components/Components.h"

namespace qvt
{
    Ray::Ray(glm::vec3 ap,glm::vec3 bp,glm::vec3 c)
    {
        entity = scene->CreateEntity("Ray");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<RayPrimitiveComponent>(ap,bp,c);
       
        tr.position = (ap+bp)/2.0f;


        entity.AddComponent<GameObjectNameComponent>("Ray");



    }

    Ray::~Ray()
    {

    }

   
}