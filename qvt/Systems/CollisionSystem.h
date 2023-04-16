#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace qvt
{
    class Scene;
    class Entity;
    

    class CollisionSystem
    {
        public:
            CollisionSystem(Scene* sc);
            ~CollisionSystem();
            void OnUpdate(float ts);
            static void UpdateAABBoxes(bool forceUpdate=false);
            static void UpdateOBBoxes(bool forceUpdate=false);
            static void UpdateSphereBVs(bool forceUpdate=false);
            static void UpdateCapsuleBVs(bool forceUpdate=false);
            static void UpdateLineSegments(bool forceUpdate=false);
            static void UpdateRectangles(bool forceUpdate = false);
            static void UpdateTriangles(bool forceUpdate = false);
            static void UpdateTetrahedrons(bool forceUpdate = false);
            static void UpdateLines(bool forceUpdate = false);
            static void UpdateRays(bool forceUpdate = false);
            static bool IsPointInTriangle(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c);
            static float max(float x1,float x2);
            static float min(float x1,float x2);
            static void printMat3(const glm::mat3& A);
            static void printVec3(const glm::vec3& v);
            static void printVec4(const glm::vec4& v);
            static float vec3Length(const glm::vec3& v);
            static bool isUpperMatrix3(glm::mat3& A);
            static void findEigenMatrix3(glm::mat3& A,glm::mat3& E);
            static void findEigenVector3(glm::mat3& A,float l,glm::vec3& v);
            static void findEigenValuesTrig3(glm::mat3& A,glm::vec3& l);
            static float findDeterminant3(glm::mat3& A);
            static bool isDiagonalMatrix3(glm::mat3& A);
            static void findCovarianceMatrix(std::vector<glm::vec3>& vertices,glm::mat3& C);
            static void findEigenVectors33(glm::mat3& A,glm::vec3& l,glm::mat3& E);
            static void GaussElimination3(glm::mat3& A,glm::vec3& b,glm::vec3& x);
            static float Clamp(float n,float min,float max);
            static float ClosestPtSegmentSegment(glm::vec3 p1,glm::vec3 q1,glm::vec3 p2,glm::vec3 q2,
                                                float& s,float& t,glm::vec3& c1,glm::vec3& c2);
            static float SqDistPointSegment(glm::vec3 a,glm::vec3 b,glm::vec3 c);
            static bool TestAABBAABB(glm::vec3& Amin,glm::vec3& Amax,glm::vec3& Bmin,glm::vec3& Bmax);
            static bool TestSphereSphere(glm::vec3& Acenter,float Aradius,glm::vec3& Bcenter,float Bradius);
            static bool TestOBBOBB(glm::vec3& Acenter,glm::mat3& Arotations,glm::vec3& Ae,glm::vec3& Bcenter,glm::mat3& Brotations,glm::vec3& Be);
            static bool TestCapsuleCapsule(glm::vec3& Aa,glm::vec3& Ab,float Aradius,glm::vec3& Ba,glm::vec3& Bb,float Bradius);
            static bool TestSphereCapsule(glm::vec3& Acenter,float Aradius,glm::vec3& Ba,glm::vec3& Bb,float Bradius);

            static glm::vec3 ClosestPtPointPlane(glm::vec3& Q,glm::vec3& PlanePoint,glm::vec3& PlaneNormal);
            static glm::vec3 ClosestPtPointPlaneNormalized(glm::vec3& Q,glm::vec3& PlanePoint,glm::vec3& PlaneNormal);
            static float DistPointPlane(glm::vec3& p,glm::vec3& PlanePoint,glm::vec3& PlaneNormal);
            static void ClosestPtPointSegment(glm::vec3& p,glm::vec3& a,glm::vec3& b,float& t,glm::vec3& d);
            static void ClosestPtPointAABB(glm::vec3& p,glm::vec3& Bmin,glm::vec3& Bmax,glm::vec3& q);
            static float SqDistPointAABB(glm::vec3& p,glm::vec3& Amin,glm::vec3& Amax);
            static void ClosestPtPointOBB(glm::vec3& p,glm::vec3& Acenter,glm::mat3& Arotations,glm::vec3& Ae,glm::vec3& q);
            static float SqDistPointOBB(glm::vec3& p,glm::vec3& Acenter,glm::mat3& Arotations,glm::vec3& Ae);
            static void ClosestPtPointRect(glm::vec3& p,glm::vec3& Rcenter,glm::vec3& Ru,glm::vec3& Rv,float Rex,float Rey,glm::vec3& q);
            static void ClosestPtPointRect(glm::vec3& p,glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3& q);
            static glm::vec3 ClosestPtPointTriangle(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c);
            static int PointOutsideOfPlane(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 d);
            static glm::vec3 ClosestPtPointTetrahedron(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 d);


        private:
            void CheckCollisions();
            
            static float TriArea2D(float x1,float y1,float x2,float y2,float x3,float y3);
            static bool EigenCheck(glm::mat3& A,glm::vec3& l);
            static std::vector<std::string> colliderList;

            


        private:
            static Scene* scene;
            static bool firstUpdate;

            friend class Panel; 



    };

}
#endif