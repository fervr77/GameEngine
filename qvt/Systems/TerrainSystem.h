#ifndef TERRAIN_SYSTEM_H
#define TERRAIN_SYSTEM_H
#include "Scene/Scene.h"


namespace qvt
{
    class TerrainSystem
    {
        public:
            TerrainSystem(Scene* scene);
            ~TerrainSystem();
            void Update(float timeStep);

        





        private:
            Scene* scene;

            


    };

}
#endif