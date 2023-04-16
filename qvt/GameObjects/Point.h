#ifndef POINT_H
#define POINT_H
#include "GameObject.h"

namespace qvt
{
    class Point : public GameObject
    {
        public:
            Point(glm::vec3 p=glm::vec3(0,0,0),glm::vec3 c=glm::vec3(1,1,1));
            ~Point();

    };

}
#endif