#include "Tree.h"
#include "Components/Components.h"

namespace qvt
{
    Tree::Tree(glm::vec3 pos)
    {
        this->entity = scene->CreateEntity("Tree");
        entity.AddComponent<MeshComponent>("tree");
        entity.AddComponent<PbrRendererComponent>("cyan",(uint32_t)entity.GetEntityID());
        auto& tr = entity.GetComponent<TransformComponent>();
        tr.position = pos;
        entity.AddComponent<GameObjectNameComponent>("Tree");


    }

    Tree::~Tree()
    {
        
    }

}