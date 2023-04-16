#include "Test.h"
#include "Applications/App1/DataComponents/DataComponents.h"
#include "Components/Components.h"
#include <iostream>

namespace qvt
{
    Test::~Test()
    {
        OnDestroy();
    }

    void Test::OnCreate()
    {
      
        auto& data = m_entity.AddComponent<TestDataComponent>();
        auto& g = m_entity.GetComponent<Live2DGraphComponent>();
        g.AddGraph("Rotation speed",&data.rotSpeed,"Speed");
        


        data.rotSpeed = 12;
    }

    void Test::OnUpdate(float ts)
    {
        auto& data = m_entity.GetComponent<TestDataComponent>();

        auto& tc = m_entity.GetComponent<TransformComponent>();

        float y;

        y = data.rotSpeed*data.totalTime;
        glm::vec3 deltaRotation = glm::vec3(tc.rotation.x,y,tc.rotation.z)-tc.rotation;
        tc.rotation.x += deltaRotation.x;
        tc.rotation.y += deltaRotation.y;
        tc.rotation.z += deltaRotation.z;

    

        data.totalTime += ts;

    }

    void Test::OnDestroy()
    {
        

         auto& g = m_entity.GetComponent<Live2DGraphComponent>();
         g.RemoveGraph("Rotation speed","Speed");
        m_entity.RemoveComponent<TestDataComponent>();
       
    }

    void Test::OnImGuiRender()
    {
        auto& data = m_entity.GetComponent<TestDataComponent>();
       

        ImGui::DragFloat("Rotation speed",&data.rotSpeed);
       

    }

    

}