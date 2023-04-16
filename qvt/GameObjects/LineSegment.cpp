#include "LineSegment.h"
#include "Components/Components.h"

namespace qvt
{
    LineSegment::LineSegment(glm::vec3 ap,glm::vec3 bp,glm::vec3 c)
    {
        entity = scene->CreateEntity("LineSegment");
        auto& tr = entity.GetComponent<TransformComponent>();
        auto& ls = entity.AddComponent<LineSegmentPrimitiveComponent>(ap,bp,c);
        auto& p = entity.AddComponent<ParentComponent>();
        Entity pointA = scene->CreateEntity("PointA");
        Entity pointB = scene->CreateEntity("PointB");
        auto& trA = pointA.GetComponent<TransformComponent>();
        auto& trB = pointB.GetComponent<TransformComponent>();
        trA.position = ap;
        trB.position = bp;
        trA.scale = glm::vec3(.1,.1,.1);
        trB.scale = glm::vec3(.1,.1,.1);

        tr.position = (ap+bp)/2.0f;

        p.children.push_back(pointA);
        p.children.push_back(pointB);

        pointA.AddComponent<ChildComponent>().parent = entity;
        pointB.AddComponent<ChildComponent>().parent = entity;

        entity.AddComponent<GameObjectNameComponent>("LineSegment");



    }

    LineSegment::~LineSegment()
    {

    }

    void LineSegment::SetPointA(glm::vec3 p)
    {
        auto& line = entity.GetComponent<ParentComponent>();
        line.children[0].GetComponent<TransformComponent>().position = p;
        
    }

    void LineSegment::SetPointB(glm::vec3 p)
    {
        auto& line = entity.GetComponent<ParentComponent>();
        line.children[1].GetComponent<TransformComponent>().position = p;

    }
}