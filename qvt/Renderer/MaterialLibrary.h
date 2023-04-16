#ifndef MATERIAL_LIBRARY_H
#define MATERIAL_LIBRARY_H
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

namespace qvt
{
    class MaterialLibrary
    {
        public:
            struct SimpleMaterial
            {
                glm::vec3 color;

            };

            struct PbrMaterial
            {
                glm::vec3 albedo;
                float metallic;
                float roughness;
                float ao;
                unsigned int albedoMap;
                unsigned int metallicMap;
                unsigned int roughnessMap;
                unsigned int aoMap;
                unsigned int normalMap;
                unsigned int emissiveMap;
            };



        public:
            MaterialLibrary();
            ~MaterialLibrary();
            static SimpleMaterial* GetSimpleMaterial(const std::string& name);
            static PbrMaterial* GetPbrMaterial(const std::string& name);
            static void AddNewPbrMaterial(const std::string& name);
            static std::vector<std::string> pbrMaterialList;



        private:
            static std::unordered_map<std::string,SimpleMaterial> simpleMaterialMap;
            static std::unordered_map<std::string,PbrMaterial> pbrMaterialMap;




    };

}
#endif