#ifndef CONE_H
#define CONE_H
#include "GameObject.h"
#include <glm/glm.hpp>

namespace qvt
{
    class Cone: public GameObject
    {
        public:
            Cone(glm::vec3 pos = glm::vec3(0,0,0));
            ~Cone();


        

    };

}
#endif