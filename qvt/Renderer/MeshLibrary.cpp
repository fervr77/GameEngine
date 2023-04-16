#include "MeshLibrary.h"
#include "MeshLoader.h"
#include <iostream>


namespace qvt
{
    std::unordered_map<std::string,std::pair<unsigned int,unsigned int>> MeshLibrary::meshMap;
    std::unordered_map<std::string,float> MeshLibrary::meshRadiusMap;
    std::vector<std::string> MeshLibrary::meshList;
    std::unordered_map<std::string,std::pair<glm::vec3,glm::vec3>> MeshLibrary::AABBMap;
    std::unordered_map<std::string,std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>>> MeshLibrary::OBBoxMap;

    std::pair<unsigned int,unsigned int> MeshLibrary::GetMesh(const std::string& name)
    {
        if(meshMap.count(name)==0)
        {
           
            meshMap[name] = MeshLoader::LoadMeshFromFile(name);
           
            
        }

        return meshMap[name];
    }

    std::pair<unsigned int,unsigned int> MeshLibrary::GetTerrainPatch()
    {
        if(meshMap.count("terrainPatch")==0)
        {
            meshMap["terrainPatch"] = MeshLoader::LoadTerrainPatch();

        }

        return meshMap["terrainPatch"];
    
    }

    float MeshLibrary::GetMeshRadius(const std::string& name)
    {
        if(meshRadiusMap.count(name)==0)
        {
            meshRadiusMap[name] = MeshLoader::LoadMeshRadius(name);


        }

        return meshRadiusMap[name];
    }

    std::pair<glm::vec3,glm::vec3> MeshLibrary::GetAABBox(const std::string& name)
    {
        if(AABBMap.count(name)==0)
        {
            AABBMap[name] = MeshLoader::LoadAABBox(name);
        }

        return AABBMap[name];
    }

    std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> MeshLibrary::GetOBBox(const std::string& name)
    {
        if(OBBoxMap.count(name)==0)
        {
            OBBoxMap[name] = MeshLoader::LoadOBBox(name);
        }


        return OBBoxMap[name];
    }
        
    


}