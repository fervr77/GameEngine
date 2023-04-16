#include "TerrainSystem.h"
#include "Components/Components.h"
#include "Systems/CameraSystem.h"
#include <iostream>

namespace qvt
{
    

    TerrainSystem::TerrainSystem(Scene* scene)
    {
        this->scene = scene;

    }

    TerrainSystem::~TerrainSystem()
    {

    }

    void TerrainSystem::Update(float deltaTime)
    {

        auto view = scene->m_registry.view<TerrainComponent>();

        glm::vec3 camPos = CameraSystem::GetMainCameraPosition();

        for(auto& entity:view)
        {
            auto& tc = scene->m_registry.get<TerrainComponent>(entity);
            auto& tr = scene->m_registry.get<TransformComponent>(entity);

            tc.quadTree.Update(camPos,tr.position);


        }

       
        

       

    }


}