#ifndef APP1_TEST_H
#define APP1_TEST_H
#include "Scene/ScriptableEntity.h"
#include "Core/Input.h"

namespace qvt
{
    class Test: public ScriptableEntity
    {
        public:
        
        Test() = default;
        virtual ~Test();

        virtual void OnCreate()override;

        virtual void OnUpdate(float ts)override;

        virtual void OnDestroy()override;

        virtual void OnImGuiRender()override;

       
        


    };
}

#endif