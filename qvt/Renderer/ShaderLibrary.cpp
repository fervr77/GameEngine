#include "ShaderLibrary.h"
#include "ShaderLoader.h"
#include "Core/Logger.h"

namespace qvt
{
    std::unordered_map<std::string,unsigned int> ShaderLibrary::shaderMap;

    unsigned int ShaderLibrary::GetShader(const std::string& name)
    {
        if(shaderMap.count(name)==0)
        {
            QVT_CORE_CRITICAL(name+" not loaded.Load it first");
            return 0;
            
        }

        return shaderMap[name];

    }

    
}