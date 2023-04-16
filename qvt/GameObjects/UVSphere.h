#ifndef UVSPHERE_H
#define UVSPHERE_H
#include "GameObject.h"

namespace qvt
{
    class UVSphere : public GameObject
    {
        public:
            UVSphere(glm::vec3 pos=glm::vec3(0,0,0));
            ~UVSphere();


    };

}
#endif