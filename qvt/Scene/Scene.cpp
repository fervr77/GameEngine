#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Components/Components.h"

namespace qvt
{
    
    unsigned int Scene::entityCounter = 0;
    Scene* Scene::s_instance = nullptr;

    Scene::Scene()
    {
        s_instance = this;
        

    }

    Scene::~Scene()
    {

    }

    void Scene::OnEvent(Event& event)
    {

    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity{m_registry.create(),this};
        auto& tr = m_registry.emplace<TransformComponent>(entity.m_entityHandle);
        auto& tag = m_registry.emplace<TagComponent>(entity.m_entityHandle);
        tag.name = name;
        auto& uuid = m_registry.emplace<UUIDComponent>(entity.m_entityHandle);
        uuid.id = entityCounter;
        auto& g = m_registry.emplace<Live2DGraphComponent>(entity.m_entityHandle);
        g.AddGraph("Position",&tr.position.x,"X position");
        g.AddGraph("Position",&tr.position.y,"Y position");
        g.AddGraph("Position",&tr.position.z,"Z position");
        g.AddGraph("Angle",&tr.rotation.x,"X angle");
        g.AddGraph("Angle",&tr.rotation.y,"Y angle");
        g.AddGraph("Angle",&tr.rotation.z,"Z angle");

        

        Scene::entityCounter++;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_registry.destroy(entity.m_entityHandle);
    }

    unsigned int Scene::GetComponentID()
    {
        return Entity::componentID;
    }









    

    





}