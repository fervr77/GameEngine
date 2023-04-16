#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H
#include "GameObject.h"

namespace qvt
{
    class Tetrahedron : public GameObject
    {
        public:
            Tetrahedron(glm::vec3 p = glm::vec3(0,0,0),glm::vec3 c=glm::vec3(1,1,1));
            ~Tetrahedron();

    };

}
#endif