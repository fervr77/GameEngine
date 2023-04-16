#include "MaterialLibrary.h"
#include "Core/Logger.h"
#include <iostream>
#include "Renderer/TextureLibrary.h"


namespace qvt
{
    std::unordered_map<std::string,MaterialLibrary::SimpleMaterial> MaterialLibrary::simpleMaterialMap;
    std::unordered_map<std::string,MaterialLibrary::PbrMaterial> MaterialLibrary::pbrMaterialMap;
    std::vector<std::string> MaterialLibrary::pbrMaterialList;


    MaterialLibrary::MaterialLibrary()
    {

        simpleMaterialMap["red"] = {glm::vec3(1.0f,0.0f,0.0f)};
        simpleMaterialMap["blue"] = {glm::vec3(0.0f,0.0f,1.0f)};
        simpleMaterialMap["green"] = {glm::vec3(0.0f,1.0f,0.0f)};

        pbrMaterialMap["cyan"] = {glm::vec3(0.0f,1.0f,1.0f),0.0f,0.0,1.0,TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetBlackTexture()};

         pbrMaterialMap["white_metallic"] = {glm::vec3(1.0f,1.0f,1.0f),1.0f,0.1,1.0,TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetBlackTexture()};


        pbrMaterialMap["red"] = {glm::vec3(0.5f,0.0f,0.0f),0.0f,0.1,1.0,TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetBlackTexture()};

        pbrMaterialMap["metallic_red"] = {glm::vec3(0.5f,0.0f,0.0f),1.0f,0.1,1.0,TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetBlackTexture()};

        pbrMaterialMap["blue"] = {glm::vec3(0.0f,0.0f,1.0f),0.0f,0.0,1.0,TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetBlackTexture()};

        pbrMaterialMap["green"] = {glm::vec3(0.0f,1.0f,0.0f),0.0f,0.0,1.0,TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetBlackTexture()};

        pbrMaterialMap["floortiles"] = {glm::vec3(1.0f,1.0f,1.0f),1.0f,1.0,1.0,TextureLibrary::GetTexture("floortiles.png"),
        TextureLibrary::GetTexture("metallic_floortiles.png"),TextureLibrary::GetTexture("roughness_floortiles.png"),TextureLibrary::GetTexture("ao_floortiles.png"),
        TextureLibrary::GetTexture("normal_floortiles.png"),TextureLibrary::GetBlackTexture()};

        pbrMaterialMap["hoverbike"] = {glm::vec3(1.0f,1.0f,1.0f),1.0f,1.0,1.0,TextureLibrary::GetTexture("hoverbike.png"),
        TextureLibrary::GetTexture("metallic_hoverbike.png"),TextureLibrary::GetTexture("roughness_hoverbike.png"),TextureLibrary::GetTexture("ao_hoverbike.png"),
        TextureLibrary::GetTexture("normal_hoverbike.png"),TextureLibrary::GetTexture("emissive_hoverbike.png")};

        pbrMaterialMap["hovercar"] = {glm::vec3(1.0f,1.0f,1.0f),1.0f,1.0,1.0,TextureLibrary::GetTexture("hovercar.jpg"),
        TextureLibrary::GetTexture("metallic_hovercar.jpg"),TextureLibrary::GetTexture("roughness_hovercar.jpg"),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetTexture("normal_hovercar.jpg"),TextureLibrary::GetTexture("emissive_hovercar.jpg")};

        pbrMaterialMap["snakestatue"] = {glm::vec3(1.0f,1.0f,1.0f),1.0f,1.0,1.0,TextureLibrary::GetTexture("hovercar.jpg"),
        TextureLibrary::GetTexture("metallic_hovercar.jpg"),TextureLibrary::GetTexture("roughness_hovercar.jpg"),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetTexture("normal_hovercar.jpg"),TextureLibrary::GetTexture("emissive_hovercar.jpg")};

        pbrMaterialMap["statue"] = {glm::vec3(1.0f,1.0f,1.0f),1.0f,1.0,1.0,TextureLibrary::GetTexture("statue.png"),
        TextureLibrary::GetTexture("metallic_statue.png"),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetTexture("normal_statue.png"),TextureLibrary::GetBlackTexture()};

        pbrMaterialList.push_back("cyan");
        pbrMaterialList.push_back("white_metallic");
        pbrMaterialList.push_back("red");
        pbrMaterialList.push_back("metallic_red");
        pbrMaterialList.push_back("blue");
        pbrMaterialList.push_back("hoverbike");
        pbrMaterialList.push_back("green");
        pbrMaterialList.push_back("hovercar");
        pbrMaterialList.push_back("floortiles");
        pbrMaterialList.push_back("snakestatue");
        pbrMaterialList.push_back("statue");


        


        


        





    }

    MaterialLibrary::~MaterialLibrary()
    {

    }

    MaterialLibrary::SimpleMaterial* MaterialLibrary::GetSimpleMaterial(const std::string& name)
    {
        if(simpleMaterialMap.count(name)==0)
        {
            QVT_CORE_CRITICAL(name+" Material not found");
            return nullptr;
        }

        return &simpleMaterialMap[name];
    }


    MaterialLibrary::PbrMaterial* MaterialLibrary::GetPbrMaterial(const std::string& name)
    {
        if(pbrMaterialMap.count(name)==0)
        {
            QVT_CORE_CRITICAL(name+" Material not found");
            return nullptr;
        }

        return &pbrMaterialMap[name];
    }

    void MaterialLibrary::AddNewPbrMaterial(const std::string& name)
    {
        if(pbrMaterialMap.count(name)==0)
        {
            pbrMaterialMap[name] = {glm::vec3(0.0f,1.0f,1.0f),0.0f,0.0,1.0,TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),TextureLibrary::GetWhiteTexture(),
        TextureLibrary::GetWhiteTexture(),TextureLibrary::GetBlackTexture()};

        pbrMaterialList.push_back(name);

        }

    }
}