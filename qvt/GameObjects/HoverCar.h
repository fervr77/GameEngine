#ifndef HOVER_CAR_H
#define HOVER_CAR_H
#include "GameObject.h"

namespace qvt
{
    class HoverCar : public GameObject
    {
        public:
            HoverCar(glm::vec3 pos=glm::vec3(0,0,0));
            ~HoverCar();


    };

}
#endif