#include <glad/glad.h>
#include "MeshLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Core/Logger.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <iostream>
#include <limits>
#include <Systems/CollisionSystem.h>
#include "MeshLibrary.h"

namespace std
{
    template<> struct hash<qvt::MeshLoader::Vertex> {
        size_t operator()(qvt::MeshLoader::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^
                   (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };

    template<> struct hash<qvt::MeshLoader::PosVertex> {
        size_t operator()(qvt::MeshLoader::PosVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position)));
        }
    };
}

namespace qvt
{
    std::unordered_map<unsigned int,std::string> MeshLoader::vaoDictionary;

    std::pair<unsigned int,unsigned int> MeshLoader::LoadMeshFromFile(const std::string& name)
    {
        const std::string path  = "../../models/";
        const std::string filepath = path+name+".obj";

       

        std::pair<unsigned int,unsigned int> mesh;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex,uint32_t> uniqueVertices{};

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn,err;

        if(!tinyobj::LoadObj(&attrib,&shapes,&materials,&warn,&err,filepath.c_str()))
        {
            QVT_CORE_CRITICAL(filepath+"  OBJ file not loaded");
            return std::pair<unsigned int,unsigned int>();
        }

       

       
        for(const auto& shape:shapes)
        {
            
            
            for(const auto& index:shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.position = {
                    attrib.vertices[3*index.vertex_index+0],
                    attrib.vertices[3*index.vertex_index+1],
                    attrib.vertices[3*index.vertex_index+2]
                };

                vertex.normal = {
                    attrib.normals[3*index.normal_index+0],
                    attrib.normals[3*index.normal_index+1],
                    attrib.normals[3*index.normal_index+2]

                };

                vertex.uv = 
                {
                    attrib.texcoords[2*index.texcoord_index+0],
                    attrib.texcoords[2*index.texcoord_index+1]

                };

                if(uniqueVertices.count(vertex)==0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        

        unsigned int VAO;
        unsigned int VBO,EBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);
        unsigned int VBO2;
        
        
        glEnableVertexAttribArray(3);
        
        glEnableVertexAttribArray(4);
       
        glEnableVertexAttribArray(5);
        
        glEnableVertexAttribArray(6);
        glVertexAttribDivisor(3,1);
        glVertexAttribDivisor(4,1);
        glVertexAttribDivisor(5,1);
        glVertexAttribDivisor(6,1);
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(uint32_t)*indices.size(),indices.data(),GL_STATIC_DRAW);

       
        

        mesh.first = VAO;
        mesh.second = indices.size();

        vaoDictionary[VAO] = name;
       
        return mesh;
    }

    float MeshLoader::LoadMeshRadius(const std::string& name)
    {
        const std::string path  = "../../models/";
        const std::string filepath = path+name+".obj";

        float radiusx = -10000;
        float radiusy = -10000;
        float radiusz = -10000;

        float maxRadius = -10000;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex,uint32_t> uniqueVertices{};

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn,err;

        if(!tinyobj::LoadObj(&attrib,&shapes,&materials,&warn,&err,filepath.c_str()))
        {
            QVT_CORE_CRITICAL(filepath+"  OBJ file not loaded");
            return 0;
        }

        
        for(const auto& shape:shapes)
        {
            
            for(const auto& index:shape.mesh.indices)
            {
                
                

                if(attrib.vertices[3*index.vertex_index+0]>radiusx)
                {
                    radiusx = attrib.vertices[3*index.vertex_index+0];

                }

                if(attrib.vertices[3*index.vertex_index+1]>radiusy)
                {
                    radiusy = attrib.vertices[3*index.vertex_index+1];
                }

                if(attrib.vertices[3*index.vertex_index+2]>radiusz)
                {
                    radiusz = attrib.vertices[3*index.vertex_index+2];
                }

                

                
                
            }

           
        }

        if(radiusx>maxRadius)
        {
            maxRadius = radiusx;
        }

        if(radiusy>maxRadius)
        {
            maxRadius = radiusy;
        }

        if(radiusz>maxRadius)
        {
            maxRadius = radiusz;
        }

       

      
        return maxRadius;


        

    }

    std::pair<unsigned int,unsigned int> MeshLoader::LoadTerrainPatch()
    {
        std::pair<unsigned int,unsigned int> result;

        float vertices[] = 
        {
            0.0f,0.0f,0.0f,
            0.333f,0.0f,0.0f,
            0.666f,0.0f,0.0f,
            1.0f,0.0f,0.0f,

            0.0f,0.0f,0.333f,
            0.33f,0.0f,0.333f,
            0.66f,0.0f,0.333f,
            1.0f,0.0f,0.333f,

            0.0f,0.0f,0.666f,
            0.333f,0.0f,0.666f,
            0.666f,0.0f,0.666f,
            1.0f,0.0f,0.666f,

            0.0f,0.0f,1.0f,
            0.333f,0.0f,1.0f,
            0.666f,0.0f,1.0f,
            1.0f,0.0f,1.0f


        };

        float vertices9[]
        {
            0.0f,0.0f,0.0f,
            0.5f,0.0f,0.0f,
            1.0f,0.0f,0.0f,

            0.0f,0.0f,0.5f,
            0.5f,0.0f,0.5f,
            1.0f,0.0f,0.5f,

            0.0f,0.0f,1.0f,
            0.5f,0.0f,1.0f,
            1.0f,0.0f,1.0f

        };

       



        unsigned int VAO,VBO,EBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices9),vertices9,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glPatchParameteri(GL_PATCH_VERTICES,9);
        glEnableVertexAttribArray(0);


        result.first = VAO;
        result.second = 9;

        return result;


    }

    std::pair<glm::vec3,glm::vec3> MeshLoader::LoadAABBox(const std::string& name)
    {
        const std::string path  = "../../models/";
        const std::string filepath = path+name+".obj";

        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float minZ = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::min();
        float maxY = std::numeric_limits<float>::min();
        float maxZ = std::numeric_limits<float>::min();

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn,err;

        if(!tinyobj::LoadObj(&attrib,&shapes,&materials,&warn,&err,filepath.c_str()))
        {
            QVT_CORE_CRITICAL(filepath + " File not loaded");
            return std::pair<glm::vec3,glm::vec3>();
        }

        for(const auto& shape:shapes)
        {
            for(const auto& index:shape.mesh.indices)
            {
                float x = attrib.vertices[3*index.vertex_index+0];
                float y = attrib.vertices[3*index.vertex_index+1];
                float z = attrib.vertices[3*index.vertex_index+2];

                if(x>maxX)
                {
                    maxX = x;
                }

                if(x<minX)
                {
                    minX = x;
                }

                if(y>maxY)
                {
                    maxY = y;
                }

                if(y<minY)
                {
                    minY = y;
                }

                if(z>maxZ)
                {
                    maxZ = z;
                }

                if(z<minZ)
                {
                    minZ = z;
                }


            }
        }

        glm::vec3 min = glm::vec3(minX,minY,minZ);
        glm::vec3 max = glm::vec3(maxX,maxY,maxZ);

        return std::pair<glm::vec3,glm::vec3>(min,max);





    }

    std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> MeshLoader::LoadOBBox(const std::string& name)
    {
        const std::string path  = "../../models/";
        const std::string filepath = path+name+".obj";

        std::unordered_map<PosVertex,uint32_t> uniqueVertices;

        tinyobj::attrib_t attributes;
        std::vector<tinyobj::material_t> materials;
        std::vector<tinyobj::shape_t> shapes;
        std::string err,warn;



        std::vector<PosVertex> vertices;
        std::vector<glm::vec3> finalVertices;

        if(!tinyobj::LoadObj(&attributes,&shapes,&materials,&warn,&err,filepath.c_str()))
        {
            QVT_CORE_CRITICAL(filepath+"  OBJ file not loaded");
            return std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>>();
        }

        for(const auto& shape:shapes)
        {
            for(const auto& index:shape.mesh.indices)
            {
                float x = attributes.vertices[3*index.vertex_index+0];
                float y = attributes.vertices[3*index.vertex_index+1];
                float z = attributes.vertices[3*index.vertex_index+2];

                
                PosVertex v{};
                v.position.x = x;
                v.position.y = y;
                v.position.z = z;

                
        

                if(uniqueVertices.count(v)==0)
                {
                    
                    uniqueVertices[v] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(v);
                    finalVertices.push_back(v.position);
                }
             
            }
        }





        float centerx = 0;
        float centery = 0;
        float centerz = 0;

        float extentX = 0;
        float extentY = 0;
        float extentZ = 0;

        std::pair<glm::vec3,glm::vec3> aabb = MeshLibrary::GetAABBox(name);

        glm::vec3 minVertex = aabb.first;
        glm::vec3 maxVertex = aabb.second;

        centerx = (maxVertex.x+minVertex.x)/2.0f;
        centery = (maxVertex.y+minVertex.y)/2.0f;
        centerz = (maxVertex.z+minVertex.z)/2.0f;

    

        glm::mat3 covarianceMatrix;

        CollisionSystem::findCovarianceMatrix(finalVertices,covarianceMatrix);

       
        glm::mat3 O;

        glm::vec3 l;

        CollisionSystem::findEigenValuesTrig3(covarianceMatrix,l);
        CollisionSystem::findEigenVectors33(covarianceMatrix,l,O);

        if(O[0][0]>=.99)
        {
            O[0][0] = 1;
            O[1][0] = 0;
            O[2][0] = 0;
        }

        if(O[1][1]>=.99)
        {
            O[1][1] = 1;
            O[0][1] = 0;
            O[2][1] = 0;
        }

        if(O[2][2]>=.99)
        {
            O[2][2] = 1;
            O[0][2] = 0;
            O[1][2] = 0;

        }

        

       
        for(int i=0;i<vertices.size();i++)
        {
            float x = glm::dot(vertices[i].position-glm::vec3(centerx,centery,centerz),glm::vec3(O[0][0],O[0][1],O[0][2]));
            float y = glm::dot(vertices[i].position-glm::vec3(centerx,centery,centerz),glm::vec3(O[1][0],O[1][1],O[1][2]));
            float z = glm::dot(vertices[i].position-glm::vec3(centerx,centery,centerz),glm::vec3(O[2][0],O[2][1],O[2][2]));
    

            if(x*x>(extentX*extentX))
            {
                if(x<0)
                {
                    x*=-1;
                }
                extentX = x;
            }

            if(y*y>(extentY*extentY))
            {
                if(y<0)
                {
                    y*=-1;
                }
                extentY = y;
            }

            if(z*z>(extentZ*extentZ))
            {
                if(z<0)
                {
                    z*=-1;
                }

                extentZ = z;
            }

            /*if((vertices[i].position.x-centerx)*(vertices[i].position.x-centerx)>(extentX*extentX))
            {
                extentX = vertices[i].position.x-centerx;
            }

            if((vertices[i].position.y-centery)*(vertices[i].position.y-centery)>(extentY*extentY))
            {
                extentY = vertices[i].position.y-centery;
            }

            if((vertices[i].position.z-centerz)*(vertices[i].position.z-centerz)>(extentZ*extentZ))
            {
                extentZ = vertices[i].position.z-centerz;
            }*/
        }

       
    
        std::pair<glm::vec3,glm::mat3> p1 = std::pair<glm::vec3,glm::mat3>(glm::vec3(extentX,extentY,extentZ),O);

        return std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>>(glm::vec3(centerx,centery,centerz),p1);
        
    }
}