#include "ShaderLoader.h"
#include <sstream>
#include <fstream>
#include "Core/Logger.h"
#include <glad/glad.h>

namespace qvt
{
    unsigned int ShaderLoader::Load(const std::string& fileName,unsigned int flags)
    {
        
        const std::string prefix = "../../shaders/";

        
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int geometryShader;
        unsigned int tessControlShader;
        unsigned int tessEvaluationShader;
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        int success;
        char infoLog[512];

        if((flags&0x1)>>0)
        {
                std::ifstream file(prefix+fileName+".vs");

            if(file.is_open())
            {
                std::stringstream ss;

                ss << file.rdbuf();

                std::string vertexString = ss.str();

                const char* vertexSource = vertexString.c_str();

                vertexShader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertexShader,1,&vertexSource,nullptr);
                glCompileShader(vertexShader);
                glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
                if(!success)
                {
                    glGetShaderInfoLog(vertexShader,512,nullptr,infoLog);
                    QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::VERTEX::COMPILE::"+infoLog);
                    return 0;
                    
                }

                file.close();

                glAttachShader(shaderProgram,vertexShader);



            }
            else
            {
                QVT_CORE_CRITICAL("FILE NOT OPENED");
                return 0;
            }


        }

        if((flags&0x2)>>1)
        {
                std::ifstream file(prefix+fileName+".fs");

            if(file.is_open())
            {
                std::stringstream ss;

                ss << file.rdbuf();

                std::string fragmentString = ss.str();

                const char* fragmentSource = fragmentString.c_str();

                fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragmentShader,1,&fragmentSource,nullptr);
                glCompileShader(fragmentShader);
                glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
                if(!success)
                {
                    glGetShaderInfoLog(fragmentShader,512,nullptr,infoLog);
                    QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::FRAGMENT::COMPILE::"+infoLog);
                    return 0;
                    
                }

                file.close();
                glAttachShader(shaderProgram,fragmentShader);

            }
            else
            {   
                QVT_CORE_CRITICAL("FILE NOT OPENED");
                return 0;

            }

        }

        if((flags&0x4)>>2)
        {
                std::ifstream file(prefix+fileName+".gs");

            if(file.is_open())
            {
                std::stringstream ss;

                ss << file.rdbuf();

                std::string GSString = ss.str();

                const char* GSSource = GSString.c_str();

                geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometryShader,1,&GSSource,nullptr);
                glCompileShader(geometryShader);
                glGetShaderiv(geometryShader,GL_COMPILE_STATUS,&success);
                if(!success)
                {
                    glGetShaderInfoLog(geometryShader,512,nullptr,infoLog);
                    QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::GEOMETRY SHADER::COMPILE::"+infoLog);
                    return 0;
                    
                }

                file.close();
                glAttachShader(shaderProgram,geometryShader);

            }
            else
            {   
                QVT_CORE_CRITICAL("FILE NOT OPENED");
                return 0;

            }   


        }

        if((flags&0x8)>>3)
        {
             std::ifstream file(prefix+fileName+".tc");

            if(file.is_open())
            {
                std::stringstream ss;

                ss << file.rdbuf();

                std::string tessControlString = ss.str();

                const char* tessControlSource = tessControlString.c_str();

                tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
                glShaderSource(tessControlShader,1,&tessControlSource,nullptr);
                glCompileShader(tessControlShader);
                glGetShaderiv(tessControlShader,GL_COMPILE_STATUS,&success);
                if(!success)
                {
                    glGetShaderInfoLog(tessControlShader,512,nullptr,infoLog);
                    QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::TESS CONTROL::COMPILE::"+infoLog);
                    return 0;
                    
                }

                file.close();
                glAttachShader(shaderProgram,tessControlShader);

            }
            else
            {   
                QVT_CORE_CRITICAL("FILE NOT OPENED");
                return 0;

            }
            
        }

        if((flags&0x10)>>4)
        {
            std::ifstream file(prefix+fileName+".te");

                if(file.is_open())
                {
                    std::stringstream ss;

                    ss << file.rdbuf();

                    std::string tessEvaluationString = ss.str();

                    const char* tessEvaluationSource = tessEvaluationString.c_str();

                    tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
                    glShaderSource(tessEvaluationShader,1,&tessEvaluationSource,nullptr);
                    glCompileShader(tessEvaluationShader);
                    glGetShaderiv(tessEvaluationShader,GL_COMPILE_STATUS,&success);
                    if(!success)
                    {
                        glGetShaderInfoLog(tessEvaluationShader,512,nullptr,infoLog);
                        QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::TESS EVALUATION::COMPILE::"+infoLog);
                        return 0;
                        
                    }

                    file.close();
                    glAttachShader(shaderProgram,tessEvaluationShader);

                }
                else
                {   
                    QVT_CORE_CRITICAL("FILE NOT OPENED");
                    return 0;

                }


        }

        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
            if(!success)
            {
                glGetProgramInfoLog(shaderProgram,512,nullptr,infoLog);
                QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::PROGRAM::LINK::"+infoLog);
                return 0;
                
            }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);
        glDeleteShader(tessControlShader);
        glDeleteShader(tessEvaluationShader);


        return shaderProgram;



    }

    unsigned int ShaderLoader::LoadTerrainShader(const std::string& fileName)
    {
        const std::string prefix = "../../shaders/";

        std::ifstream file(prefix+fileName+".vs");

        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int tessControlShader;
        unsigned int tessEvaluationShader;
        unsigned int GSShader;
        unsigned int shaderProgram;
        int success;
        char infoLog[512];

        if(file.is_open())
        {
            std::stringstream ss;

            ss << file.rdbuf();

            std::string vertexString = ss.str();

            const char* vertexSource = vertexString.c_str();

            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader,1,&vertexSource,nullptr);
            glCompileShader(vertexShader);
            glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
            if(!success)
            {
                glGetShaderInfoLog(vertexShader,512,nullptr,infoLog);
                QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::VERTEX::COMPILE::"+infoLog);
                return 0;
                
            }

            file.close();



        }
        else
        {
            QVT_CORE_CRITICAL("FILE NOT OPENED");
            return 0;
        }

        file.open(prefix+fileName+".fs");

        if(file.is_open())
        {
            std::stringstream ss;

            ss << file.rdbuf();

            std::string fragmentString = ss.str();

            const char* fragmentSource = fragmentString.c_str();

            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader,1,&fragmentSource,nullptr);
            glCompileShader(fragmentShader);
            glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
            if(!success)
            {
                glGetShaderInfoLog(fragmentShader,512,nullptr,infoLog);
                QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::FRAGMENT::COMPILE::"+infoLog);
                return 0;
                
            }

            file.close();

        }
        else
        {   
            QVT_CORE_CRITICAL("FILE NOT OPENED");
            return 0;

        }

        /*file.open(prefix+fileName+".tc");

        if(file.is_open())
        {
            std::stringstream ss;

            ss << file.rdbuf();

            std::string tessControlString = ss.str();

            const char* tessControlSource = tessControlString.c_str();

            tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tessControlShader,1,&tessControlSource,nullptr);
            glCompileShader(tessControlShader);
            glGetShaderiv(tessControlShader,GL_COMPILE_STATUS,&success);
            if(!success)
            {
                glGetShaderInfoLog(tessControlShader,512,nullptr,infoLog);
                QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::TESS CONTROL::COMPILE::"+infoLog);
                return 0;
                
            }

            file.close();

        }
        else
        {   
            QVT_CORE_CRITICAL("FILE NOT OPENED");
            return 0;

        }

        file.open(prefix+fileName+".te");

        if(file.is_open())
        {
            std::stringstream ss;

            ss << file.rdbuf();

            std::string tessEvaluationString = ss.str();

            const char* tessEvaluationSource = tessEvaluationString.c_str();

            tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(tessEvaluationShader,1,&tessEvaluationSource,nullptr);
            glCompileShader(tessEvaluationShader);
            glGetShaderiv(tessEvaluationShader,GL_COMPILE_STATUS,&success);
            if(!success)
            {
                glGetShaderInfoLog(tessEvaluationShader,512,nullptr,infoLog);
                QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::TESS EVALUATION::COMPILE::"+infoLog);
                return 0;
                
            }

            file.close();

        }
        else
        {   
            QVT_CORE_CRITICAL("FILE NOT OPENED");
            return 0;

        }

         file.open(prefix+fileName+".gs");

        if(file.is_open())
        {
            std::stringstream ss;

            ss << file.rdbuf();

            std::string GSString = ss.str();

            const char* GSSource = GSString.c_str();

            GSShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(GSShader,1,&GSSource,nullptr);
            glCompileShader(GSShader);
            glGetShaderiv(GSShader,GL_COMPILE_STATUS,&success);
            if(!success)
            {
                glGetShaderInfoLog(GSShader,512,nullptr,infoLog);
                QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::GEOMETRY SHADER::COMPILE::"+infoLog);
                return 0;
                
            }

            file.close();

        }
        else
        {   
            QVT_CORE_CRITICAL("FILE NOT OPENED");
            return 0;

        }   */





        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram,vertexShader);
        glAttachShader(shaderProgram,fragmentShader);
        //glAttachShader(shaderProgram,tessControlShader);
        //glAttachShader(shaderProgram,tessEvaluationShader);
       // glAttachShader(shaderProgram,GSShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
            if(!success)
            {
                glGetProgramInfoLog(shaderProgram,512,nullptr,infoLog);
                QVT_CORE_CRITICAL(std::string()+"ERROR::SHADER::PROGRAM::LINK::"+infoLog);
                return 0;
                
            }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        //glDeleteShader(tessControlShader);
       // glDeleteShader(tessEvaluationShader);
        //glDeleteShader(GSShader);


        return shaderProgram;

    }


    
}