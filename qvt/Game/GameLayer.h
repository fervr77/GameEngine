#ifndef GAME_LAYER_H
#define GAME_LAYER_H
#include "Core/Layer.h"
#include "Scene/Entity.h"
#include "Systems/RendererSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/LightSystem.h"
#include "Systems/TerrainSystem.h"
#include "Systems/ScriptSystem.h"
#include "Systems/CollisionSystem.h"
#include "Scene/Camera.h"
#include "Renderer/Framebuffer.h"

#include <vector>



namespace qvt
{
    class KeyPressedEvent;
    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
    class MouseMovedEvent;
    class WindowResizedEvent;
    class Panel;
   

    class GameLayer : public Layer 
    {
        public:
            GameLayer(Scene* sc);
            virtual ~GameLayer();
            virtual void OnAttach()override;
            virtual void OnDetach()override;
            virtual void OnUpdate(float ts)override;
            virtual void OnEvent(Event& event)override;
            virtual void OnImGuiRender()override;
            void SetMainCameraAspectRatio(float ar);
            void OnViewportResizeEvent();
            static Scene* scene;
            




        private:
            bool OnKeyPressedEvent(KeyPressedEvent& e);
            bool OnMousePressedEvent(MouseButtonPressedEvent& e);
            bool OnMouseReleasedEvent(MouseButtonReleasedEvent& e);
            bool OnMouseMovedEvent(MouseMovedEvent& e);
            bool OnWindowResizedEvent(WindowResizedEvent& e );
            bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
            bool OnMouseScrolledEvent(MouseScrolledEvent& e);
            

            

        private:
            static float m_deltaTime;
            
            Entity object;
            RendererSystem* rendererSystem;
            CameraSystem* cameraSystem;
            LightSystem* lightSystem;
            TerrainSystem* terrainSystem;
            ScriptSystem* scriptSystem;
            CollisionSystem* collisionSystem;
            bool m_viewporthovered;
            bool m_viewportfocused;
            static Camera* editorCamera;
            static Camera* mainCamera;
            static glm::vec2 m_viewportSize; 
		    glm::vec2 m_viewportBounds[2];
            Panel* panel;
            int mousePressedPos[2];
            int mouseReleasedPos[2];
            static Framebuffer* framebuffer;
            FramebufferSpecification spec;

            static Framebuffer* camerabuffer;
            FramebufferSpecification camerabufferSpec;
            static int hoveredEntity;
            static int gizmotype;



            static bool isCameraOn;
            static unsigned int cameraQuadVAO;
            static unsigned int cameraQuadVBO;
            static unsigned int cameraQuadEBO;
            static unsigned int cameraQuadTexture;
           
           


           


            friend class Panel;
            friend class CameraSystem;
            friend class RendererSystem;
            







    };

}
#endif