#ifndef TERRAIN_NODE_H
#define TERRAIN_NODE_H
#include <glm/glm.hpp>
#include <vector>

namespace qvt
{
    class TerrainNode
    {
        public:
            TerrainNode(TerrainNode* parent,int lod,float* index,int numRootNodes,unsigned int VAO,glm::vec3 localPosition,float scaleXZ,float scaleY);
            TerrainNode() = default;
            ~TerrainNode();
            void addChildren();
            void removeChildren();
            void render();
            void update(glm::vec3 distance,int* lod_ranges,glm::vec3 worldPosition);
            static unsigned int counter;




        private:
            bool isLeaf;
            int lod;
            float index[2];
            float gap;
            float scaleXZ;
            float scaleY;
            unsigned int VAO;
            int numRootNodes;
            glm::vec3 localPosition;
            glm::vec3 localScale;
            TerrainNode* parent;
            std::vector<TerrainNode> children;
            



    };

}

#endif