#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H
#include "Scene/Scene.h"
#include "Renderer/MaterialLibrary.h"


namespace qvt
{
    class WindowResizedEvent;

    class RendererSystem
    {
        public:
            RendererSystem() = default;
            RendererSystem(Scene* scene);
            ~RendererSystem();
            void OnUpdate(float ts);
            bool OnWindowResizedEvent(WindowResizedEvent& e);
            static void AddToSimpleRenderMap(const std::string& name,uint32_t id);
            static void RemoveFromSimpleRenderMap(const std::string& name,uint32_t id);
            static void AddToPbrRenderMap(const std::string& name,uint32_t id);
            static void RemoveFromPbrRenderMap(const std::string& name,uint32_t id);
            static void AddToPbrStaticRenderMap(const std::string& name,uint32_t id);
            static void RemoveFromPbrStaticRenderMap(const std::string& name,uint32_t id);
            void PbrStaticPrepare();
            static bool renderEnvMap;
            static bool renderBVs;
            static bool renderPrimitives;
            static unsigned int viewFrustumCounter;


            MaterialLibrary materialLibrary;

        private:
            void RenderCube();
            void RenderQuad();
            std::unordered_map<std::string,std::vector<std::vector<uint32_t>>> GetBatchedMap(void);
            std::unordered_map<std::string,std::vector<std::vector<uint32_t>>> GetStaticBatchedMap(void);
            
            
            
        


        private:
            Scene* m_scene;
            static std::unordered_map<std::string,std::vector<uint32_t>> simpleRenderMap;
            static std::unordered_map<std::string,int> simpleUniformMap;
            static std::unordered_map<std::string,int> primitiveUniformMap;
            static std::unordered_map<std::string,std::vector<uint32_t>> pbrRenderMap;
            static std::unordered_map<std::string,std::vector<uint32_t>> pbrStaticRenderMap;
            static std::unordered_map<std::string,int> pbrUniformMap;
            static std::unordered_map<std::string,int> pbrStaticUniformMap;
            static std::unordered_map<std::string,std::vector<std::vector<uint32_t>>> staticMap;
            static std::unordered_map<std::string,std::vector<std::pair<unsigned int,std::pair<unsigned int,unsigned int>>>> staticPreparedMap;
            static const unsigned int batchSize = 1024;
            static glm::mat4 transforms[batchSize];
            
            unsigned int hdrTexture;
            unsigned int envCubemap;
            unsigned int irradianceMap;
            unsigned int prefilterMap;
            unsigned int brdfLUTTexture;
            unsigned int cubeVAO = 0;
            unsigned int cubeVBO = 0;
            unsigned int quadVAO = 0;
            unsigned int quadVBO = 0;
            std::vector<unsigned int> auxVBO;


            friend class TerrainNode;


            

    };

}
#endif