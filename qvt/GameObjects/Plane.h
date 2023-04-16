#ifndef PLANE_H
#define PLANE_H
#include "GameObject.h"

namespace qvt
{
    class Plane : public GameObject
    {
        public:
            Plane(glm::vec3 p = glm::vec3(0,0,0),glm::vec3 n=glm::vec3(0,1,0),glm::vec3 c=glm::vec3(1,1,1));
            ~Plane();

    };

}
#endif