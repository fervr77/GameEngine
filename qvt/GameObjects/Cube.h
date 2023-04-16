#ifndef CUBE_H
#define CUBE_H
#include "GameObject.h"
#include <glm/glm.hpp>

namespace qvt
{
    class Cube: public GameObject
    {
        public:
            Cube(glm::vec3 pos = glm::vec3(0,0,0));
            ~Cube();


        

    };

}
#endif