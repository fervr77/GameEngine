#ifndef APP1_H
#define APP1_H
#include "Core/Layer.h"
#include "Scene/Entity.h"
#include "Scene/Camera.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "GameObjects/GameAssets.h"

namespace qvt
{
    class App1: public Layer
    {
        public:
            App1(Scene* sc);
            virtual ~App1();
            virtual void OnAttach()override;
            virtual void OnDetach()override;
            virtual void OnUpdate(float ts)override;
            virtual void OnEvent(Event& event)override;
            virtual void OnImGuiRender()override;
            void SetMainCameraAspectRatio(float ar);
            void OnViewportResizeEvent();

        private:
            bool OnKeyPressedEvent(KeyPressedEvent& e);
            bool OnMousePressedEvent(MouseButtonPressedEvent& e);
            bool OnMouseMovedEvent(MouseMovedEvent& e);
            void ScriptSystemInit();
            



        private:
            Scene* scene;
            
            


        
            




    };

}
#endif