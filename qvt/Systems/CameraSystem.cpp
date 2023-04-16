#include <glad/glad.h>
#include "CameraSystem.h"
#include "Core/Input.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Renderer/ShaderLibrary.h"
#include <iostream>
#include "Core/Application.h"
#include "Components/Components.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




namespace qvt
{
    std::unordered_map<std::string,int> CameraSystem::uniformMap;
    glm::mat4 CameraSystem::projectionMatrix;
    glm::mat4 CameraSystem::viewMatrix;
    Camera* CameraSystem::mainCamera;
    std::vector<CameraSystem::Plane> CameraSystem::planes;
    Scene* CameraSystem::scene;
    float CameraSystem::aspectRatio;

    CameraSystem::CameraSystem(Camera* cam,Scene* sc)
    {
        mainCamera = cam;
        scene = sc;
        unsigned int shaderProgram = ShaderLibrary::GetShader("simple");
        int loc = glGetUniformLocation(shaderProgram,"projection");
        uniformMap["simple_projection"] = loc;
        loc = glGetUniformLocation(shaderProgram,"view");
        uniformMap["simple_view"] = loc;
        loc = glGetUniformLocation(shaderProgram,"color");
        uniformMap["simple_color"] = loc;
        loc = glGetUniformLocation(shaderProgram,"transform");
        uniformMap["simple_transform"] = loc;
        lastX = Application::Get().GetActiveWindow()->GetWidth()/2.0f;
        lastY = Application::Get().GetActiveWindow()->GetHeight()/2.0f;

        shaderProgram = ShaderLibrary::GetShader("pbr");
        
        loc = glGetUniformLocation(shaderProgram,"projection");
        
        uniformMap["pbr_projection"] = loc;
        loc = glGetUniformLocation(shaderProgram,"view");
        
        uniformMap["pbr_view"] = loc;
        loc = glGetUniformLocation(shaderProgram,"camPos");
         
        uniformMap["pbr_camPos"] = loc;

        shaderProgram = ShaderLibrary::GetShader("pbrstatic");
        
        loc = glGetUniformLocation(shaderProgram,"projection");
        
        uniformMap["pbr_projection_static"] = loc;
        loc = glGetUniformLocation(shaderProgram,"view");
        
        uniformMap["pbr_view_static"] = loc;
        loc = glGetUniformLocation(shaderProgram,"camPos");
         
        uniformMap["pbr_camPos_static"] = loc;

        planes.push_back(Plane());
        planes.push_back(Plane());
        planes.push_back(Plane());
        planes.push_back(Plane());
        planes.push_back(Plane());
        planes.push_back(Plane());
        

        



    }

    CameraSystem::~CameraSystem()
    {

    }

    void CameraSystem::OnUpdate(float ts)
    {
       
        deltaTime = ts;

        
        
         if(Input::IsKeyPressed(Key::W))
        {
            

            mainCamera->GetKeyboardInput(Camera::FORWARD,deltaTime);

            
        }

        if(Input::IsKeyPressed(Key::S))
        {
            mainCamera->GetKeyboardInput(Camera::BACKWARD,deltaTime);


        }

        if(Input::IsKeyPressed(Key::A))
        {
            mainCamera->GetKeyboardInput(Camera::LEFT,deltaTime);

            
        }

        if(Input::IsKeyPressed(Key::D))
        {
            mainCamera->GetKeyboardInput(Camera::RIGHT,deltaTime);


        }


        projectionMatrix = mainCamera->GetProjectionMatrix();
        viewMatrix = mainCamera->GetViewMatrix();

        Camera* fCamera = mainCamera;

        glm::vec3 nearNormal = glm::cross(fCamera->frustumPoints[6] - fCamera->frustumPoints[5], fCamera->frustumPoints[8] - fCamera->frustumPoints[5]);
		nearNormal = glm::normalize(nearNormal);
		glm::vec3 farNormal = glm::cross(fCamera->frustumPoints[1] - fCamera->frustumPoints[2], fCamera->frustumPoints[3] - fCamera->frustumPoints[2]);
		farNormal = glm::normalize(farNormal);
		glm::vec3 topNormal = glm::cross(fCamera->frustumPoints[2] - fCamera->frustumPoints[1], fCamera->frustumPoints[5] - fCamera->frustumPoints[1]);
		topNormal = glm::normalize(topNormal);
		glm::vec3 bottomNormal = glm::cross(fCamera->frustumPoints[4] - fCamera->frustumPoints[3], fCamera->frustumPoints[8] - fCamera->frustumPoints[4]);
		bottomNormal = glm::normalize(bottomNormal);
		glm::vec3 leftNormal = glm::cross(fCamera->frustumPoints[5]- fCamera->frustumPoints[1], fCamera->frustumPoints[8] - fCamera->frustumPoints[5]);
		leftNormal = glm::normalize(leftNormal);
		glm::vec3 rightNormal = glm::cross(fCamera->frustumPoints[2] - fCamera->frustumPoints[6], fCamera->frustumPoints[3] - fCamera->frustumPoints[2]);
		rightNormal = glm::normalize(rightNormal);
		float nearD = -1.0f * glm::dot(nearNormal, fCamera->frustumPoints[6]);
		float farD = -1.0f * glm::dot(farNormal,fCamera->frustumPoints[1]);
		float topD = -1.0f * glm::dot(topNormal, fCamera->frustumPoints[2]);
		float bottomD = -1.0f * glm::dot(bottomNormal, fCamera->frustumPoints[4]);
		float leftD = -1.0f * glm::dot(leftNormal, fCamera->frustumPoints[8]);
		float rightD = -1.0f * glm::dot(rightNormal, fCamera->frustumPoints[2]);

		planes[Left].a = leftNormal.x;
		planes[Left].b = leftNormal.y;
		planes[Left].c = leftNormal.z;
		planes[Left].d = leftD;

		planes[Right].a = rightNormal.x;
		planes[Right].b = rightNormal.y;
		planes[Right].c = rightNormal.z;
		planes[Right].d = rightD;

		planes[Top].a = topNormal.x;
		planes[Top].b = topNormal.y;
		planes[Top].c = topNormal.z;
		planes[Top].d = topD;

		planes[Bottom].a = bottomNormal.x;
		planes[Bottom].b = bottomNormal.y;
		planes[Bottom].c = bottomNormal.z;
		planes[Bottom].d = bottomD;

		planes[Near].a = nearNormal.x;
		planes[Near].b = nearNormal.y;
		planes[Near].c = nearNormal.z;
		planes[Near].d = nearD;

		planes[Far].a = farNormal.x;
		planes[Far].b = farNormal.y;
		planes[Far].c = farNormal.z;
		planes[Far].d = farD;

        Camera* editorcam = GameLayer::editorCamera;
		glm::mat4 v = editorcam->GetViewMatrix();
		glm::mat4 p = editorcam->GetProjectionMatrix();

		editorcam->frustumPoints[0] = editorcam->position;
		glm::vec4 result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
	    editorcam->frustumPoints[1] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
		result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		editorcam->frustumPoints[2] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
		result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
        editorcam->frustumPoints[3] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
		result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
		editorcam->frustumPoints[4] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
		result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
		editorcam->frustumPoints[5] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
		result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
		editorcam->frustumPoints[6] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
		result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
		editorcam->frustumPoints[7] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
		result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
		editorcam->frustumPoints[8] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
	    
        if(editorcam->showFrustum)
        {
            

                editorcam->nearPlane.clear();
                editorcam->nearPlane.push_back(editorcam->frustumPoints[5]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[6]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[7]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[8]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[5]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[1]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[2]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[3]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[4]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[1]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[2]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[6]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[7]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[3]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[4]);
                editorcam->nearPlane.push_back(editorcam->frustumPoints[8]);

                
                glBindVertexArray(editorcam->FrustumVAO);
                glBindBuffer(GL_ARRAY_BUFFER,editorcam->FrustumVBO);
                glBufferSubData(GL_ARRAY_BUFFER,0,editorcam->frustumSize*sizeof(glm::vec3),editorcam->nearPlane.data());
                


        }

        auto view = scene->m_registry.view<CameraComponent>();

        for(auto& entity:view)
        {
            auto& cam = scene->m_registry.get<CameraComponent>(entity);

            Camera* camera = cam.camera;

            glm::mat4 v = camera->GetViewMatrix();
			glm::mat4 p = camera->GetProjectionMatrix();

			camera->frustumPoints[0] = camera->position;
			glm::vec4 result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
			camera->frustumPoints[1] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			camera->frustumPoints[2] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
			camera->frustumPoints[3] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
			camera->frustumPoints[4] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
			camera->frustumPoints[5] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
			camera->frustumPoints[6] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			result = glm::inverse(v) * glm::inverse(p) * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
			camera->frustumPoints[7] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			result = glm::inverse(v) * glm::inverse(p) * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
			camera->frustumPoints[8] = glm::vec3(result.x / result.w, result.y / result.w, result.z / result.w);
			
            if(camera->showFrustum)
            {
                camera->nearPlane.clear();
                camera->nearPlane.push_back(camera->frustumPoints[5]);
                camera->nearPlane.push_back(camera->frustumPoints[6]);
                camera->nearPlane.push_back(camera->frustumPoints[7]);
                camera->nearPlane.push_back(camera->frustumPoints[8]);
                camera->nearPlane.push_back(camera->frustumPoints[5]);
                camera->nearPlane.push_back(camera->frustumPoints[1]);
                camera->nearPlane.push_back(camera->frustumPoints[2]);
                camera->nearPlane.push_back(camera->frustumPoints[3]);
                camera->nearPlane.push_back(camera->frustumPoints[4]);
                camera->nearPlane.push_back(camera->frustumPoints[1]);
                camera->nearPlane.push_back(camera->frustumPoints[2]);
                camera->nearPlane.push_back(camera->frustumPoints[6]);
                camera->nearPlane.push_back(camera->frustumPoints[7]);
                camera->nearPlane.push_back(camera->frustumPoints[3]);
                camera->nearPlane.push_back(camera->frustumPoints[4]);
                camera->nearPlane.push_back(camera->frustumPoints[8]);


               
                glBindVertexArray(camera->FrustumVAO);
                glBindBuffer(GL_ARRAY_BUFFER,camera->FrustumVBO);
                glBufferSubData(GL_ARRAY_BUFFER,0,camera->frustumSize*sizeof(glm::vec3),camera->nearPlane.data());
                

            }

        }



        





        
        


    }

    bool CameraSystem::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        
        return true; 

    }

    bool CameraSystem::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        float xpos = e.GetX();
        float ypos = e.GetY();

        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos-lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        if(Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
        {
             mainCamera->ProcessMouseInput(xoffset,yoffset);

             return true;

        }

        

       
        return false;

    }

    bool CameraSystem::OnWindowResizedEvent(WindowResizedEvent& e)
    {
        
        aspectRatio = (float)e.GetWidth()/(float)e.GetHeight();

        mainCamera->UpdateAspectRatio(aspectRatio);

        return true;
    }

    bool CameraSystem::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        mainCamera->ProcessMouseScrollInput(e.GetXOffset(),e.GetYOffset());

        return true;
    }

    glm::vec3 CameraSystem::GetMainCameraPosition()
    {
        return mainCamera->position;
    }

    void CameraSystem::SetMainCamera(Camera* c)
    {
        mainCamera = c;

        auto view = scene->m_registry.view<CameraComponent>();

        for(auto& entity:view)
        {
            auto& c = scene->m_registry.get<CameraComponent>(entity);
            c.camera->primary = false;
        }

        GameLayer::editorCamera->primary = false;
        GameLayer::mainCamera = c;
        GameLayer::mainCamera->UpdateAspectRatio(aspectRatio);

    }

    bool CameraSystem::FrustumCullingTest(glm::vec3 position,float radius)
    {
        float p1 = planes[Near].a * position.x + planes[Near].b * position.y + planes[Near].c * position.z + planes[Near].d+radius;
		float p2 = planes[Far].a * position.x + planes[Far].b * position.y + planes[Far].c * position.z + planes[Far].d+radius;
		float p3 = planes[Left].a * position.x + planes[Left].b * position.y + planes[Left].c * position.z + planes[Left].d+radius;
		float p4 = planes[Right].a * position.x + planes[Right].b * position.y + planes[Right].c * position.z + planes[Right].d+radius;
		float p5 = planes[Top].a * position.x + planes[Top].b * position.y + planes[Top].c * position.z + planes[Top].d+radius;
		float p6 = planes[Bottom].a * position.x + planes[Bottom].b * position.y + planes[Bottom].c * position.z + planes[Bottom].d+radius;


		if (p1 > 0.0f && p2> 0.0f && p5> 0.0f && p6> 0.0f && p3 > 0.0f && p4>0.0f)
		{
			return true;
		}

		
		return false;

    }

    std::unordered_map<std::string,std::vector<uint32_t>> CameraSystem::GetFrustumFilteredMap(std::unordered_map<std::string,std::vector<uint32_t>>& map)
    {
        std::unordered_map<std::string,std::vector<uint32_t>> filteredMap;

        for(auto& item:map)
        {
            for(auto& v:item.second)
            {
                if(scene->m_registry.all_of<MeshComponent,TransformComponent>((entt::entity)v))
                {
                    auto& mc = scene->m_registry.get<MeshComponent>((entt::entity)v);
                    auto& tr = scene->m_registry.get<TransformComponent>((entt::entity)v);
                    float max = 0;

                    if(tr.scale.x>max)
                    {
                        max = tr.scale.x;
                    }
                    if(tr.scale.y>max)
                    {
                        max = tr.scale.y;
                    }
                    if(tr.scale.z>max)
                    {
                        max = tr.scale.z;
                    }

                    float r = mc.radius*max;

                    bool result = FrustumCullingTest(tr.position,r);

                    if(result)
                    {
                        if(filteredMap.count(item.first)==0)
                        {
                            filteredMap[item.first] = std::vector<uint32_t>();
                        }

                        filteredMap[item.first].push_back(v);
                    }


                }
                

                
            }
        }


        return filteredMap;
    }

}