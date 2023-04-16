#ifndef MONKEY_H
#define MONKEY_H
#include "GameObject.h"

namespace qvt
{
    class Monkey : public GameObject
    {
        public:
            Monkey(glm::vec3 pos=glm::vec3(0,0,0));
            ~Monkey();


    };

}
#endif