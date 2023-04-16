#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "GameObject.h"

namespace qvt
{
    class Rectangle : public GameObject
    {
        public:
            Rectangle(glm::vec3 p = glm::vec3(0,0,0),glm::vec3 c=glm::vec3(1,1,1));
            ~Rectangle();

    };

}
#endif