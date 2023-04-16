#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <vector>
#include "Scene/Entity.h"

namespace qvt
{
    class Entity;

    class Scene;

    class GameObject
    {
        public:
            GameObject();
            ~GameObject();
            Entity& GetEntity(){return entity;}

            template<typename T>
            T& GetComponent()
            {
                return entity.GetComponent<T>();
            }

            template<typename T,typename... Args>
            T& AddComponent(Args&&... args)
            {
                return entity.AddComponent<T>(args...);
            }

            template<typename T>
            bool HasComponent()
            {
                return entity.HasComponent<T>();
            }

            template<typename T>
            void RemoveComponent()
            {
                entity.RemoveComponent<T>();
            }

        protected:
            Scene* scene;
            Entity entity;



    };

}
#endif