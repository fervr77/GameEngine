#ifndef RAY_H
#define RAY_H
#include "GameObject.h"

namespace qvt
{
    class Ray : public GameObject
    {
        public:
            Ray(glm::vec3 ap = glm::vec3(0,0,0),glm::vec3 bp=glm::vec3(1,1,1),glm::vec3 c =glm::vec3(1,1,1));
            ~Ray();
           

    };

}
#endif