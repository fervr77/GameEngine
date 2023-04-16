#ifndef TREE_H
#define TREE_H
#include "GameObject.h"
#include <glm/glm.hpp>

namespace qvt
{
    class Tree: public GameObject
    {
        public:
            Tree(glm::vec3 pos = glm::vec3(0,0,0));
            ~Tree();


        

    };

}
#endif