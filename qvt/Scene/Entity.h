#ifndef ENTITY_H
#define ENTITY_H
#include <entt/entt.hpp>
#include "Core/Logger.h"
#include <glm/glm.hpp>
#include "Scene/Scene.h"

namespace qvt
{
    

    class Entity
    {
        public:
            Entity() = default;
            virtual ~Entity() = default;
            Entity(entt::entity handle,Scene* scene);
            Entity(const Entity& other) = default;

            template<typename T,typename... Args>
            T& AddComponent(Args&&... args)
            {
                if(m_scene->m_registry.any_of<T>(m_entityHandle))
                {
                    QVT_CORE_CRITICAL("Entity already has that component");
                    T& component = m_scene->m_registry.get<T>(m_entityHandle);

                    return component;
                    
                }

                
                T& component = m_scene->m_registry.emplace<T>(m_entityHandle,std::forward<Args>(args)...);
                componentID++;


                return component;
            }

            template<typename T>
            T& GetComponent()
            {
                return m_scene->m_registry.get<T>(m_entityHandle);
            }

            template<typename T>
            bool HasComponent()
            {
                return m_scene->m_registry.all_of<T>(m_entityHandle);
            }

            template<typename T>
            void RemoveComponent()
            {
                m_scene->m_registry.remove<T>(m_entityHandle);
            }

            entt::entity GetEntityID(){return m_entityHandle;}

            operator bool()const { return m_entityHandle != entt::null;}
            operator uint32_t()const { return (uint32_t)m_entityHandle;}
            operator entt::entity()const { return m_entityHandle;}
            bool operator==(const Entity& other){return m_entityHandle==other.m_entityHandle && m_scene == other.m_scene;}
            bool operator!=(const Entity& other){return !(*this==other);}





        private:
            entt::entity m_entityHandle = entt::null;
            Scene* m_scene = nullptr;
            static unsigned int componentID;

            friend class Scene;



    };

}
#endif