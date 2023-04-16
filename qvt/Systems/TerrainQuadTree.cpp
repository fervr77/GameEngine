#include "TerrainQuadTree.h"
#include "Renderer/MeshLibrary.h"
#include <iostream>

namespace qvt
{
    TerrainQuadTree::TerrainQuadTree(int numNodes,float scaleXZ,float scaleY)
    {
        this->numRootNodes = numNodes;
         std::pair<unsigned int,unsigned int> result = MeshLibrary::GetTerrainPatch();
         this->VAO = result.first;
         this->scaleXZ = scaleXZ;
         this->scaleY = scaleY;
         this->tessellationFactor = 1200;
         this->tessellationSlope = 3.6;
         this->tessellationShift = 0.1;
         this->lod_ranges[0] = 1500;
         this->lod_ranges[1] = 0;
         this->lod_ranges[2] = 0;
         this->lod_ranges[3] = 0;
         this->lod_ranges[4] = 0;
         this->lod_ranges[5] = 0;
         this->lod_ranges[6] = 0;
         this->lod_ranges[7] = 0;

         for(int i=0;i<8;i++)
         {
             this->lod_morph[i] = lod_ranges[i] - updateMorphingArea(i+1);
         }

         


        for(int i=0;i<numRootNodes;i++)
        {
            for(int j=0;j<numRootNodes;j++)
            {
                float values[2];
                values[0] = i;
                values[1] = j;

                

                
                children.push_back(TerrainNode(nullptr,0,values,numRootNodes,VAO,glm::vec3((float)i/numRootNodes,0,(float)j/numRootNodes),scaleXZ,scaleY));
            }
        }

        


    }

    TerrainQuadTree::~TerrainQuadTree()
    {
        
    }

    void TerrainQuadTree::Update(glm::vec3 cameraPosition,glm::vec3 worldPosition)
    {
        
        for(auto& child:children)
        {
            child.update(cameraPosition,lod_ranges,worldPosition);
        }


    }

    void TerrainQuadTree::Render()
    {
        for(auto& child:children)
        {
            child.render();
        }
    }

    glm::vec3 TerrainQuadTree::GetScaleVector()
    {
        glm::vec3 scale = glm::vec3(scaleXZ,scaleY,scaleXZ);

        return scale;
    }

    int TerrainQuadTree::updateMorphingArea(int lod)
    {
        return    (int)(scaleXZ/numRootNodes/pow(2,lod));
    }
}