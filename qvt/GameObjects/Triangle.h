#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "GameObject.h"

namespace qvt
{
    class Triangle : public GameObject
    {
        public:
            Triangle(glm::vec3 p = glm::vec3(0,0,0),glm::vec3 c=glm::vec3(1,1,1));
            ~Triangle();

    };

}
#endif