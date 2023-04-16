#include <glad/glad.h>
#include "LightSystem.h"
#include "Renderer/ShaderLibrary.h"
#include <iostream>

namespace qvt
{
    LightSystem::PointLight LightSystem::pointLight;
    std::unordered_map<std::string,int> LightSystem::lightUniformMap;

    LightSystem::LightSystem()
    {
        pointLight.position = glm::vec3(20,20,10);
        pointLight.color = glm::vec3(300.0f,300.0f,300.0f);

        unsigned int shaderProgram = ShaderLibrary::GetShader("pbr");
        glUseProgram(shaderProgram);
        int loc = glGetUniformLocation(shaderProgram,"lightPositions[0]");
        
        lightUniformMap["light_position"] = loc;
        loc = glGetUniformLocation(shaderProgram,"lightColors[0]");
        
        lightUniformMap["light_color"] = loc;

        glUniform3f(lightUniformMap["light_position"],pointLight.position.x,pointLight.position.y,pointLight.position.z);
        glUniform3f(lightUniformMap["light_color"],pointLight.color.r,pointLight.color.g,pointLight.color.b);

        shaderProgram = ShaderLibrary::GetShader("pbrstatic");
        glUseProgram(shaderProgram);
        loc = glGetUniformLocation(shaderProgram,"lightPositions[0]");
        
        lightUniformMap["light_position_static"] = loc;
        loc = glGetUniformLocation(shaderProgram,"lightColors[0]");
        
        lightUniformMap["light_color_static"] = loc;

        glUniform3f(lightUniformMap["light_position_static"],pointLight.position.x,pointLight.position.y,pointLight.position.z);
        glUniform3f(lightUniformMap["light_color_static"],pointLight.color.r,pointLight.color.g,pointLight.color.b);



    }

    LightSystem::~LightSystem()
    {

    }

    void LightSystem::OnUpdate(float ts)
    {

    }
}