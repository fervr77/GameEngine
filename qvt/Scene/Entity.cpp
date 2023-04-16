#include "Entity.h"
#include "Scene/Scene.h"


namespace qvt
{
    unsigned int Entity::componentID = 0;

    Entity::Entity(entt::entity handle,Scene* scene):m_entityHandle(handle),m_scene(scene)
    {
        
    }

}