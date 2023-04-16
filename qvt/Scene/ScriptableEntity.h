#ifndef SCRIPTABLE_ENTITY_H
#define SCRIPTABLE_ENTITY_H
#include "Scene/Entity.h"

namespace qvt
{
    class Scene;

    class ScriptableEntity
    {
        public:
            virtual ~ScriptableEntity(){}
            template<typename T>
            T& GetComponent()
            {
                return m_entity.GetComponent<T>();
            }


        protected:
            virtual void OnCreate(){}
            virtual void OnUpdate(float ts){}
            virtual void OnDestroy(){}
            virtual void OnImGuiRender(){}
           



        protected:
            Entity m_entity;
            Scene* scene;

            friend class Scene;
            friend class ScriptSystem;
            friend class Panel;
            friend class GameLayer;


        
    };
}


#endif