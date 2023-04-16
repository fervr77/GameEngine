#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H
#include <string>

namespace qvt
{
    class ShaderLoader
    {
        public:
            enum ShaderType
            {
                VERTEX = (1<<0),
                FRAGMENT = (1<<1),
                GEOMETRY = (1<<2),
                TESSCONTROL = (1<<3),
                TESSEVAL = (1<<4)
            };

        public:
            static unsigned int Load(const std::string& fileName,unsigned int flags);
            static unsigned int LoadTerrainShader(const std::string& fileName);
                       

    };

}
#endif