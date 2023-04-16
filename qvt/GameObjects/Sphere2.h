#ifndef SPHERE2_H
#define SPHERE2_H
#include "GameObject.h"

namespace qvt
{
    class Sphere2 : public GameObject
    {
        public:
            Sphere2(glm::vec3 pos=glm::vec3(0,0,0));
            ~Sphere2();


    };

}
#endif