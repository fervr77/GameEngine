#ifndef SCENE_H
#define SCENE_H
#include "Events/Event.h"
#include <entt/entt.hpp>



namespace qvt
{
    class Entity;

    class Scene
    {
        public:
            Scene();
            ~Scene();

            void OnEvent(Event& event);
            Entity CreateEntity(const std::string& name = std::string());
            void DestroyEntity(Entity entity);
            static unsigned int GetComponentID();
            static Scene& Get(){return *s_instance;}
            static unsigned int entityCounter;

            


        private:
            entt::registry m_registry;
            static Scene* s_instance;



            friend class Entity;
            friend class RendererSystem;
            friend class GameLayer;
            friend class TerrainSystem;
            friend class Panel;
            friend class CameraSystem;
            friend class ScriptSystem;
            friend class CollisionSystem;




    };

}
#endif