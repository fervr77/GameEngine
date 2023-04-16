#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace qvt
{
    class MeshLibrary
    {
        public:
            static std::pair<unsigned int,unsigned int> GetMesh(const std::string& name);
            static float GetMeshRadius(const std::string& name);
            static std::pair<glm::vec3,glm::vec3> GetAABBox(const std::string& name);
            static std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> GetOBBox(const std::string& name);
            static std::pair<unsigned int,unsigned int> GetTerrainPatch();
            static std::vector<std::string> meshList;
            

        private:
            static std::unordered_map<std::string,std::pair<unsigned int,unsigned int>> meshMap;
            static std::unordered_map<std::string,float> meshRadiusMap;
            static std::unordered_map<std::string,std::pair<glm::vec3,glm::vec3>> AABBMap;
            static std::unordered_map<std::string,std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>>> OBBoxMap;

            
        


    };

}
#endif