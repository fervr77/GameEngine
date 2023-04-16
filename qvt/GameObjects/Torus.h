#ifndef TORUS_H
#define TORUS_H
#include "GameObject.h"
#include <glm/glm.hpp>

namespace qvt
{
    class Torus: public GameObject
    {
        public:
            Torus(glm::vec3 pos = glm::vec3(0,0,0));
            ~Torus();


        

    };

}
#endif