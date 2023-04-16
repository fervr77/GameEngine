#include "ScriptSystem.h"
#include "Scene/Scene.h"
#include "Components/Components.h"

namespace qvt
{
    std::unordered_map<std::string,std::function<ScriptableEntity*()>> ScriptSystem::map;
    std::vector<std::string> ScriptSystem::scriptList;
    float ScriptSystem::prevTimeStep = 0.0f;

    ScriptSystem::ScriptSystem(Scene* sc)
    {
        scene = sc;
       
    }

    ScriptSystem::~ScriptSystem()
    {
        map.clear();
        scriptList.clear();
    }

    void ScriptSystem::Update(float ts)
    {
        if(ts>100*prevTimeStep && prevTimeStep != 0.0f)
        {
            ts = prevTimeStep;
        }

        scene->m_registry.view<ScriptComponent>().each([=](auto entity,auto& sc)
        {
            for(auto& script:sc.scripts)
            {
                if(!script.Instance)
                {
                    if(strcmp(script.behaviorName.c_str(),"")==0)
                    {
                        return;
                    }

                    script.InstantiateScript = map[script.behaviorName];
                    script.Instance = script.InstantiateScript();
                    script.Instance->m_entity = Entity{entity,scene};
                    script.Instance->scene = scene;
                    script.Instance->OnCreate();
                }

                script.Instance->OnUpdate(ts);
            }

        });


    }


}