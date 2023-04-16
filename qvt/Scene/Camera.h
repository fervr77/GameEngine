#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <vector>

namespace qvt
{
    class Camera
    {
        public:
            enum CameraMovement
            {
                FORWARD = 0,
                BACKWARD,
                LEFT,
                RIGHT

            };

        public:
            Camera(glm::vec3 position,float ar);
            ~Camera();
             glm::mat4 GetProjectionMatrix();
             glm::mat4 GetViewMatrix();
             void UpdateAspectRatio(float ar);
             void SetAsPrimary(void);
           





        private:
            void UpdateVectors();
            void GetKeyboardInput(CameraMovement m,float ts);
            void ProcessMouseInput(float xoffset,float yoffset);
            void ProcessMouseScrollInput(float xpos,float ypos);
            



        private:
            glm::vec3 position;
            glm::vec3 Front;
            glm::vec3 Right;
            glm::vec3 Up;
            glm::vec3 WorldUp;
            float yaw;
            float pitch;
            float fov;
            float aspectRatio;
            float cameraSpeed;
            float mouseSensitivity;
            float zNear;
            float zFar;
            bool primary;
            bool showFrustum;
            bool xzView;
            bool yzView;
            bool xyView;
            bool oppositeView;
            
            glm::vec3 frustumColor;
            std::vector<glm::vec3> frustumPoints;
            std::vector<glm::vec3> nearPlane;
            int frustumSize;
            unsigned int FrustumVAO;
            unsigned int FrustumVBO;
            

            friend class CameraSystem;
            friend class GameLayer;
            friend class Panel;
            friend class RendererSystem;


    };

}
#endif