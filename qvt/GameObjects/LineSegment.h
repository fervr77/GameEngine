#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H
#include "GameObject.h"

namespace qvt
{
    class LineSegment : public GameObject
    {
        public:
            LineSegment(glm::vec3 ap = glm::vec3(0,0,0),glm::vec3 bp=glm::vec3(1,1,1),glm::vec3 c =glm::vec3(1,1,1));
            ~LineSegment();
            void SetPointA(glm::vec3 p);
            void SetPointB(glm::vec3 p);

    };

}
#endif