#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H
#include "Scene/Camera.h"
#include <unordered_map>
#include <vector>

namespace qvt
{
    class KeyPressedEvent;
    class MouseMovedEvent;
    class WindowResizedEvent;
    class MouseScrolledEvent;
    class Scene;

    class CameraSystem
    {
        public:
            CameraSystem(Camera* Cam,Scene* sc);
            ~CameraSystem();
            void OnUpdate(float ts);
            bool OnKeyPressedEvent(KeyPressedEvent& e);
            bool OnMouseMovedEvent(MouseMovedEvent& e);
            bool OnWindowResizedEvent(WindowResizedEvent& e);
            bool OnMouseScrolledEvent(MouseScrolledEvent& e);
            static glm::vec3 GetMainCameraPosition();
            static void SetMainCamera(Camera* c);
            static bool FrustumCullingTest(glm::vec3 position,float radius);
            static std::unordered_map<std::string,std::vector<uint32_t>> GetFrustumFilteredMap(std::unordered_map<std::string,std::vector<uint32_t>>& map);

            bool firstMouse = true;



        private:
                enum CameraPlanes
            {
                Near = 0,
                Far = 1,
                Left = 2,
                Right = 3,
                Top = 4,
                Bottom = 5
            };

            struct Plane
            {
                float a;
                float b;
                float c;
                float d;
            };

            static std::vector<Plane> planes;

                
                



        private:
            static Camera* mainCamera;
            float deltaTime;
            
            float lastX;
            float lastY;
            static float aspectRatio;
            static std::unordered_map<std::string,int> uniformMap;
            static glm::mat4 projectionMatrix;
            static glm::mat4 viewMatrix;
            static Scene* scene;

            friend class RendererSystem;
            friend class GameLayer;
            friend class Panel;


    };

}
#endif