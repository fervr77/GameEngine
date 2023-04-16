#ifndef LIGHT_SYSTEM_H
#define LIGHT_SYSTEM_H
#include <glm/glm.hpp>
#include <unordered_map>

namespace qvt
{
    class LightSystem
    {
        public:
            LightSystem();
            ~LightSystem();
            void OnUpdate(float ts);

        public:
            struct PointLight
            {
                glm::vec3 position;
                glm::vec3 color;
            };



        private:
            static PointLight pointLight;
            static std::unordered_map<std::string,int> lightUniformMap;

            friend class RendererSystem;


    };

}
#endif