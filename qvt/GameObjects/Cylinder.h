#ifndef CYLINDER_H
#define CYLINDER_H
#include "GameObject.h"

namespace qvt
{
    class Cylinder : public GameObject
    {
        public:
            Cylinder(glm::vec3 pos=glm::vec3(0,0,0));
            ~Cylinder();


    };

}
#endif