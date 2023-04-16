#define GLM_ENABLE_EXPERIMENTAL
#include "RendererSystem.h"
#include <glad/glad.h>
#include "Renderer/ShaderLibrary.h"
#include "Renderer/ShaderLoader.h"
#include "Renderer/MeshLibrary.h"
#include "Renderer/TextureLibrary.h"
#include "Events/ApplicationEvents.h"
#include "Systems/CameraSystem.h"
#include "Systems/LightSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Components/Components.h"
#include <iostream>
#include "stb_image.h"
#include "Renderer/MeshLoader.h"
#include "Game/GameLayer.h"

namespace qvt
{
    std::unordered_map<std::string,std::vector<uint32_t>> RendererSystem::simpleRenderMap;
    std::unordered_map<std::string,int> RendererSystem::simpleUniformMap;
    std::unordered_map<std::string,int> RendererSystem::primitiveUniformMap;
    std::unordered_map<std::string,std::vector<uint32_t>> RendererSystem::pbrRenderMap;
    std::unordered_map<std::string,std::vector<uint32_t>> RendererSystem::pbrStaticRenderMap;
    std::unordered_map<std::string,int> RendererSystem::pbrUniformMap;
    std::unordered_map<std::string,int> RendererSystem::pbrStaticUniformMap;
    const unsigned int RendererSystem::batchSize;
    glm::mat4 RendererSystem::transforms[batchSize];
    bool RendererSystem::renderEnvMap = false;
    bool RendererSystem::renderBVs = false;
    bool RendererSystem::renderPrimitives = false;
    unsigned int RendererSystem::viewFrustumCounter = 0;
    std::unordered_map<std::string,std::vector<std::vector<uint32_t>>> RendererSystem::staticMap;
    std::unordered_map<std::string,std::vector<std::pair<unsigned int,std::pair<unsigned int,unsigned int>>>> RendererSystem::staticPreparedMap;
   
   
    RendererSystem::RendererSystem(Scene* scene):m_scene(scene)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        
        glDepthFunc(GL_LEQUAL);
        

        ShaderLibrary::shaderMap["background"] = ShaderLoader::Load("background",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["brdf"] = ShaderLoader::Load("brdf",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["equitocube"] = ShaderLoader::Load("equitocube",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["irradiance"] = ShaderLoader::Load("irradiance",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["pbr"] = ShaderLoader::Load("pbr",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["pbrstatic"] = ShaderLoader::Load("pbrstatic",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["prefilter"] = ShaderLoader::Load("prefilter",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["simple"] = ShaderLoader::Load("simple",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["terrain"] = ShaderLoader::Load("terrain",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT | ShaderLoader::TESSCONTROL | ShaderLoader::TESSEVAL | ShaderLoader::GEOMETRY);
        ShaderLibrary::shaderMap["primitive"] = ShaderLoader::Load("primitive",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);
        ShaderLibrary::shaderMap["camera"] = ShaderLoader::Load("camera",ShaderLoader::VERTEX|ShaderLoader::FRAGMENT);

        MeshLibrary::meshList.push_back("cube");
        MeshLibrary::meshList.push_back("hoverbike");
        MeshLibrary::meshList.push_back("uvsphere");
        MeshLibrary::meshList.push_back("tree");
        MeshLibrary::meshList.push_back("cylinder");
        MeshLibrary::meshList.push_back("monkey");
        MeshLibrary::meshList.push_back("s0");
        MeshLibrary::meshList.push_back("s1");
        MeshLibrary::meshList.push_back("s2");
        MeshLibrary::meshList.push_back("hovercar");
        MeshLibrary::meshList.push_back("tsuga");
        MeshLibrary::meshList.push_back("statue");
        MeshLibrary::meshList.push_back("snakeStatue");
        MeshLibrary::meshList.push_back("girl");
        MeshLibrary::meshList.push_back("bunny");
        MeshLibrary::meshList.push_back("teapot");
        MeshLibrary::meshList.push_back("cone");
        MeshLibrary::meshList.push_back("torus");
        MeshLibrary::meshList.push_back("arrow");
        
        
        unsigned int shaderProgram = ShaderLibrary::GetShader("equitocube");


        shaderProgram = ShaderLibrary::GetShader("background");
        int loc = glGetUniformLocation(shaderProgram,"environmentMap");
        glUniform1i(loc,0);
        pbrUniformMap["background_environmentMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"projection");
        pbrUniformMap["background_projection"] = loc;
        loc = glGetUniformLocation(shaderProgram,"view");
        pbrUniformMap["background_view"] = loc;


        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1,&captureFBO);
        glGenRenderbuffers(1,&captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,512,512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,captureRBO);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nrComponents;
        const std::string path = "../../textures/hdr/hamarikyu.hdr";

        float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);

        
        if (data)
        {
            glGenTextures(1, &hdrTexture);
            glBindTexture(GL_TEXTURE_2D, hdrTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load HDR image." << std::endl;
        }

       
        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
        // ----------------------------------------------------------------------------------------------
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        // pbr: convert HDR equirectangular environment map to cubemap equivalent

        shaderProgram = ShaderLibrary::GetShader("equitocube");

        glUseProgram(shaderProgram);
        loc = glGetUniformLocation(shaderProgram,"equirectangularMap");
        glUniform1i(loc,0);
        loc = glGetUniformLocation(shaderProgram,"projection");
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(captureProjection));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            int loc = glGetUniformLocation(shaderProgram,"view");
            glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            RenderCube();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

                
            glGenTextures(1, &irradianceMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

            // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
            // -----------------------------------------------------------------------------
            shaderProgram = ShaderLibrary::GetShader("irradiance");
            glUseProgram(shaderProgram);
            loc = glGetUniformLocation(shaderProgram,"environmentMap");
            glUniform1i(loc,0);
            loc = glGetUniformLocation(shaderProgram,"projection");
            glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(captureProjection));
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

            glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                int loc = glGetUniformLocation(shaderProgram,"view");
                glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(captureViews[i]));
                
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                RenderCube();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
            // --------------------------------------------------------------------------------
            
            glGenTextures(1, &prefilterMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

            // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
            // ----------------------------------------------------------------------------------------------------
            shaderProgram = ShaderLibrary::GetShader("prefilter");
            glUseProgram(shaderProgram);
            loc = glGetUniformLocation(shaderProgram,"environmentMap");
            glUniform1i(loc,0);
            loc = glGetUniformLocation(shaderProgram,"projection");
            glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(captureProjection));
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            unsigned int maxMipLevels = 5;
            for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
            {
                // reisze framebuffer according to mip-level size.
                unsigned int mipWidth  = 128 * std::pow(0.5, mip);
                unsigned int mipHeight = 128 * std::pow(0.5, mip);
                glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float)mip / (float)(maxMipLevels - 1);
                int loc = glGetUniformLocation(shaderProgram,"roughness");
                glUniform1f(loc,roughness);
                
                for (unsigned int i = 0; i < 6; ++i)
                {
                    int loc = glGetUniformLocation(shaderProgram,"view");
                    glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(captureViews[i]));
                    
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    RenderCube();
                }
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // pbr: generate a 2D LUT from the BRDF equations used.
            // ----------------------------------------------------
            
            glGenTextures(1, &brdfLUTTexture);

            // pre-allocate enough memory for the LUT texture.
            glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
            // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

            glViewport(0, 0, 512, 512);
            shaderProgram = ShaderLibrary::GetShader("brdf");
            glUseProgram(shaderProgram);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderQuad();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);










         
        
        shaderProgram = ShaderLibrary::GetShader("pbr");
        
        MeshLibrary::GetMesh("cube");
        TextureLibrary::GetTexture("floortiles.png");

        glUseProgram(shaderProgram);
        loc = glGetUniformLocation(shaderProgram,"transform");
       
        pbrUniformMap["pbr_transform"] = loc;
        loc = glGetUniformLocation(shaderProgram,"albedo");
       
        pbrUniformMap["pbr_albedo"] = loc;
        loc = glGetUniformLocation(shaderProgram,"roughness");
        
        pbrUniformMap["pbr_roughness"] = loc;
        loc = glGetUniformLocation(shaderProgram,"metallic");
       
        pbrUniformMap["pbr_metallic"] = loc;
        loc = glGetUniformLocation(shaderProgram,"ao");
       
        pbrUniformMap["pbr_ao"] = loc;

        loc = glGetUniformLocation(shaderProgram,"irradianceMap");
        glUniform1i(loc,0);
        pbrUniformMap["pbr_irrandianceMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"prefilterMap");
        glUniform1i(loc,1);
        pbrUniformMap["pbr_prefilterMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"brdfLUT");
        glUniform1i(loc,2);
        pbrUniformMap["pbr_brdf"] = loc;
        loc = glGetUniformLocation(shaderProgram,"albedoMap");
        glUniform1i(loc,3);
        pbrUniformMap["pbr_albedoMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"metallicMap");
        glUniform1i(loc,4);
        pbrUniformMap["pbr_metallicMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"roughnessMap");
        glUniform1i(loc,5);
        pbrUniformMap["pbr_roughnessMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"aoMap");
        glUniform1i(loc,6);
        pbrUniformMap["pbr_aoMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"normalMap");
        glUniform1i(loc,7);
        pbrUniformMap["pbr_normalMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"emissiveMap");
        glUniform1i(loc,8);
        pbrUniformMap["pbr_emissiveMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"useNormalMap");
        pbrUniformMap["pbr_useNormalMap"] = loc;

        loc = glGetUniformLocation(shaderProgram,"entityID");
        pbrUniformMap["pbr_entityID"] = loc;

        glBindVertexArray(0);

        auxVBO.push_back(0);
        auxVBO.push_back(0);

        glGenBuffers(1,&auxVBO[0]);
        glBindBuffer(GL_ARRAY_BUFFER,auxVBO[0]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4)*1024,NULL,GL_DYNAMIC_DRAW);
        glGenBuffers(1,&auxVBO[1]);
        glBindBuffer(GL_ARRAY_BUFFER,auxVBO[1]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4)*1024,NULL,GL_DYNAMIC_DRAW);


        shaderProgram = ShaderLibrary::GetShader("pbrstatic");
        glUseProgram(shaderProgram);
        
       
        
        loc = glGetUniformLocation(shaderProgram,"albedo");
       
        pbrStaticUniformMap["pbr_albedo"] = loc;
        loc = glGetUniformLocation(shaderProgram,"roughness");
        
        pbrStaticUniformMap["pbr_roughness"] = loc;
        loc = glGetUniformLocation(shaderProgram,"metallic");
       
        pbrStaticUniformMap["pbr_metallic"] = loc;
        loc = glGetUniformLocation(shaderProgram,"ao");
       
        pbrStaticUniformMap["pbr_ao"] = loc;

        loc = glGetUniformLocation(shaderProgram,"irradianceMap");
        glUniform1i(loc,0);
        pbrStaticUniformMap["pbr_irrandianceMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"prefilterMap");
        glUniform1i(loc,1);
        pbrStaticUniformMap["pbr_prefilterMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"brdfLUT");
        glUniform1i(loc,2);
        pbrStaticUniformMap["pbr_brdf"] = loc;
        loc = glGetUniformLocation(shaderProgram,"albedoMap");
        glUniform1i(loc,3);
        pbrStaticUniformMap["pbr_albedoMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"metallicMap");
        glUniform1i(loc,4);
        pbrStaticUniformMap["pbr_metallicMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"roughnessMap");
        glUniform1i(loc,5);
        pbrStaticUniformMap["pbr_roughnessMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"aoMap");
        glUniform1i(loc,6);
        pbrStaticUniformMap["pbr_aoMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"normalMap");
        glUniform1i(loc,7);
        pbrStaticUniformMap["pbr_normalMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"emissiveMap");
        glUniform1i(loc,8);
        pbrStaticUniformMap["pbr_emissiveMap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"useNormalMap");
        pbrStaticUniformMap["pbr_useNormalMap"] = loc;

        loc = glGetUniformLocation(shaderProgram,"entityID");
        pbrStaticUniformMap["pbr_entityID"] = loc;

        shaderProgram = ShaderLibrary::GetShader("terrain");
        loc = glGetUniformLocation(shaderProgram,"projection");
        pbrUniformMap["terrain_projection"] = loc;
        loc = glGetUniformLocation(shaderProgram,"view");
        pbrUniformMap["terrain_view"] = loc;
        loc = glGetUniformLocation(shaderProgram,"transform");
        pbrUniformMap["terrain_transform"] = loc;
        loc = glGetUniformLocation(shaderProgram,"localMatrix");
        pbrUniformMap["terrain_localMatrix"] = loc;
        loc = glGetUniformLocation(shaderProgram,"index");
        pbrUniformMap["terrain_index"] = loc;
        loc = glGetUniformLocation(shaderProgram,"gap");
        pbrUniformMap["terrain_gap"] = loc;
        loc = glGetUniformLocation(shaderProgram,"location");
        pbrUniformMap["terrain_location"] = loc;
        loc = glGetUniformLocation(shaderProgram,"lod");
        pbrUniformMap["terrain_lod"] = loc;
        loc = glGetUniformLocation(shaderProgram,"scaleY");
        pbrUniformMap["terrain_scaleY"] = loc;
        loc = glGetUniformLocation(shaderProgram,"cameraPosition");
        pbrUniformMap["terrain_cameraPosition"] = loc;

        std::string prefix = "lod_morph_area[";
        std::string sufix = "]";

        for(int i=0;i<8;i++)
        {
            int loc = glGetUniformLocation(shaderProgram,(prefix+std::to_string(i)+sufix).c_str());
            pbrUniformMap["terrain_morph_"+std::to_string(i)] = loc;


        }

        loc = glGetUniformLocation(shaderProgram,"tessellationFactor");
        pbrUniformMap["terrain_tessellationFactor"] = loc;
        loc = glGetUniformLocation(shaderProgram,"tessellationSlope");
        pbrUniformMap["terrain_tessellationSlope"] = loc;
        loc = glGetUniformLocation(shaderProgram,"tessellationShift");
        pbrUniformMap["terrain_tessellationShift"] = loc;
        loc = glGetUniformLocation(shaderProgram,"heightmap");
        pbrUniformMap["terrain_heightmap"] = loc;

        shaderProgram = ShaderLibrary::GetShader("simple");
        loc = glGetUniformLocation(shaderProgram,"transform");
        simpleUniformMap["simple_transform"] = loc;
        loc = glGetUniformLocation(shaderProgram,"color");
        simpleUniformMap["simple_color"] = loc;
        loc = glGetUniformLocation(shaderProgram,"projection");
        simpleUniformMap["simple_projection"] = loc;
        loc = glGetUniformLocation(shaderProgram,"view");
        simpleUniformMap["simple_view"] = loc;

        shaderProgram = ShaderLibrary::GetShader("primitive");
        loc = glGetUniformLocation(shaderProgram,"transform");
       
        primitiveUniformMap["primitive_transform"] = loc;
        loc = glGetUniformLocation(shaderProgram,"color");
        
        primitiveUniformMap["primitive_color"] = loc;
        loc = glGetUniformLocation(shaderProgram,"entityID");
        
        primitiveUniformMap["primitive_entityID"] = loc;
        loc = glGetUniformLocation(shaderProgram,"projection");
        
        primitiveUniformMap["primitive_projection"] = loc;
        loc = glGetUniformLocation(shaderProgram,"view");
       
        primitiveUniformMap["primitive_view"] = loc;


       

        glEnable(GL_CULL_FACE);
       

    }

    RendererSystem::~RendererSystem()
    {
        
    }

    void RendererSystem::OnUpdate(float ts)
    {
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        unsigned int shaderProgram;
        glm::vec3 lightPosition = LightSystem::pointLight.position;
        glm::vec3 lightColor = LightSystem::pointLight.color;
        glm::vec3 camPosition = CameraSystem::GetMainCameraPosition();

        
        //std::unordered_map<std::string,std::vector<std::vector<uint32_t>>> renderMap = GetBatchedMap();
        
        /*for(auto& batch:renderMap)
        {
            MaterialLibrary::PbrMaterial material = materialLibrary.GetPbrMaterial(batch.first);
            glUniform3f(pbrUniformMap["pbr_albedo"],material.albedo.r,material.albedo.g,material.albedo.b);
            glUniform1f(pbrUniformMap["pbr_metallic"],material.metallic);
            glUniform1f(pbrUniformMap["pbr_roughness"],material.roughness);
            glUniform1f(pbrUniformMap["pbr_ao"],material.ao);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D,material.albedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D,material.metallicMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D,material.roughnessMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D,material.aoMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D,material.normalMap);
            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_2D,material.emissiveMap);
            glUniform1i(pbrUniformMap["pbr_useNormalMap"],material.normalMap!=TextureLibrary::GetWhiteTexture());

            

            for(auto& v:batch.second)
            {
                
                int N = v.size()/batchSize;
                int r = v.size()%batchSize;

                int vboIndex = 0;
                int vboSize = auxVBO.size();

                
                

                auto& mc = m_scene->m_registry.get<MeshComponent>((entt::entity)v[0]);

                glBindVertexArray(mc.VAO);
               
                
                

                for(int i=0;i<N;i++)
                {

                   
                        
                    
                    for(int j=0;j<batchSize;j++)
                    {
                        
                        auto& tc = m_scene->m_registry.get<TransformComponent>((entt::entity)v[j+i*batchSize]);

                        glm::mat4 transform = glm::mat4(1.0f);
                        transform = glm::translate(transform,tc.position);
                        transform = glm::rotate(transform,tc.angle,tc.rotation);
                        transform = glm::scale(transform,tc.scale);

                        transforms[j] = transform;

                        
                    } 

                    
                    glBindBuffer(GL_ARRAY_BUFFER,auxVBO[vboIndex]);
                    glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)0);
                    glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(1*sizeof(glm::vec4)));
                    glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(2*sizeof(glm::vec4)));
                    glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(3*sizeof(glm::vec4)));
                    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::mat4)*batchSize,transforms);

                    glDrawElementsInstanced(GL_TRIANGLES,mc.numIndices,GL_UNSIGNED_INT,nullptr,batchSize);

                    
                    
                    vboIndex = (vboIndex+1)%vboSize;

               
                }

               

                // remaining objects

                for(int j=0;j<r;j++)
                {
                    
                    auto& tc = m_scene->m_registry.get<TransformComponent>((entt::entity)v[j+N*batchSize]);

                    glm::mat4 transform = glm::mat4(1.0f);
                    transform = glm::translate(transform,tc.position);
                    transform = glm::rotate(transform,tc.angle,tc.rotation);
                    transform = glm::scale(transform,tc.scale);

                    transforms[j] = transform;

                    
                    

                } 



               
                
                glBindBuffer(GL_ARRAY_BUFFER,auxVBO[vboIndex]);
                glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)0);
                glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(1*sizeof(glm::vec4)));
                glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(2*sizeof(glm::vec4)));
                glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(3*sizeof(glm::vec4)));
                glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::mat4)*r,transforms);

                
                glDrawElementsInstanced(GL_TRIANGLES,mc.numIndices,GL_UNSIGNED_INT,nullptr,r);

                
            }


            

            

        }*/

        std::unordered_map<std::string,std::vector<uint32_t>> filteredMap = CameraSystem::GetFrustumFilteredMap(pbrRenderMap);

        
        bool firstIteration = true;

        for(auto& entity:filteredMap)
        {
            if(firstIteration)
            {
                shaderProgram = ShaderLibrary::GetShader("pbr");
                glUseProgram(shaderProgram);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D,brdfLUTTexture);
                


                
                
                
                glUniformMatrix4fv(CameraSystem::uniformMap["pbr_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                glUniformMatrix4fv(CameraSystem::uniformMap["pbr_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                glUniform3f(CameraSystem::uniformMap["pbr_camPos"],camPosition.x,camPosition.y,camPosition.z);

               
                glUniform3f(LightSystem::lightUniformMap["light_position"],lightPosition.x,lightPosition.y,lightPosition.z);
                glUniform3f(LightSystem::lightUniformMap["light_color"],lightColor.r,lightColor.g,lightColor.b);
                firstIteration = false;

            }

            MaterialLibrary::PbrMaterial material = *materialLibrary.GetPbrMaterial(entity.first);
            glUniform3f(pbrUniformMap["pbr_albedo"],material.albedo.r,material.albedo.g,material.albedo.b);
            glUniform1f(pbrUniformMap["pbr_metallic"],material.metallic);
            glUniform1f(pbrUniformMap["pbr_roughness"],material.roughness);
            glUniform1f(pbrUniformMap["pbr_ao"],material.ao);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D,material.albedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D,material.metallicMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D,material.roughnessMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D,material.aoMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D,material.normalMap);
            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_2D,material.emissiveMap);
            glUniform1i(pbrUniformMap["pbr_useNormalMap"],material.normalMap!=TextureLibrary::GetWhiteTexture());

                
            for(auto& v:entity.second)
            {
                

                auto& tc = m_scene->m_registry.get<TransformComponent>((entt::entity)v);
                
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform,tc.position);
                glm::mat4 rot = glm::toMat4(glm::quat(tc.rotation));
                transform = transform*rot;
                transform = glm::scale(transform,tc.scale);

                glUniformMatrix4fv(pbrUniformMap["pbr_transform"],1,GL_FALSE,glm::value_ptr(transform));
                glUniform1i(pbrUniformMap["pbr_entityID"],v+1);

                if(m_scene->m_registry.all_of<MeshComponent>((entt::entity)v))
                {
                    auto& mc = m_scene->m_registry.get<MeshComponent>((entt::entity)v);
                
                    glBindVertexArray(mc.VAO);
                    glDrawElements(GL_TRIANGLES,mc.numIndices,GL_UNSIGNED_INT,nullptr);

                        
                }

                

            }


        }


        
        firstIteration = true;

        for(auto& batch:staticPreparedMap)
        {
            if(firstIteration)
            {
                shaderProgram = ShaderLibrary::GetShader("pbrstatic");
                glUseProgram(shaderProgram);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP,prefilterMap);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D,brdfLUTTexture);

                glUniformMatrix4fv(CameraSystem::uniformMap["pbr_projection_static"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                glUniformMatrix4fv(CameraSystem::uniformMap["pbr_view_static"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                glUniform3f(CameraSystem::uniformMap["pbr_camPos_static"],camPosition.x,camPosition.y,camPosition.z);

                
                glUniform3f(LightSystem::lightUniformMap["light_position_static"],lightPosition.x,lightPosition.y,lightPosition.z);
                glUniform3f(LightSystem::lightUniformMap["light_color_static"],lightColor.r,lightColor.g,lightColor.b);
                firstIteration = false;

            }


            MaterialLibrary::PbrMaterial material = *materialLibrary.GetPbrMaterial(batch.first);
            glUniform3f(pbrStaticUniformMap["pbr_albedo"],material.albedo.r,material.albedo.g,material.albedo.b);
            glUniform1f(pbrStaticUniformMap["pbr_metallic"],material.metallic);
            glUniform1f(pbrStaticUniformMap["pbr_roughness"],material.roughness);
            glUniform1f(pbrStaticUniformMap["pbr_ao"],material.ao);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D,material.albedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D,material.metallicMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D,material.roughnessMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D,material.aoMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D,material.normalMap);
            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_2D,material.emissiveMap);
            glUniform1i(pbrStaticUniformMap["pbr_useNormalMap"],material.normalMap!=TextureLibrary::GetWhiteTexture());

            for(auto& v:batch.second)
            {
                glBindVertexArray(v.first);
                glDrawElementsInstanced(GL_TRIANGLES,v.second.first,GL_UNSIGNED_INT,NULL,v.second.second);
            }


        }

        
        auto view = m_scene->m_registry.view<TerrainComponent>();

        firstIteration = true;

        for(auto& entity:view)
        {
            if(firstIteration)
            {
                shaderProgram = ShaderLibrary::GetShader("terrain");

                glUseProgram(shaderProgram);
                glUniformMatrix4fv(pbrUniformMap["terrain_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                glUniformMatrix4fv(pbrUniformMap["terrain_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                glUniform3f(pbrUniformMap["terrain_cameraPosition"],camPosition.x,camPosition.y,camPosition.z);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,TextureLibrary::GetTexture("hm0.bmp"));
                glUniform1i(pbrUniformMap["terrain_heightmap"],0);

                firstIteration = false;

            }

            auto& tc = m_scene->m_registry.get<TransformComponent>(entity);
            auto& ttc = m_scene->m_registry.get<TerrainComponent>(entity);
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform,tc.position);
            transform = glm::scale(transform,ttc.quadTree.GetScaleVector());




            glUniformMatrix4fv(pbrUniformMap["terrain_transform"],1,GL_FALSE,glm::value_ptr(transform));
            

            for(int i=0;i<8;i++)
                {
                    glUniform1i(pbrUniformMap["terrain_morph_"+std::to_string(i)],ttc.quadTree.lod_morph[i]);
                }

            glUniform1i(pbrUniformMap["terrain_tessellationFactor"],ttc.quadTree.tessellationFactor);
            glUniform1f(pbrUniformMap["terrain_tessellationSlope"],ttc.quadTree.tessellationSlope);
            glUniform1f(pbrUniformMap["terrain_tessellationShift"],ttc.quadTree.tessellationShift);
            
            ttc.quadTree.Render();
        }

        // Render the camera frustums 
        if(viewFrustumCounter>0)
        {
            unsigned int shaderProgram = ShaderLibrary::GetShader("simple");
            glUseProgram(shaderProgram);
            glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
            glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));

            if(GameLayer::editorCamera->showFrustum)
            {
                glUniform3f(simpleUniformMap["simple_color"],GameLayer::editorCamera->frustumColor.r,GameLayer::editorCamera->frustumColor.g,GameLayer::editorCamera->frustumColor.b);
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform,GameLayer::editorCamera->position);
                glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(glm::mat4(1.0f)));
                glBindVertexArray(GameLayer::editorCamera->FrustumVAO);
                glDrawArrays(GL_LINE_STRIP,0,GameLayer::editorCamera->frustumSize);

            }

            auto view = m_scene->m_registry.view<CameraComponent>();

            for(auto& entity:view)
            {
                auto& cam = m_scene->m_registry.get<CameraComponent>(entity);

                if(cam.camera->showFrustum)
                {
                     glUniform3f(simpleUniformMap["simple_color"],cam.camera->frustumColor.r,cam.camera->frustumColor.g,cam.camera->frustumColor.b);
                     glm::mat4 transform = glm::mat4(1.0f);
                     transform = glm::translate(transform,cam.camera->position);
                     glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(glm::mat4(1.0f)));
                     glBindVertexArray(cam.camera->FrustumVAO);
                     glDrawArrays(GL_LINE_STRIP,0,cam.camera->frustumSize);

                }
            }


        }

        auto circleView = m_scene->m_registry.view<CircleShapeComponent>();

        firstIteration = true;
        bool simpleShaderUniformSet = false;

        for(auto& entity:circleView)
        {
            if(firstIteration)
            {
                shaderProgram = ShaderLibrary::GetShader("simple");
                glUseProgram(shaderProgram);
                glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                firstIteration = false;
                simpleShaderUniformSet = true;
            }

            auto& shape = m_scene->m_registry.get<CircleShapeComponent>(entity);

            glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

            auto& tr = m_scene->m_registry.get<TransformComponent>(entity);

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform,tr.position);
            glm::mat4 rot = glm::toMat4(glm::quat(tr.rotation));
            transform = transform*rot;
            transform = glm::scale(transform,tr.scale);

            glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

            glBindVertexArray(shape.VAO);
            glDrawArrays(GL_LINE_STRIP,0,shape.numVertices);



        }

        auto triShapeView = m_scene->m_registry.view<TriangleShapeComponent>();

        firstIteration  = true;

        for(auto& entity:triShapeView)
        {
            if(firstIteration && simpleShaderUniformSet==false)
            {
                shaderProgram = ShaderLibrary::GetShader("simple");
                glUseProgram(shaderProgram);
                glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                firstIteration = false;
                simpleShaderUniformSet = true;

            }

            auto& shape = m_scene->m_registry.get<TriangleShapeComponent>(entity);

            glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

            auto& tr = m_scene->m_registry.get<TransformComponent>(entity);

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform,tr.position);
            glm::mat4 rot = glm::toMat4(glm::quat(tr.rotation));
            transform = transform*rot;
            transform = glm::scale(transform,tr.scale);

            glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

            glBindVertexArray(shape.VAO);
            glDrawArrays(GL_LINE_STRIP,0,shape.numVertices);



        }

       
       if(renderBVs)
       {

            auto AABBView = m_scene->m_registry.view<AABBoxComponent>();

            firstIteration = true;

            for(auto& entity:AABBView)
            {
                if(firstIteration && simpleShaderUniformSet==false)
                {
                    shaderProgram = ShaderLibrary::GetShader("simple");
                    glUseProgram(shaderProgram);
                    glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                    glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                    firstIteration = false;
                    simpleShaderUniformSet = true;


                }

                auto& shape = m_scene->m_registry.get<AABBoxComponent>(entity);

                glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

                auto& tr = m_scene->m_registry.get<TransformComponent>(entity);

                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,shape.numVertices);



            }

            auto sphereBVView = m_scene->m_registry.view<SphereBVComponent>();

            firstIteration = true;

            for(auto& entity:sphereBVView)
            {
                if(firstIteration && simpleShaderUniformSet==false)
                {
                    shaderProgram = ShaderLibrary::GetShader("simple");
                    glUseProgram(shaderProgram);
                    glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                    glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                    firstIteration = false;
                    simpleShaderUniformSet = true;

                }

                auto& shape = m_scene->m_registry.get<SphereBVComponent>(entity);

                glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

                auto& tr = m_scene->m_registry.get<TransformComponent>(entity);

                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform,shape.center);
                transform = glm::scale(transform,glm::vec3(shape.radius,shape.radius,shape.radius));
                glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

                std::pair<unsigned int,unsigned int> mesh = MeshLibrary::GetMesh("uvsphere");
                
                glBindVertexArray(mesh.first);
                glDrawElements(GL_POINTS,mesh.second,GL_UNSIGNED_INT,nullptr);



            }

            auto obbView = m_scene->m_registry.view<OBBoxComponent>();

            firstIteration = true;

            for(auto& entity:obbView)
            {
                if(firstIteration && simpleShaderUniformSet==false)
                {
                        shaderProgram = ShaderLibrary::GetShader("simple");
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        firstIteration = false;
                        simpleShaderUniformSet = true;

                }

                auto& shape = m_scene->m_registry.get<OBBoxComponent>(entity);

                glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

                auto& tr = m_scene->m_registry.get<TransformComponent>(entity);

                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,shape.numVertices);



            }

            auto capsuleView = m_scene->m_registry.view<CapsuleBVComponent>();

            firstIteration = true;

            for(auto& entity:capsuleView)
            {
                if(firstIteration && simpleShaderUniformSet==false)
                {
                        shaderProgram = ShaderLibrary::GetShader("simple");
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        firstIteration = false;
                        simpleShaderUniformSet = true;

                }

                auto& shape = m_scene->m_registry.get<CapsuleBVComponent>(entity);

                glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

                auto& tr = m_scene->m_registry.get<TransformComponent>(entity);

                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,shape.numVertices);


            }

            auto pointsView = m_scene->m_registry.view<PointsRendererComponent>();

            firstIteration = true;

            for(auto& entity:pointsView)
            {
                    if(firstIteration && simpleShaderUniformSet==false)
                    {
                        shaderProgram = ShaderLibrary::GetShader("simple");
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        firstIteration = false;
                        simpleShaderUniformSet = true;

                    }

                auto&  shape = m_scene->m_registry.get<PointsRendererComponent>(entity);

                glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_POINTS,0,shape.points.size());

            }




            auto lineStripView = m_scene->m_registry.view<LineStripRendererComponent>();

            firstIteration = true;

            for(auto& entity:lineStripView)
            {
                    if(firstIteration && simpleShaderUniformSet==false)
                    {
                        shaderProgram = ShaderLibrary::GetShader("simple");
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(simpleUniformMap["simple_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(simpleUniformMap["simple_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        firstIteration = false;
                        simpleShaderUniformSet = true;

                    }

                auto&  shape = m_scene->m_registry.get<LineStripRendererComponent>(entity);

                glUniform3f(simpleUniformMap["simple_color"],shape.color.r,shape.color.g,shape.color.b);

                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(simpleUniformMap["simple_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,shape.points.size());

            }

    

       }

       if(renderPrimitives)
       {
           
           firstIteration = true;

           bool primitiveShaderUniformSet = false;

           auto lineSegmentView = m_scene->m_registry.view<LineSegmentPrimitiveComponent>();

           for(auto& entity:lineSegmentView)
           {
               if(firstIteration && primitiveShaderUniformSet==false)
               {
                  

                        shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

               }

                auto&  shape = m_scene->m_registry.get<LineSegmentPrimitiveComponent>(entity);
                auto& p = m_scene->m_registry.get<ParentComponent>(entity);

                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);

                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                
                
                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,2);

                glUniform3f(primitiveUniformMap["primitive_color"],0.0f,1.0f,1.0f);
                auto& Atr = p.children[0].GetComponent<TransformComponent>();
                auto& Btr = p.children[1].GetComponent<TransformComponent>();
                std::pair<unsigned int,unsigned int> mesh = MeshLibrary::GetMesh("cube");

                transform = glm::mat4(1.0f);
                transform = glm::translate(transform,Atr.position);
                transform = glm::scale(transform,glm::vec3(.1,.1,.1));
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(p.children[0].GetEntityID())+1);
                glBindVertexArray(mesh.first);
                glDrawElements(GL_TRIANGLES,mesh.second,GL_UNSIGNED_INT,0);

                transform = glm::mat4(1.0f);
                transform = glm::translate(transform,Btr.position);
                transform = glm::scale(transform,glm::vec3(.1,.1,.1));
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(p.children[1].GetEntityID())+1);
                glBindVertexArray(mesh.first);
                glDrawElements(GL_TRIANGLES,mesh.second,GL_UNSIGNED_INT,0);

           }

           firstIteration = true;

           auto pointView = m_scene->m_registry.view<PointPrimitiveComponent>();

           for(auto& entity:pointView)
           {
               if(firstIteration && primitiveShaderUniformSet==false)
               {
                  

                        shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

               }

                auto&  shape = m_scene->m_registry.get<PointPrimitiveComponent>(entity);
                auto& pointTr = m_scene->m_registry.get<TransformComponent>(entity);
                

                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);

                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform,pointTr.position);
                transform = glm::scale(transform,glm::vec3(.1,.1,.1));
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                
                std::pair<unsigned int,unsigned int> mesh = MeshLibrary::GetMesh("cube");
                
                glBindVertexArray(mesh.first);
                glDrawElements(GL_TRIANGLES,mesh.second,GL_UNSIGNED_INT,nullptr);

           }

            firstIteration = true;

            auto planeView = m_scene->m_registry.view<PlanePrimitiveComponent>();

            for(auto& entity:planeView)
            {
                if(firstIteration && primitiveShaderUniformSet==false)
               {
                  

                        shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

               }


                auto&  shape = m_scene->m_registry.get<PlanePrimitiveComponent>(entity);
                auto& tr = m_scene->m_registry.get<TransformComponent>(entity);
                
                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);

                glm::mat4 transform = glm::mat4(1.0f);
                glm::mat4 arrowTransform;
                transform = glm::translate(transform,tr.position);
                if(shape.normal.x == 0 && shape.normal.y == 1 && shape.normal.z == 0)
                {
                    
                }
                else if(shape.normal.x == 0 && shape.normal.y == -1 && shape.normal.z ==0)
                {
                    transform = glm::rotate(transform,glm::radians(180.0f),glm::vec3(0,0,1));
                }
                else
                {
                    glm::vec3 rotAxis = glm::cross(glm::vec3(0,1,0),shape.normal);
                    glm::vec3 newNormal = glm::normalize(shape.normal);
                    float angle = acos(glm::dot(glm::vec3(0,1,0),newNormal));
                    transform = glm::rotate(transform,angle,glm::normalize(rotAxis));
                }

                arrowTransform = transform;
                
                transform = glm::scale(transform,glm::vec3(tr.scale.x,.01,tr.scale.z));

                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                
                std::pair<unsigned int,unsigned int> mesh = MeshLibrary::GetMesh("cube");
                glBindVertexArray(mesh.first);
                glDrawElements(GL_TRIANGLES,mesh.second,GL_UNSIGNED_INT,nullptr);

                glUniform3f(primitiveUniformMap["primitive_color"],0,1,1);
                arrowTransform = glm::scale(arrowTransform,glm::vec3(70,70,70));
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(arrowTransform));
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);

                std::pair<unsigned int,unsigned int> arrowMesh = MeshLibrary::GetMesh("arrow");
                glBindVertexArray(arrowMesh.first);
                glDrawElements(GL_TRIANGLES,arrowMesh.second,GL_UNSIGNED_INT,nullptr);



            }

            firstIteration = true;

            auto rectangleView = m_scene->m_registry.view<Rectangle3DPrimitiveComponent>();

            for(auto& entity:rectangleView)
            {
                 if(firstIteration && primitiveShaderUniformSet==false)
               {
                  

                        shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

               }

                auto&  shape = m_scene->m_registry.get<Rectangle3DPrimitiveComponent>(entity);

                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,5);

               


            }


            firstIteration = true;

            auto triangleView = m_scene->m_registry.view<TrianglePrimitiveComponent>();

            for(auto& entity:triangleView)
            {
                if(firstIteration && primitiveShaderUniformSet==false)
                {
                       shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

                }

                auto&  shape = m_scene->m_registry.get<TrianglePrimitiveComponent>(entity);

                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,4);




            }

            firstIteration = true;

            auto tetraView = m_scene->m_registry.view<TetrahedronPrimitiveComponent>();

            for(auto& entity:tetraView)
            {
                if(firstIteration && primitiveShaderUniformSet==false)
                {
                       shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

                }

                auto&  shape = m_scene->m_registry.get<TetrahedronPrimitiveComponent>(entity);

                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,8);


            }

            firstIteration = true;

            auto lineView = m_scene->m_registry.view<LinePrimitiveComponent>();

            for(auto& entity:lineView)
            {
                if(firstIteration && primitiveShaderUniformSet==false)
                {
                       shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

                }

                auto&  shape = m_scene->m_registry.get<LinePrimitiveComponent>(entity);

                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,8);

            }

            firstIteration = true;

            auto rayView = m_scene->m_registry.view<RayPrimitiveComponent>();

            for(auto& entity:rayView)
            {
                if(firstIteration && primitiveShaderUniformSet==false)
                {
                       shaderProgram = ShaderLibrary::GetShader("primitive");
                        
                        glUseProgram(shaderProgram);
                        glUniformMatrix4fv(primitiveUniformMap["primitive_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
                        glUniformMatrix4fv(primitiveUniformMap["primitive_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
                        
                        firstIteration = false;
                        primitiveShaderUniformSet = true;

                }

                auto&  shape = m_scene->m_registry.get<RayPrimitiveComponent>(entity);

                glUniform3f(primitiveUniformMap["primitive_color"],shape.color.r,shape.color.g,shape.color.b);
                glUniform1i(primitiveUniformMap["primitive_entityID"],uint32_t(entity)+1);
                glm::mat4 transform = glm::mat4(1.0f);
                glUniformMatrix4fv(primitiveUniformMap["primitive_transform"],1,GL_FALSE,glm::value_ptr(transform));

                glBindVertexArray(shape.VAO);
                glDrawArrays(GL_LINE_STRIP,0,8);


            }




       }

       /*if(GameLayer::isCameraOn)
       {
           
           unsigned int shaderProgram = ShaderLibrary::GetShader("camera");
           glUseProgram(shaderProgram);
           glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform,glm::vec3(0.75f,-0.625f,0.0f));
            transform = glm::scale(transform,glm::vec3(0.5f,0.5f,0.0f));
            glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D,GameLayer::cameraQuadTexture);
           int loc = glGetUniformLocation(shaderProgram,"transform");
            glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(transform));
            loc = glGetUniformLocation(shaderProgram,"ourTexture");
            glUniform1i(loc,0);
           glBindVertexArray(GameLayer::cameraQuadVAO);
           glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

       }*/





        




        




        // Render the cube map

        /*if(renderEnvMap)
        {

        
            shaderProgram = ShaderLibrary::GetShader("background");
            glUseProgram(shaderProgram);
            glUniformMatrix4fv(pbrUniformMap["background_projection"],1,GL_FALSE,glm::value_ptr(CameraSystem::projectionMatrix));
            glUniformMatrix4fv(pbrUniformMap["background_view"],1,GL_FALSE,glm::value_ptr(CameraSystem::viewMatrix));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP,envCubemap);
            RenderCube();
        }*/

    }

    bool RendererSystem::OnWindowResizedEvent(WindowResizedEvent& e)
    {
        glViewport(0,0,e.GetWidth(),e.GetHeight());

        return true;
    }

    void RendererSystem::AddToSimpleRenderMap(const std::string& name,uint32_t id)
    {
        if(simpleRenderMap.count(name)==0)
        {
            simpleRenderMap[name] = std::vector<uint32_t>();
        }

        simpleRenderMap[name].push_back(id);

    }

    void RendererSystem::RemoveFromSimpleRenderMap(const std::string& name,uint32_t id)
    {
        if(simpleRenderMap.count(name)==0)
        {
            return;
        }

        auto& v = simpleRenderMap[name];
        for(int i=0;i<v.size();i++)
        {
            if(v[i]==id)
            {
                v.erase(v.begin()+i);
                break;
            }
        }



    }


    void RendererSystem::AddToPbrRenderMap(const std::string& name,uint32_t id)
    {
        if(pbrRenderMap.count(name)==0)
        {
            pbrRenderMap[name] = std::vector<uint32_t>();
        }

        pbrRenderMap[name].push_back(id);

    }

    void RendererSystem::RemoveFromPbrRenderMap(const std::string& name,uint32_t id)
    {
        if(pbrRenderMap.count(name)==0)
        {
            return;
        }

        auto& v = pbrRenderMap[name];
        for(int i=0;i<v.size();i++)
        {
            if(v[i]==id)
            {
                

                v.erase(v.begin()+i);
                return;
            }
        }



    }



    void RendererSystem::AddToPbrStaticRenderMap(const std::string& name,uint32_t id)
    {
        if(pbrStaticRenderMap.count(name)==0)
        {
            pbrStaticRenderMap[name] = std::vector<uint32_t>();
        }

        pbrStaticRenderMap[name].push_back(id);

    }

    void RendererSystem::RemoveFromPbrStaticRenderMap(const std::string& name,uint32_t id)
    {
        if(pbrStaticRenderMap.count(name)==0)
        {
            return;
        }

        auto& v = pbrStaticRenderMap[name];
        for(int i=0;i<v.size();i++)
        {
            if(v[i]==id)
            {
                v.erase(v.begin()+i);
                break;
            }
        }



    }


    void RendererSystem::RenderCube()
    {
        

        // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
        // render Cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


    }

    void RendererSystem::RenderQuad()
    {

                if (quadVAO == 0)
            {
                float quadVertices[] = {
                    // positions        // texture Coords
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                };
                // setup plane VAO
                glGenVertexArrays(1, &quadVAO);
                glGenBuffers(1, &quadVBO);
                glBindVertexArray(quadVAO);
                glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            }
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

    }


    std::unordered_map<std::string, std::vector<std::vector<uint32_t>>> RendererSystem::GetBatchedMap()
    {
        std::unordered_map<std::string,std::vector<std::vector<uint32_t>>> map;

        for(auto& item:pbrRenderMap)
        {
            std::unordered_map<unsigned int,std::vector<uint32_t>> histogram;

            for(auto& e:item.second)
            {
                unsigned int VAO = m_scene->m_registry.get<MeshComponent>((entt::entity)e).VAO;
                if(histogram.count(VAO)==0)
                {
                    histogram[VAO] = std::vector<uint32_t>();
                }

                histogram[VAO].push_back(e);
            }

            std::vector<std::vector<uint32_t>> v;

            for(auto h:histogram)
            {
                v.push_back(h.second);
            }

            map[item.first] = v;
  
        }


        return map;
        
    }

    std::unordered_map<std::string, std::vector<std::vector<uint32_t>>> RendererSystem::GetStaticBatchedMap()
    {
        std::unordered_map<std::string,std::vector<std::vector<uint32_t>>> map;

        for(auto& item:pbrStaticRenderMap)
        {
            std::unordered_map<unsigned int,std::vector<uint32_t>> histogram;

            for(auto& e:item.second)
            {
                unsigned int VAO = m_scene->m_registry.get<MeshComponent>((entt::entity)e).VAO;
                if(histogram.count(VAO)==0)
                {
                    histogram[VAO] = std::vector<uint32_t>();
                }

                histogram[VAO].push_back(e);
            }

            std::vector<std::vector<uint32_t>> v;

            for(auto h:histogram)
            {
                v.push_back(h.second);
            }

            map[item.first] = v;
  
        }


        return map;
        
    }


    void RendererSystem::PbrStaticPrepare()
    {
        staticMap = GetStaticBatchedMap();

        for(auto& item:staticMap)
        {
            
            for(auto& v:item.second)
            {
                std::pair<unsigned int,std::pair<unsigned int,unsigned int>> result;

                auto &mc = m_scene->m_registry.get<MeshComponent>((entt::entity)v[0]);

                glm::mat4* transforms = new glm::mat4(v.size());

                for(int i=0;i<v.size();i++)
                {
                    auto& tc = m_scene->m_registry.get<TransformComponent>((entt::entity)v[i]);

                    glm::mat4 transform = glm::mat4(1.0f);
                    transform = glm::translate(transform,tc.position);
                    glm::mat4 rot = glm::toMat4(glm::quat(tc.rotation));
                    transform = transform*rot;
                    transform = glm::scale(transform,tc.scale);
                    transforms[i] = transform;
                }



                std::string meshName = mc.meshName;

                std::pair<unsigned int,unsigned int> mesh = MeshLoader::LoadMeshFromFile(meshName);

                glBindVertexArray(mesh.first);
                unsigned int VBO;
                glGenBuffers(1,&VBO);
                glBindBuffer(GL_ARRAY_BUFFER,VBO);
                glBufferData(GL_ARRAY_BUFFER,sizeof(glm::mat4)*v.size(),transforms,GL_STATIC_DRAW);
                glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)0);
                glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(1*sizeof(glm::vec4)));
                glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(2*sizeof(glm::vec4)));
                glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(3*sizeof(glm::vec4)));
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
                glEnableVertexAttribArray(5);
                glEnableVertexAttribArray(6);
                glVertexAttribDivisor(3,1);
                glVertexAttribDivisor(4,1);
                glVertexAttribDivisor(5,1);
                glVertexAttribDivisor(6,1);

                result.first = mesh.first;
                result.second.first = mesh.second;
                result.second.second = v.size();

                staticPreparedMap[item.first].push_back(result);

                delete[] transforms;

             
            }

            
        }

    }




}