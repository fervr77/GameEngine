#ifndef ARROW_H
#define ARROW_H
#include "GameObject.h"
#include <glm/glm.hpp>

namespace qvt
{
    class Arrow: public GameObject
    {
        public:
            Arrow(glm::vec3 pos = glm::vec3(0,0,0));
            ~Arrow();


        

    };

}
#endif