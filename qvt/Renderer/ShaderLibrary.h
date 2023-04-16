#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H
#include <unordered_map>
#include "Systems/RendererSystem.h"


namespace qvt
{
    class ShaderLibrary
    {
        
        public:
            static unsigned int GetShader(const std::string& name);


        private:
            static std::unordered_map<std::string,unsigned int> shaderMap;

            friend class RendererSystem;

    };

}
#endif