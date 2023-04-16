#ifndef MESH_LOADER_H
#define MESH_LOADER_H
#include <unordered_map>
#include <glm/glm.hpp>
#include <vector>

namespace qvt
{
    class MeshLoader
    {
        public:
            static std::pair<unsigned int,unsigned int> LoadMeshFromFile(const std::string& name);
            static float LoadMeshRadius(const std::string& name);
            static std::pair<glm::vec3,glm::vec3> LoadAABBox(const std::string& name);
            static std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> LoadOBBox(const std::string& name);
            static std::pair<unsigned int,unsigned int> LoadTerrainPatch();
            static std::unordered_map<unsigned int,std::string> vaoDictionary;
            

        public:
            struct Vertex
            {
                glm::vec3 position;
                glm::vec3 normal;
                glm::vec2 uv;

                bool operator==(const Vertex& other)const
                {
                    return position == other.position && normal == other.normal && uv == other.uv;
                }

            };

            struct PosVertex
            {
                glm::vec3 position;

                bool operator==(const PosVertex& other)const
                {
                    return position == other.position;
                }

            };


        

    };

}
#endif