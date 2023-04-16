#include "TextureLibrary.h"
#include "TextureLoader.h"


namespace qvt
{
    std::unordered_map<std::string,unsigned int> TextureLibrary::textureMap;
    unsigned int TextureLibrary::whiteTexture;
    unsigned int TextureLibrary::blackTexture;

    unsigned int TextureLibrary::GetTexture(const std::string& name)
    {
        if(textureMap.count(name)==0)
        {
            textureMap[name] = TextureLoader::LoadTexture(name);
        }

        return textureMap[name];
        
    }

    unsigned int TextureLibrary::GetWhiteTexture()
    {
        if(whiteTexture==0)
        {
            TextureLoader::LoadWhiteTexture(&whiteTexture);
        }

        return whiteTexture;
        
    }

    unsigned int TextureLibrary::GetBlackTexture()
    {
        if(blackTexture==0)
        {
            TextureLoader::LoadBlackTexture(&blackTexture);
        }

        return blackTexture;

    }
}