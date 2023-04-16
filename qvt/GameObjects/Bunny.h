#ifndef BUNNY_H
#define BUNNY_H
#include "GameObject.h"
#include <glm/glm.hpp>

namespace qvt
{
    class Bunny: public GameObject
    {
        public:
            Bunny(glm::vec3 pos = glm::vec3(0,0,0));
            ~Bunny();


        

    };

}
#endif