#ifndef HOVER_BIKE_H
#define HOVER_BIKE_H
#include "GameObject.h"

namespace qvt
{
    class HoverBike : public GameObject
    {
        public:
            HoverBike(glm::vec3 pos=glm::vec3(0,0,0));
            ~HoverBike();


    };

}
#endif