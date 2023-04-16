#include "Tetrahedron.h"
#include "Components/Components.h"

namespace qvt
{
    Tetrahedron::Tetrahedron(glm::vec3 p,glm::vec3 c)
    {
        entity = scene->CreateEntity("Tetrahedron");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<TetrahedronPrimitiveComponent>(c);
        
        tr.position = p;
        
        

        entity.AddComponent<GameObjectNameComponent>("Tetrahedron");
        
    }

    Tetrahedron::~Tetrahedron()
    {

    }
}