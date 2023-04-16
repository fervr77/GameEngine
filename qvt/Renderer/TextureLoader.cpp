#include <glad/glad.h>
#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Core/Logger.h"

namespace qvt
{
    unsigned int TextureLoader::LoadTexture(const std::string& name)
    {
        stbi_set_flip_vertically_on_load(true);
        
        unsigned int textureID;

        const std::string path = "../../textures/";
        const std::string filepath = path+name;

        int width,height,nchannels;

        unsigned char* image = stbi_load(filepath.c_str(),&width,&height,&nchannels,0);

        if(!image)
        {
            QVT_CORE_CRITICAL(filepath+"  not loaded ");

            return 0;
        }

        GLenum format;

        if(nchannels==1)
        {
            format = GL_RED;
        }
        else if(nchannels==3)
        {
            format = GL_RGB;
        }
        else if(nchannels==4)
        {
            format = GL_RGBA;
        }


        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,image);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);

    
        return textureID;
    }


    void TextureLoader::LoadWhiteTexture(unsigned int* t)
    {
        uint32_t data = 0xffffffff;
        GLenum m_internalFormat = GL_RGBA8;
        GLenum m_dataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D,1,t);
        glTextureStorage2D(*t,1,m_internalFormat,1,1);
        glTextureParameteri(*t,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTextureParameteri(*t,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTextureParameteri(*t,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTextureParameteri(*t,GL_TEXTURE_WRAP_T,GL_REPEAT);

        glTextureSubImage2D(*t,0,0,0,1,1,m_dataFormat,GL_UNSIGNED_BYTE,&data);

    }

    void TextureLoader::LoadBlackTexture(unsigned int* t)
    {
        uint32_t data = 0x00000000;
        GLenum m_internalFormat = GL_RGBA8;
        GLenum m_dataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D,1,t);
        glTextureStorage2D(*t,1,m_internalFormat,1,1);
        glTextureParameteri(*t,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTextureParameteri(*t,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTextureParameteri(*t,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTextureParameteri(*t,GL_TEXTURE_WRAP_T,GL_REPEAT);

        glTextureSubImage2D(*t,0,0,0,1,1,m_dataFormat,GL_UNSIGNED_BYTE,&data);


    }
}