#ifndef TEXTURE_LIBRARY_H
#define TEXTURE_LIBRARY_H
#include <unordered_map>

namespace qvt
{
    class TextureLibrary
    {
        public:
            static unsigned int GetTexture(const std::string& name);
            static unsigned int GetWhiteTexture();
            static unsigned int GetBlackTexture();

        private:
           static std::unordered_map<std::string,unsigned int> textureMap;
           static unsigned int whiteTexture;
           static unsigned int blackTexture;


    };

}

#endif