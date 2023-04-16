#ifndef STATUE_H
#define STATUE_H
#include "GameObject.h"

namespace qvt
{
    class Statue : public GameObject
    {
        public:
            Statue(glm::vec3 pos=glm::vec3(0,0,0));
            ~Statue();


    };

}
#endif