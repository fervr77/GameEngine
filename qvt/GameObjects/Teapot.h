#ifndef TEAPOT_H
#define TEAPOT_H
#include "GameObject.h"
#include <glm/glm.hpp>

namespace qvt
{
    class Teapot: public GameObject
    {
        public:
            Teapot(glm::vec3 pos = glm::vec3(0,0,0));
            ~Teapot();


        

    };

}
#endif