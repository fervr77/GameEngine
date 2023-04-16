#include <glad/glad.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Systems/CameraSystem.h"

namespace qvt
{
    Camera::Camera(glm::vec3 position,float ar)
    {
        this->position = position;
        this->fov = 45.0f;
        this->cameraSpeed = 25.0f;
        this->mouseSensitivity = 0.1f;
        this->yaw = -90.0f;
        this->pitch = 0.0f;
        this->WorldUp = glm::vec3(0,1,0);
        this->aspectRatio = ar;
        this->zNear = 1.0f;
        this->zFar = 10000.0f;
        this->primary = false;
        this->frustumColor = glm::vec3(1.0f,0.0f,0.0f);
        this->showFrustum = false;
        this->xzView = false;
        this->yzView = false;
        this->xyView = false;
        this->oppositeView = false;

        
        

        UpdateVectors();

        frustumPoints.push_back(position);
        frustumPoints.push_back(glm::vec3(0,0,0));
        frustumPoints.push_back(glm::vec3(0,0,0));
        frustumPoints.push_back(glm::vec3(0,0,0));
        frustumPoints.push_back(glm::vec3(0,0,0));
        frustumPoints.push_back(glm::vec3(0,0,0));
        frustumPoints.push_back(glm::vec3(0,0,0));
        frustumPoints.push_back(glm::vec3(0,0,0));
        frustumPoints.push_back(glm::vec3(0,0,0));

        this->frustumSize = 16;
        nearPlane.push_back(frustumPoints[5]);
        nearPlane.push_back(frustumPoints[6]);
        nearPlane.push_back(frustumPoints[7]);
        nearPlane.push_back(frustumPoints[8]);
        nearPlane.push_back(frustumPoints[5]);
        nearPlane.push_back(frustumPoints[1]);
        nearPlane.push_back(frustumPoints[2]);
        nearPlane.push_back(frustumPoints[3]);
        nearPlane.push_back(frustumPoints[4]);
        nearPlane.push_back(frustumPoints[1]);
        nearPlane.push_back(frustumPoints[2]);
        nearPlane.push_back(frustumPoints[6]);
        nearPlane.push_back(frustumPoints[7]);
        nearPlane.push_back(frustumPoints[3]);
        nearPlane.push_back(frustumPoints[4]);
        nearPlane.push_back(frustumPoints[8]);

        glGenVertexArrays(1,&FrustumVAO);
        glBindVertexArray(FrustumVAO);
        glGenBuffers(1,&FrustumVBO);
        glBindBuffer(GL_ARRAY_BUFFER,FrustumVBO);
        glBufferData(GL_ARRAY_BUFFER,frustumSize*sizeof(glm::vec3),nearPlane.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);


    }

    Camera::~Camera()
    {

    }

    void Camera::SetAsPrimary()
    {
        CameraSystem::SetMainCamera(this);
        
        this->primary = true;

    }

    void Camera::UpdateVectors()
    {
        
        glm::vec3 front;
        front.x = glm::cos(glm::radians(yaw))*glm::cos(glm::radians(pitch));
        front.y = glm::sin(glm::radians(pitch));
        front.z = glm::sin(glm::radians(yaw))*glm::cos(glm::radians(pitch));

        this->Front = glm::normalize(front);
        this->Right = glm::normalize(glm::cross(this->Front,this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right,this->Front));

    }

    void Camera::GetKeyboardInput(CameraMovement m,float ts)
    {
        float v = cameraSpeed*ts;
       
        if(m==FORWARD)
        {
            this->position += v*this->Front;
            
        }
        else if(m==BACKWARD)
        {
            this->position -= v*this->Front;
            
        }

        else if(m==LEFT)
        {
            this->position -= v*this->Right;
        
        }

        else if(m==RIGHT)
        {
            this->position += v*this->Right;
        }

    }

    void Camera::ProcessMouseInput(float xoffset,float yoffset)
    {
        if(xzView == false && xyView == false && yzView==false)
        {

        
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if(pitch>89.0f)
        {
            pitch = 89.0f;
        }

        if(pitch<-89.0f)
        {
            pitch = -89.0f;
        }

        UpdateVectors();

        }
        else if(xzView)
        {
        
            yoffset *= mouseSensitivity;

            position.z += yoffset;

        }
        else if(yzView)
        {
             yoffset *= mouseSensitivity;

            position.y += yoffset;

        }
        else if(xyView)
        {
            yoffset *= mouseSensitivity;

            position.y += yoffset;

        }

    }

    void Camera::ProcessMouseScrollInput(float xpos,float ypos)
    {
        this->fov -= (ypos);

        if(fov<1.0f)
        {
            fov = 1.0f;
        }
        if(fov>45.0f)
        {
            fov = 45.0f;
        }
    }

    void Camera::UpdateAspectRatio(float ar)
    {
        this->aspectRatio = ar;
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        
        
            return glm::perspective(glm::radians(this->fov),this->aspectRatio,this->zNear,this->zFar);

        
        
    }


    glm::mat4 Camera::GetViewMatrix()
    {
        

        return glm::lookAt(this->position,this->position+this->Front,this->Up);
    }
}