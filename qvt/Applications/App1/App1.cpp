#define GLM_ENABLE_EXPERIMENTAL
#include "App1.h"
#include "Components/Components.h"
#include <functional>
#include "Core/Logger.h"
#include <iostream>
#include "Core/Application.h"
#include "Systems/CameraSystem.h"
#include <cstdlib>
#include "Core/Input.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "imgui.h"
#include "Systems/ScriptSystem.h"
#include "Applications/App1/Scripts/Test.h"
#include "Systems/CollisionSystem.h"




namespace qvt
{
    
    

    App1::App1(Scene* sc):Layer("App1")
    {
         scene = sc;
         
                         
    }

    App1::~App1()
    {
        
    }

    void App1::OnAttach()
    {
        ScriptSystemInit();
        
        Entity cube = scene->CreateEntity("hoverbike");
        cube.AddComponent<MeshComponent>("hoverbike");
        cube.AddComponent<PbrRendererComponent>("hoverbike",(uint32_t)cube.GetEntityID());
        cube.AddComponent<OBBoxComponent>("hoverbike");

        Entity box = scene->CreateEntity("box");
        box.AddComponent<MeshComponent>("cube");
        box.AddComponent<PbrRendererComponent>("green",(uint32_t)box.GetEntityID());
        box.AddComponent<CapsuleBVComponent>("cube");

        

        

        Entity camera = scene->CreateEntity("camera");
        auto& c = camera.AddComponent<CameraComponent>(glm::vec3(0,10,0));
        c.camera->SetAsPrimary();


        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++)
            {
                Entity cube = scene->CreateEntity("tile");
                cube.AddComponent<MeshComponent>("cube");

                if((i+j)%2==0)
                {
                    cube.AddComponent<PbrRendererComponent>("red",(uint32_t)cube.GetEntityID());

                }
                else
                {
                    cube.AddComponent<PbrRendererComponent>("white_metallic",(uint32_t)cube.GetEntityID());
                }

                auto& tr = cube.GetComponent<TransformComponent>();

                tr.position = glm::vec3(i*2,-1,j*2);
                tr.scale = glm::vec3(2,0.2,2);

                cube.AddComponent<AABBoxComponent>("cube");
                tr.isStatic = true;
            }
        }

        
    
        
    }

    void App1::SetMainCameraAspectRatio(float ar)
    {
        
    }
        
    

    void App1::OnDetach()
    {
        
    }

    void App1::OnViewportResizeEvent()
    {
        
    }

    void App1::OnUpdate(float ts)
    {
        
        
 
        
        
    }

    void App1::OnEvent(Event& event)
    {
        EventHandler handler(event);
        handler.Handle<KeyPressedEvent>(std::bind(&App1::OnKeyPressedEvent,this,std::placeholders::_1));
        handler.Handle<MouseButtonPressedEvent>(std::bind(&App1::OnMousePressedEvent,this,std::placeholders::_1));
        handler.Handle<MouseMovedEvent>(std::bind(&App1::OnMouseMovedEvent,this,std::placeholders::_1));
        
    }

    void App1::OnImGuiRender()
    {
        
           

    }

    bool App1::OnKeyPressedEvent(KeyPressedEvent& e)
    {

       
          

        
        
        return false;
    }

    bool App1::OnMousePressedEvent(MouseButtonPressedEvent& e)
    {
        
         

        return false;
    }

    bool App1::OnMouseMovedEvent(MouseMovedEvent& e)
    {
       

       
        
        

        return false;
    }

    void App1::ScriptSystemInit()
    {
        ScriptSystem::map["Test"] = [](){return static_cast<ScriptableEntity*>(new Test());};
        ScriptSystem::scriptList.push_back("Test");

    }

    

    


}