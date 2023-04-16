#ifndef SPHERE1_H
#define SPHERE1_H
#include "GameObject.h"

namespace qvt
{
    class Sphere1 : public GameObject
    {
        public:
            Sphere1(glm::vec3 pos=glm::vec3(0,0,0));
            ~Sphere1();


    };

}
#endif