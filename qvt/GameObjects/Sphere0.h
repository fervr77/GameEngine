#ifndef SPHERE0_H
#define SPHERE0_H
#include "GameObject.h"

namespace qvt
{
    class Sphere0 : public GameObject
    {
        public:
            Sphere0(glm::vec3 pos=glm::vec3(0,0,0));
            ~Sphere0();


    };

}
#endif