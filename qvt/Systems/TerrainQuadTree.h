#ifndef TERRAIN_QUAD_TREE_H
#define TERRAIN_QUAD_TREE_H
#include <vector>
#include "TerrainNode.h"

namespace qvt
{
    class TerrainQuadTree
    {
        public:
            TerrainQuadTree(int numNodes,float scaleXZ,float scaleY);
            void Update(glm::vec3 cameraPosition,glm::vec3 worldPosition);
            void Render();
            glm::vec3 GetScaleVector();
            ~TerrainQuadTree();


            private:
                int updateMorphingArea(int);


        private:
            int numRootNodes;
            std::vector<TerrainNode> children;
            unsigned int VAO;
            float scaleXZ;
            float scaleY;
            int lod_ranges[8];
            int lod_morph[8];
            int tessellationFactor;
            float tessellationSlope;
            float tessellationShift;

            friend class RendererSystem;






    };

}
#endif