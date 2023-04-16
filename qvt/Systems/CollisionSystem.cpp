#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "CollisionSystem.h"
#include "Scene/Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/RendererSystem.h"
#include "Renderer/MeshLibrary.h"
#include "Components/Components.h"
#include <iostream>


namespace qvt
{
    std::vector<std::string> CollisionSystem::colliderList;
    Scene* CollisionSystem::scene;
    bool CollisionSystem::firstUpdate = true;

    CollisionSystem::CollisionSystem(Scene* sc)
    {
        scene = sc;
        MeshLibrary::GetAABBox("cube");
        MeshLibrary::GetAABBox("uvsphere");
        MeshLibrary::GetAABBox("cylinder");
        MeshLibrary::GetAABBox("s0");
        MeshLibrary::GetAABBox("s1");
        MeshLibrary::GetAABBox("s2");
        MeshLibrary::GetAABBox("hoverbike");
        MeshLibrary::GetAABBox("hovercar");
        MeshLibrary::GetAABBox("tree");
        MeshLibrary::GetAABBox("statue");
        MeshLibrary::GetAABBox("monkey");
        MeshLibrary::GetAABBox("snakeStatue");
        MeshLibrary::GetAABBox("tsuga");
        MeshLibrary::GetAABBox("teapot");
        MeshLibrary::GetAABBox("bunny");
        MeshLibrary::GetAABBox("cone");
        MeshLibrary::GetAABBox("torus");
        MeshLibrary::GetAABBox("arrow");
       
        

        MeshLibrary::GetOBBox("cube");
        MeshLibrary::GetOBBox("uvsphere");
        MeshLibrary::GetOBBox("cylinder");
        MeshLibrary::GetOBBox("s0");
        MeshLibrary::GetOBBox("s1");
        MeshLibrary::GetOBBox("s2");
        MeshLibrary::GetOBBox("hoverbike");
        MeshLibrary::GetOBBox("hovercar");
        MeshLibrary::GetOBBox("tree");
        MeshLibrary::GetOBBox("statue");
        MeshLibrary::GetOBBox("monkey");
        MeshLibrary::GetOBBox("snakeStatue");
        MeshLibrary::GetOBBox("tsuga");
        MeshLibrary::GetOBBox("teapot");
        MeshLibrary::GetOBBox("bunny");
        MeshLibrary::GetOBBox("cone");
        MeshLibrary::GetOBBox("torus");
        MeshLibrary::GetOBBox("arrow");
        
        colliderList.push_back("AABBox");
        colliderList.push_back("OBBox");
        colliderList.push_back("SphereBV");
        colliderList.push_back("CapsuleBV");




    }

    CollisionSystem::~CollisionSystem()
    {

    }

    void CollisionSystem::OnUpdate(float ts)
    {
        

        UpdateAABBoxes();
        UpdateOBBoxes();
        UpdateSphereBVs();
        UpdateCapsuleBVs();
        UpdateLineSegments();
        UpdateRectangles();
        UpdateTriangles();
        UpdateTetrahedrons();
        UpdateLines();
        UpdateRays();

        firstUpdate = false;
        CheckCollisions();
    }

    void CollisionSystem::UpdateAABBoxes(bool forceUpdate)
    {
        auto AABBView = scene->m_registry.view<AABBoxComponent>();
        for(auto& entity:AABBView)
        {
            auto& box = scene->m_registry.get<AABBoxComponent>(entity);
            auto& tr = scene->m_registry.get<TransformComponent>(entity);

            if(tr.isStatic && !firstUpdate && !forceUpdate)continue;

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform,tr.position);
            glm::mat4 rot = glm::toMat4(glm::quat(tr.rotation));
            transform = transform*rot;
            transform = glm::scale(transform,tr.scale);

            std::pair<glm::vec3,glm::vec3> original = MeshLibrary::GetAABBox(box.meshName);

            box.max.x = max(transform[0][0]*original.first.x,transform[0][0]*original.second.x)
                        + max(transform[1][0]*original.first.y,transform[1][0]*original.second.y)
                        + max(transform[2][0]*original.first.z,transform[2][0]*original.second.z)+transform[3][0];

            box.max.y = max(transform[0][1]*original.first.x,transform[0][1]*original.second.x)
                        + max(transform[1][1]*original.first.y,transform[1][1]*original.second.y)
                        + max(transform[2][1]*original.first.z,transform[2][1]*original.second.z)+transform[3][1];

            box.max.z = max(transform[0][2]*original.first.x,transform[0][2]*original.second.x)
                        + max(transform[1][2]*original.first.y,transform[1][2]*original.second.y)
                        + max(transform[2][2]*original.first.z,transform[2][2]*original.second.z)+transform[3][2];

            box.min.x = min(transform[0][0]*original.first.x,transform[0][0]*original.second.x)
                        + min(transform[1][0]*original.first.y,transform[1][0]*original.second.y)
                        + min(transform[2][0]*original.first.z,transform[2][0]*original.second.z)+transform[3][0];

            box.min.y = min(transform[0][1]*original.first.x,transform[0][1]*original.second.x)
                        + min(transform[1][1]*original.first.y,transform[1][1]*original.second.y)
                        + min(transform[2][1]*original.first.z,transform[2][1]*original.second.z)+transform[3][1];

            box.min.z = min(transform[0][2]*original.first.x,transform[0][2]*original.second.x)
                        + min(transform[1][2]*original.first.y,transform[1][2]*original.second.y)
                        + min(transform[2][2]*original.first.z,transform[2][2]*original.second.z)+transform[3][2];



        if(RendererSystem::renderBVs || firstUpdate)
        {
                float vertices[] = 
            {
                // Line strip
                box.min.x,box.max.y,box.min.z, //(1)
                box.min.x,box.max.y,box.max.z, //(2)
                box.max.x,box.max.y,box.max.z, //(3)
                box.max.x,box.max.y,box.min.z, //(4)
                box.min.x,box.max.y,box.min.z, //(1)
                box.min.x,box.min.y,box.min.z, //(5)
                box.min.x,box.min.y,box.max.z, //(6)
                box.max.x,box.min.y,box.max.z, //(7)
                box.max.x,box.min.y,box.min.z, //(8)
                box.max.x,box.max.y,box.min.z, //(4)
                box.max.x,box.max.y,box.max.z, //(3)
                box.max.x,box.min.y,box.max.z, //(7)
                box.min.x,box.min.y,box.max.z, //(6)
                box.min.x,box.max.y,box.max.z, //(2)
                box.min.x,box.max.y,box.min.z, //(1)
                box.min.x,box.min.y,box.min.z, //(5)
                box.max.x,box.min.y,box.min.z //(8)
                

            };

            glBindVertexArray(box.VAO);
            glBindBuffer(GL_ARRAY_BUFFER,box.VBO);
            glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);

        }

    
        }


    }

    void CollisionSystem::UpdateOBBoxes(bool forceUpdate)
    {
        auto obbView = scene->m_registry.view<OBBoxComponent>();

        for(auto& entity:obbView)
        {
            auto& obb = scene->m_registry.get<OBBoxComponent>(entity);
            auto& tr = scene->m_registry.get<TransformComponent>(entity);
            if(tr.isStatic && !firstUpdate && !forceUpdate)continue;

            std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> original = MeshLibrary::GetOBBox(obb.meshName);

            obb.center.x = original.first.x;
            obb.center.y = original.first.y;
            obb.center.z = original.first.z;

            glm::vec3 newScale = original.second.second*tr.scale;

            obb.e.x = original.second.first.x*newScale.x;
            obb.e.y = original.second.first.y*newScale.y;
            obb.e.z = original.second.first.z*newScale.z;

            obb.center.x *= newScale.x;
            obb.center.y *= newScale.y;
            obb.center.z *= newScale.z;
            

            
            obb.rotations = original.second.second;

            

            glm::vec3 u = obb.e.x*(glm::vec3(obb.rotations[0][0],obb.rotations[0][1],obb.rotations[0][2]));
            glm::vec3 v = obb.e.y*(glm::vec3(obb.rotations[1][0],obb.rotations[1][1],obb.rotations[1][2]));
            glm::vec3 w = obb.e.z*(glm::vec3(obb.rotations[2][0],obb.rotations[2][1],obb.rotations[2][2]));
        
        

            float minx = obb.center.x-(u.x+v.x+w.x);
            float miny = obb.center.y-(u.y+v.y+w.y);
            float minz = obb.center.z-(u.z+v.z+w.z);
            float maxx = obb.center.x+(u.x+v.x+w.x);
            float maxy = obb.center.y+(u.y+v.y+w.y);
            float maxz = obb.center.z+(u.z+v.z+w.z);


            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::toMat4(glm::quat(tr.rotation));
            obb.rotations = glm::mat3(rot)*obb.rotations;
        

            if(RendererSystem::renderBVs || firstUpdate)
            {
                std::vector<glm::vec3> vertices;

                vertices.push_back(obb.rotations*glm::vec3(minx,maxy,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,maxy,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,maxy,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,maxy,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,maxy,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,miny,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,miny,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,miny,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,miny,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,maxy,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,maxy,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,miny,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,miny,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,maxy,maxz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,maxy,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(minx,miny,minz)+tr.position);
                vertices.push_back(obb.rotations*glm::vec3(maxx,miny,minz)+tr.position);

                glBindVertexArray(obb.VAO);
                glBindBuffer(GL_ARRAY_BUFFER,obb.VBO);
                glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());
            }

            obb.center = obb.rotations*obb.center + tr.position;

        
        

           
            


        }

    }

    void CollisionSystem::UpdateSphereBVs(bool forceUpdate)
    {
        auto sbvView = scene->m_registry.view<SphereBVComponent>();

        for(auto& entity:sbvView)
        {
            auto& sphere = scene->m_registry.get<SphereBVComponent>(entity);
            auto& tr = scene->m_registry.get<TransformComponent>(entity);
            if(tr.isStatic && !firstUpdate && !forceUpdate)continue;

            if(strcmp(sphere.meshName.c_str(),"uvsphere")==0 || strcmp(sphere.meshName.c_str(),"s0") || strcmp(sphere.meshName.c_str(),"s1")==0 || strcmp(sphere.meshName.c_str(),"s2")==0)
            {
                sphere.center = tr.position;
                sphere.radius = tr.scale.x;

                continue;
            }

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform,tr.position);
            glm::mat4 rot = glm::toMat4(glm::quat(tr.rotation));
            transform = transform*rot;
            transform = glm::scale(transform,tr.scale);

            std::pair<glm::vec3,glm::vec3> original = MeshLibrary::GetAABBox(sphere.meshName);

            float maxX = max(transform[0][0]*original.first.x,transform[0][0]*original.second.x)
                        + max(transform[1][0]*original.first.y,transform[1][0]*original.second.y)
                        + max(transform[2][0]*original.first.z,transform[2][0]*original.second.z)+transform[3][0];

            float maxY = max(transform[0][1]*original.first.x,transform[0][1]*original.second.x)
                        + max(transform[1][1]*original.first.y,transform[1][1]*original.second.y)
                        + max(transform[2][1]*original.first.z,transform[2][1]*original.second.z)+transform[3][1];

            float maxZ = max(transform[0][2]*original.first.x,transform[0][2]*original.second.x)
                        + max(transform[1][2]*original.first.y,transform[1][2]*original.second.y)
                        + max(transform[2][2]*original.first.z,transform[2][2]*original.second.z)+transform[3][2];

            float minX = min(transform[0][0]*original.first.x,transform[0][0]*original.second.x)
                        + min(transform[1][0]*original.first.y,transform[1][0]*original.second.y)
                        + min(transform[2][0]*original.first.z,transform[2][0]*original.second.z)+transform[3][0];

            float minY = min(transform[0][1]*original.first.x,transform[0][1]*original.second.x)
                        + min(transform[1][1]*original.first.y,transform[1][1]*original.second.y)
                        + min(transform[2][1]*original.first.z,transform[2][1]*original.second.z)+transform[3][1];

            float minZ = min(transform[0][2]*original.first.x,transform[0][2]*original.second.x)
                        + min(transform[1][2]*original.first.y,transform[1][2]*original.second.y)
                        + min(transform[2][2]*original.first.z,transform[2][2]*original.second.z)+transform[3][2];


            

            float a = (maxX-minX)/2.0f;
            float b = (maxY-minY)/2.0f;
            float c = (maxZ-minZ)/2.0f;

            sphere.center.x = (maxX+minX)/2.0f;
            sphere.center.y = (maxY+minY)/2.0f;
            sphere.center.z = (maxZ+minZ)/2.0f;

            float r = -1000000;

            if(a>r)
            {
                r  = a;
            }
            if(b>r)
            {
                r = b;
            }
            if(c>r)
            {
                r = c;
            }

            sphere.radius = r;
            
        }

    }

    void CollisionSystem::UpdateCapsuleBVs(bool forceUpdate)
    {
        auto capView = scene->m_registry.view<CapsuleBVComponent>();

        for(auto& entity:capView)
        {
            auto& cap = scene->m_registry.get<CapsuleBVComponent>(entity);
            auto& tr = scene->m_registry.get<TransformComponent>(entity);

            if(tr.isStatic && !firstUpdate && !forceUpdate)continue;

            std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> obb = MeshLibrary::GetOBBox(cap.meshName);

            glm::vec3 newScale = obb.second.second*tr.scale;

            obb.second.first*= newScale;

            float maxAxis = -100000;
            unsigned int index = -1;
            unsigned int secIndex = -1;
            unsigned int thirdIndex  = -1;

            if(obb.second.first[0]>maxAxis)
            {
                maxAxis = obb.second.first[0];
                index = 0;

                if(obb.second.first[1]>obb.second.first[2])
                {
                    cap.radius = obb.second.first[1];
                    secIndex = 1;
                    thirdIndex = 2;
                }
                else
                {
                    cap.radius = obb.second.first[2];
                    secIndex = 2;
                    thirdIndex = 1;
                }
            }
            if(obb.second.first[1]>maxAxis)
            {
                maxAxis = obb.second.first[1];
                index = 1;

                if(obb.second.first[0]>obb.second.first[2])
                {
                    cap.radius = obb.second.first[0];
                    secIndex = 0;
                    thirdIndex = 2;
                }
                else
                {
                    cap.radius = obb.second.first[2];
                    secIndex = 2;
                    thirdIndex = 0;

                }
            }
            if(obb.second.first[2]>maxAxis)
            {
                maxAxis = obb.second.first[2];
                index = 2;

                if(obb.second.first[0]>obb.second.first[1])
                {
                    cap.radius = obb.second.first[0];
                    secIndex = 0;
                    thirdIndex = 1;
                }
                else
                {
                    cap.radius = obb.second.first[1];
                    secIndex = 1;
                    thirdIndex = 0;
                }
            }

            obb.first*= newScale;

            cap.b = obb.first + maxAxis*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);
            cap.a = obb.first - maxAxis*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);


            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::toMat4(glm::quat(tr.rotation));
            glm::mat3 rotations = glm::mat3(rot)*obb.second.second;
            


            glm::vec3 diff = cap.b-obb.first;

            glm::vec3 p1 = obb.first+cap.radius*glm::vec3(obb.second.second[secIndex][0],obb.second.second[secIndex][1],obb.second.second[secIndex][2]);
            glm::vec3 p2 = obb.first-cap.radius*glm::vec3(obb.second.second[secIndex][0],obb.second.second[secIndex][1],obb.second.second[secIndex][2]);
            glm::vec3 p3 = p1+diff;
            glm::vec3 p4 = p2+diff;
            glm::vec3 p5 = p1-diff;
            glm::vec3 p6 = p2-diff;
            glm::vec3 p7 = cap.b+cap.radius*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);
            glm::vec3 p8 = cap.a-cap.radius*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);
            glm::vec3 p9 = obb.first+cap.radius*glm::vec3(obb.second.second[thirdIndex][0],obb.second.second[thirdIndex][1],obb.second.second[thirdIndex][2]);
            glm::vec3 p10 = obb.first-cap.radius*glm::vec3(obb.second.second[thirdIndex][0],obb.second.second[thirdIndex][1],obb.second.second[thirdIndex][2]);
            glm::vec3 p11 = p9+diff;
            glm::vec3 p12 = p10+diff;
            glm::vec3 p13 = p9-diff;
            glm::vec3 p14 = p10-diff;

           

           if(RendererSystem::renderBVs || firstUpdate)
           {


            std::vector<glm::vec3> vertices;

        
            vertices.push_back(rotations*p1+tr.position);
            vertices.push_back(rotations*p3+tr.position);
            vertices.push_back(rotations*p7+tr.position);
            vertices.push_back(rotations*p4+tr.position);
            vertices.push_back(rotations*p2+tr.position);
            vertices.push_back(rotations*p6+tr.position);
            vertices.push_back(rotations*p8+tr.position);
            vertices.push_back(rotations*p5+tr.position);
            vertices.push_back(rotations*p1+tr.position);

            vertices.push_back(rotations*p9+tr.position);
            vertices.push_back(rotations*p11+tr.position);
            vertices.push_back(rotations*p7+tr.position);
            vertices.push_back(rotations*p12+tr.position);
            vertices.push_back(rotations*p10+tr.position);
            vertices.push_back(rotations*p14+tr.position);
            vertices.push_back(rotations*p8+tr.position);
            vertices.push_back(rotations*p13+tr.position);
            vertices.push_back(rotations*p9+tr.position);

        
            glBindVertexArray(cap.VAO);
            glBindBuffer(GL_ARRAY_BUFFER,cap.VBO);
            glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());

           }

            cap.a = rotations*cap.a+tr.position;
            cap.b = rotations*cap.b+tr.position;



        }

    }

    void CollisionSystem::UpdateLineSegments(bool forceUpdate)
    {
        auto lsView = scene->m_registry.view<LineSegmentPrimitiveComponent>();

        for(auto& entity:lsView)
        {
            auto& tr = scene->m_registry.get<TransformComponent>(entity);
            auto& ls = scene->m_registry.get<LineSegmentPrimitiveComponent>(entity);
            auto& p = scene->m_registry.get<ParentComponent>(entity);

            

            auto& Atr = p.children[0].GetComponent<TransformComponent>();
            auto& Btr = p.children[1].GetComponent<TransformComponent>();

            
            
            if(tr.isStatic)
            {
                Atr.isStatic = true;
                Btr.isStatic = true;
            }

            if(tr.isStatic && !firstUpdate && !forceUpdate)
            {
                continue;
            }

            
            tr.position = (ls.a+ls.b)/2.0f;
            ls.a = Atr.position;
            ls.b = Btr.position;


            if(RendererSystem::renderPrimitives || firstUpdate)
           {
            std::vector<glm::vec3> vertices;
            vertices.push_back(ls.a);
            vertices.push_back(ls.b);

            glBindVertexArray(ls.VAO);
            glBindBuffer(GL_ARRAY_BUFFER,ls.VBO);
            glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());
           }


        }

    }

    void CollisionSystem::UpdateRectangles(bool forceUpdate)
    {
        auto rectangleView = scene->m_registry.view<Rectangle3DPrimitiveComponent>();

        for(auto& entity:rectangleView)
        {
            auto& tr = scene->m_registry.get<TransformComponent>(entity);
            auto& r = scene->m_registry.get<Rectangle3DPrimitiveComponent>(entity);

            if(tr.isStatic && !firstUpdate && !forceUpdate)
            {
                continue;
            }

            glm::vec3 p1 = glm::vec3(-r.e[0],r.e[1],0.0f);
            glm::vec3 p2 = glm::vec3(r.e[0],r.e[1],0.0f);
            glm::vec3 p3 = glm::vec3(r.e[0],-r.e[1],0.0f);
            glm::vec3 p4 = glm::vec3(-r.e[0],-r.e[1],0.0f);

            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::toMat4(glm::quat(tr.rotation));
            
            glm::mat3 newRotations = glm::mat3(rot);

            r.u = glm::vec3(newRotations[0][0],newRotations[0][1],newRotations[0][2]);
            r.v = glm::vec3(newRotations[1][0],newRotations[1][1],newRotations[1][2]);

            if(RendererSystem::renderPrimitives || firstUpdate)
            {
                std::vector<glm::vec3> vertices;

                vertices.push_back(newRotations*p1+tr.position);
                vertices.push_back(newRotations*p2+tr.position);
                vertices.push_back(newRotations*p3+tr.position);
                vertices.push_back(newRotations*p4+tr.position);
                vertices.push_back(newRotations*p1+tr.position);

                glBindVertexArray(r.VAO);
                glBindBuffer(GL_ARRAY_BUFFER,r.VBO);
                glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());




            }


            


        }
    }

    void CollisionSystem::UpdateTriangles(bool forceUpdate)
    {
        auto triangleView = scene->m_registry.view<TrianglePrimitiveComponent>();
        
        for(auto& entity:triangleView)
        {
            auto& tr = scene->m_registry.get<TransformComponent>(entity);
            auto& t = scene->m_registry.get<TrianglePrimitiveComponent>(entity);

            if(tr.isStatic && !firstUpdate && !forceUpdate)
            {
                continue;
            }

            glm::vec3 p1 = glm::vec3(-0.5,0.0f,0.0f);
            glm::vec3 p2 = glm::vec3(0.0f,0.5f,0.0f);
            glm::vec3 p3 = glm::vec3(0.5f,0.0f,0.0f);

            p1.x = p1.x*tr.scale.x;
            p1.y = p1.y*tr.scale.y;
            p2.x = p2.x*tr.scale.x;
            p2.y = p2.y*tr.scale.y;
            p3.x = p3.x*tr.scale.x;
            p3.y = p3.y*tr.scale.y;

            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::toMat4(glm::quat(tr.rotation));
            glm::mat3 newRotations = glm::mat3(rot);

            t.a = newRotations*p1 + tr.position;
            t.b = newRotations*p2 + tr.position;
            t.c = newRotations*p3 + tr.position;

            if(RendererSystem::renderPrimitives || firstUpdate)
            {
                std::vector<glm::vec3> vertices;

                vertices.push_back(t.a);
                vertices.push_back(t.b);
                vertices.push_back(t.c);
                vertices.push_back(t.a);

                glBindVertexArray(t.VAO);
                glBindBuffer(GL_ARRAY_BUFFER,t.VBO);
                glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());

            }






        }


    }

    void CollisionSystem::UpdateTetrahedrons(bool forceUpdate)
    {
        auto tetraView = scene->m_registry.view<TetrahedronPrimitiveComponent>();

        for(auto& entity:tetraView)
        {
            auto& tr = scene->m_registry.get<TransformComponent>(entity);
            auto& t = scene->m_registry.get<TetrahedronPrimitiveComponent>(entity);

            if(tr.isStatic && !forceUpdate && !firstUpdate)
            {
                continue;
            }

            glm::vec3 p1 = glm::vec3(-0.5f,0.0f,0.0f);
            glm::vec3 p2 = glm::vec3(0.0f,0.0f,-0.5f);
            glm::vec3 p3 = glm::vec3(0.5f,0.0f,0.0f);
            glm::vec3 p4 = glm::vec3(0.0f,0.5f,-0.25f);

            p1.x *= tr.scale.x;
            p1.y *= tr.scale.y;
            p1.z *= tr.scale.z;

            p2.x *= tr.scale.x;
            p2.y *= tr.scale.y;
            p2.z *= tr.scale.z;

            p3.x *= tr.scale.x;
            p3.y *= tr.scale.y;
            p3.z *= tr.scale.z;

            p4.x *= tr.scale.x;
            p4.y *= tr.scale.y;
            p4.z *= tr.scale.z;

            glm::mat4 rot = glm::mat4(1.0f);
            rot = glm::toMat4(glm::quat(tr.rotation));
            glm::mat3 newRotations = glm::mat3(rot);

            t.a = newRotations*p1+tr.position;
            t.b = newRotations*p2+tr.position;
            t.c = newRotations*p3+tr.position;
            t.d = newRotations*p4+tr.position;

            if(RendererSystem::renderPrimitives || firstUpdate)
            {
                std::vector<glm::vec3> vertices;

                vertices.push_back(t.a);
                vertices.push_back(t.b);
                vertices.push_back(t.c);
                vertices.push_back(t.a);
                vertices.push_back(t.d);
                vertices.push_back(t.b);
                vertices.push_back(t.d);
                vertices.push_back(t.c);

                glBindVertexArray(t.VAO);
                glBindBuffer(GL_ARRAY_BUFFER,t.VBO);
                glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());

            }




        }
    }

    void CollisionSystem::UpdateLines(bool forceUpdate)
    {
        if(RendererSystem::renderPrimitives || firstUpdate)
        {

            auto lineView = scene->m_registry.view<LinePrimitiveComponent>();

            for(auto& entity:lineView)
            {
                auto& tr = scene->m_registry.get<TransformComponent>(entity);
                auto& l = scene->m_registry.get<LinePrimitiveComponent>(entity);

                if(tr.isStatic && !forceUpdate && !firstUpdate)
                {
                    continue;
                }

                glm::vec3 d = l.b-l.a;

                std::vector<glm::vec3> vertices;

                glm::vec3 aNew = l.a + glm::vec3(d.x*-1000000,d.y*-1000000,d.z*-1000000);
                vertices.push_back(aNew);

                for(int i=-300;i<=300;i+=100)
                {
                    glm::vec3 newPoint = l.a+glm::vec3(d.x*i,d.y*i,d.z*i);
                    vertices.push_back(newPoint);
                }


                glm::vec3 bNew = l.a + glm::vec3(d.x*1000000,d.y*1000000,d.z*1000000);

                    vertices.push_back(bNew);

                    glBindVertexArray(l.VAO);
                    glBindBuffer(GL_ARRAY_BUFFER,l.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());
                

            }
        }
    }

    void CollisionSystem::UpdateRays(bool forceUpdate)
    {
        if(RendererSystem::renderPrimitives || firstUpdate)
        {

            auto rayView = scene->m_registry.view<RayPrimitiveComponent>();

            for(auto& entity:rayView)
            {
                auto& tr = scene->m_registry.get<TransformComponent>(entity);
                auto& r = scene->m_registry.get<RayPrimitiveComponent>(entity);

                if(tr.isStatic && !forceUpdate && !firstUpdate)
                {
                    continue;
                }

                glm::vec3 d = r.b-r.a;

                std::vector<glm::vec3> vertices;

                glm::vec3 aNew = r.a;
                vertices.push_back(aNew);

                for(int i=1000;i<=6000;i+=1000)
                {
                    glm::vec3 newPoint = r.a+glm::vec3(d.x*i,d.y*i,d.z*i);
                    vertices.push_back(newPoint);
                }


                glm::vec3 bNew = r.a + glm::vec3(d.x*1000000,d.y*1000000,d.z*1000000);

                    vertices.push_back(bNew);

                    glBindVertexArray(r.VAO);
                    glBindBuffer(GL_ARRAY_BUFFER,r.VBO);
                    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*vertices.size(),vertices.data());
                

            }
        }
    }

    

    
    float CollisionSystem::max(float x1,float x2)
    {
        return x1>x2?x1:x2;
    }

    float CollisionSystem::Clamp(float n,float min,float max)
    {
        if(n<min)return min;
        if(n>max)return max;

        return n;
    }

    float CollisionSystem::ClosestPtSegmentSegment(glm::vec3 p1,glm::vec3 q1,glm::vec3 p2,glm::vec3 q2,
                                                float& s,float& t,glm::vec3& c1,glm::vec3& c2)
        {
            float EPSILON = .0001;
            glm::vec3 d1 = q1-p1;
            glm::vec3 d2 = q2-p2;
            glm::vec3 r = p1-p2;
            float a = glm::dot(d1,d1);
            float e = glm::dot(d2,d2);
            float f = glm::dot(d2,r);

            if(a<=EPSILON && e<=EPSILON)
            {
                s = t  = 0.0f;
                c1 = p1;
                c2 = p2;

                return glm::dot(c1-c2,c1-c2);
            }

            if(a<=EPSILON)
            {
                s = 0.0f;
                t = f/e;
                t = Clamp(t,0.0f,1.0f);
            }
            else
            {
                float c = glm::dot(d1,r);
                if(e<=EPSILON)
                {
                    t = 0.0f;
                    s = Clamp(-c/a,0.0f,1.0f);
                }
                else
                {
                    float b = glm::dot(d1,d2);
                    float denom = a*e-b*b;

                    if(denom!=0.0f)
                    {
                        s = Clamp((b*f-c*e)/denom,0.0f,1.0f);

                    }
                    else
                    {
                        s = 0.0f;
                    }

                    t = (b*s+f)/e;


                    if(t<0.0f)
                    {
                        t = 0.0f;
                        s = Clamp(-c/a,0.0f,1.0f);
                    }
                    else if(t>1.0f)
                    {
                        t = 1.0f;
                        s = Clamp((b-c)/a,0.0f,1.0f);
                    }
                }
            }

            c1 = p1+d1*s;
            c2 = p2+d2*t;

            return glm::dot(c1-c2,c1-c2);

        }
                                                
                                                

    float CollisionSystem::min(float x1,float x2)
    {
        return x1<x2?x1:x2;
    }

    bool CollisionSystem::IsPointInTriangle(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c)
    {
        glm::vec3 ab = b-a;
        glm::vec3 cb = c-a;
        glm::vec3 m = glm::cross(ab,cb);
        glm::vec3 n = glm::normalize(m);
        float d = glm::dot(n,a);
        float pdist = glm::dot(n,p);
        float x = abs(m.x);
        float y = abs(m.y);
        float z = abs(m.z);
        float nu;
        float nv;
        float odd;

        if(x>=y && x>=z)
        {
            nu = TriArea2D(p.y,p.z,b.y,b.z,c.y,c.z);
            nv = TriArea2D(p.y,p.z,c.y,c.z,a.y,a.z);
            odd = 1.0f/m.x;

        }

        if(y>=x && y>=z)
        {
            nu = TriArea2D(p.z,p.x,b.z,b.x,c.z,c.x);
            nv = TriArea2D(p.z,p.x,c.z,c.x,a.z,a.x);
            odd = 1.0f/m.y;
        }

        if(z>=x && z>=y)
        {
            nu  = TriArea2D(p.x,p.y,b.x,b.y,c.x,c.y);
            nv = TriArea2D(p.x,p.y,c.x,c.y,a.x,a.y);
            odd = 1.0f/m.z;
        }

        float u = nu*odd;
        float v = nv*odd;
        float w = 1-u-v;

        float tolerance1 = 1;
        float tolerance2 = 1;

        bool result2  = pdist<(d+tolerance1) && pdist>(d-tolerance1);



        return v>=0.0f && w >= 0.0f && (v+w)<=1.0f && result2;
    }

    float CollisionSystem::TriArea2D(float x1,float y1,float x2,float y2,float x3,float y3)
    {
        return (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
        
    }

    void CollisionSystem::CheckCollisions()
    {
        auto aabbView = scene->m_registry.view<AABBoxComponent>();

        for(auto& entity:aabbView)
        {
            auto group = scene->m_registry.view<AABBoxComponent>();
            auto& tag = scene->m_registry.get<TagComponent>(entity);
            auto& Abox = scene->m_registry.get<AABBoxComponent>(entity);
            Abox.color = glm::vec3(1,1,1);
            

            for(auto& other:group)
            {
                if(entity==other)
                {
                    
                    continue;
                }

            
                auto& Bbox = scene->m_registry.get<AABBoxComponent>(other);

                bool result = TestAABBAABB(Abox.min,Abox.max,Bbox.min,Bbox.max);

                if(result)
                {
                   

                    Abox.color = glm::vec3(1,0,0);
                    Bbox.color = glm::vec3(1,0,0);
                }
                

            }
        }

        auto sphereView = scene->m_registry.view<SphereBVComponent>();

        for(auto& entity:sphereView)
        {
            auto group = scene->m_registry.view<SphereBVComponent>();
            auto& aSphere = scene->m_registry.get<SphereBVComponent>(entity);
            aSphere.color = glm::vec3(1,1,1);

            for(auto& other:group)
            {
                if(entity==other)
                {
                    continue;
                }

                

                
                auto& bSphere = scene->m_registry.get<SphereBVComponent>(other);

                bool result = TestSphereSphere(aSphere.center,aSphere.radius,bSphere.center,bSphere.radius);

                if(result)
                {
                    aSphere.color = glm::vec3(1,0,0);
                    bSphere.color = glm::vec3(1,0,0);
                }
                





            }
        }

        auto obbView = scene->m_registry.view<OBBoxComponent>();

        for(auto& entity:obbView)
        {
            auto group = scene->m_registry.view<OBBoxComponent>();
            auto& Abox = scene->m_registry.get<OBBoxComponent>(entity);
            Abox.color = glm::vec3(1,1,1);

            for(auto& other:group)
            {
                if(entity==other)
                {
                    continue;
                }

               

                auto& Bbox = scene->m_registry.get<OBBoxComponent>(other);

                bool result = TestOBBOBB(Abox.center,Abox.rotations,Abox.e,Bbox.center,Bbox.rotations,Bbox.e);

                if(result)
                {
                    Abox.color = glm::vec3(1,0,0);
                    Bbox.color = glm::vec3(1,0,0);
                }
            }


        }


        auto capView = scene->m_registry.view<CapsuleBVComponent>();

        for(auto& entity:capView)
        {
            auto group = scene->m_registry.view<CapsuleBVComponent>();

            auto& capA = scene->m_registry.get<CapsuleBVComponent>(entity);
            capA.color = glm::vec3(1,1,1);

            for(auto& other:group)
            {
                if(entity==other)
                {
                    continue;
                }

                

                auto& capB = scene->m_registry.get<CapsuleBVComponent>(other);

                bool result = TestCapsuleCapsule(capA.a,capA.b,capA.radius,capB.a,capB.b,capB.radius);

                if(result)
                {
                    capA.color = glm::vec3(1,0,0);
                    capB.color = glm::vec3(1,0,0);
                }
            }

            auto sphereGroup = scene->m_registry.view<SphereBVComponent>();

            for(auto& other:sphereGroup)
            {
                

                auto& sphere = scene->m_registry.get<SphereBVComponent>(other);

                bool result = TestSphereCapsule(sphere.center,sphere.radius,capA.a,capA.b,capA.radius);

                if(result)
                {
                    capA.color = glm::vec3(1,0,0);
                    sphere.color = glm::vec3(1,0,0);
                }
            }






        }

       

    }



    bool CollisionSystem::TestAABBAABB(glm::vec3& Amin,glm::vec3& Amax,glm::vec3& Bmin,glm::vec3& Bmax)
    {
        

        if(Amax.x< Bmin.x || Amin.x > Bmax.x)return false;
        if(Amax.y<Bmin.y || Amin.y > Bmax.y)return false;
        if(Amax.z<Bmin.z || Amin.z > Bmax.z)return false;

        return true;
    }

    bool CollisionSystem::TestSphereSphere(glm::vec3& Acenter,float Aradius,glm::vec3& Bcenter,float Bradius)
    {
        
        glm::vec3 d = Acenter - Bcenter;

        float d2 = d.x*d.x+d.y*d.y+d.z*d.z;

       



        if(d2>(Aradius+Bradius)*(Aradius+Bradius))
        {
            

            return false;
        }

        
        return true;


    }

    bool CollisionSystem::TestOBBOBB(glm::vec3& Acenter,glm::mat3& Arotations,glm::vec3& Ae,glm::vec3& Bcenter,glm::mat3& Brotations,glm::vec3& Be)
    {
        float EPSILON = .0001;

        

        glm::vec3 aU[3] = {glm::vec3(Arotations[0][0],Arotations[0][1],Arotations[0][2]),
                           glm::vec3(Arotations[1][0],Arotations[1][1],Arotations[1][2]),
                           glm::vec3(Arotations[2][0],Arotations[2][1],Arotations[2][2])};

        glm::vec3 bU[3] = {glm::vec3(Brotations[0][0],Brotations[0][1],Brotations[0][2]),
                           glm::vec3(Brotations[1][0],Brotations[1][1],Brotations[1][2]),
                           glm::vec3(Brotations[2][0],Brotations[2][1],Brotations[2][2])};


        float rA,rB;
        glm::mat3 R,absR;

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                R[i][j] = glm::dot(aU[j],bU[i]);

            } 
        }

        glm::vec3 t = Bcenter - Acenter;

        t = glm::vec3(glm::dot(t,aU[0]),glm::dot(t,aU[1]),glm::dot(t,aU[2]));

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                absR[i][j] = abs(R[i][j])+EPSILON;
            }
        }

    
        for(int i=0;i<3;i++)
        {
        
            rA = Ae[i];
            rB = Be[0]*absR[0][i] + Be[1]*absR[1][i]+Be[2]*absR[2][i];

           

            if(abs(t[i])>(rA+rB))
            {
               
                return false;

            } 
        }

        for(int i=0;i<3;i++)
        {
            rA = Ae[0]*absR[i][0] + Ae[1]*absR[i][1]+Ae[2]*absR[i][2];
            rB = Be[i];

            

            if(abs(t[0]*R[i][0]+t[1]*R[i][1]+t[2]*R[i][2])>(rA+rB))
            {
                
                return false;

            } 
        }

        rA = Ae[1]*absR[0][2]+Ae[2]*absR[0][1];
        rB = Be[1]*absR[2][0]+Be[2]*absR[1][0];

        

        if(abs(t[2]*R[0][1]-t[1]*R[0][2])>(rA+rB))
        {
           
            return false;

        } 

        rA = Ae[1]*absR[1][2]+Ae[2]*absR[1][1];
        rB = Be[0]*absR[2][0]+Be[2]*absR[0][0];

        
        if(abs(t[2]*R[1][1]-t[1]*R[1][2])>(rA+rB))
        {
            
            return false;
        } 

        rA = Ae[1]*absR[2][2]+Ae[2]*absR[2][1];
        rB = Be[0]*absR[1][0]+Be[1]*absR[0][0];
        
        if(abs(t[2]*R[2][1]-t[1]*R[2][2])>(rA+rB))
        {
            
            return false;
        } 

        rA = Ae[0]*absR[0][2]+Ae[2]*absR[0][0];
        rB = Be[1]*absR[2][1]+Be[2]*absR[1][1];
        
        if(abs(t[0]*R[0][2]-t[2]*R[0][0])>(rA+rB))
        {
           
            return false;
        } 

        rA = Ae[0]*absR[1][2]+Ae[2]*absR[1][0];
        rB = Be[0]*absR[2][1]+Be[2]*absR[0][1];
       
        if(abs(t[0]*R[1][2]-t[2]*R[1][0])>(rA+rB))
        {
            
            return false;
        } 

        rA = Ae[0]*absR[2][2]+Ae[2]*absR[2][0];
        rB = Be[0]*absR[1][1]+Be[1]*absR[0][1];
        
        if(abs(t[0]*R[2][2]-t[2]*R[2][0])>(rA+rB))
        {
            
            return false;
        } 

        rA = Ae[0]*absR[0][1]+Ae[1]*absR[0][0];
        rB = Be[1]*absR[2][2]+Be[2]*absR[1][2];
        
        if(abs(t[1]*R[0][0]-t[0]*R[0][1])>(rA+rB))
        {
            
            return false;
        } 

        rA = Ae[0]*absR[1][1]+Ae[1]*absR[1][0];
        rB = Be[0]*absR[2][2]+Be[2]*absR[0][2];
       
        if(abs(t[1]*R[1][0]-t[0]*R[1][1])>(rA+rB))
        {
            

            return false;
        } 

        rA = Ae[0]*absR[2][1]+Ae[1]*absR[2][0];
        rB = Be[0]*absR[1][2]+Be[1]*absR[0][2];
       
        if(abs(t[1]*R[2][0]-t[0]*R[2][1])>(rA+rB))
        {
           
            return false;

        }




        return true;

    }

    bool CollisionSystem::TestCapsuleCapsule(glm::vec3& Aa,glm::vec3& Ab,float Aradius,glm::vec3& Ba,glm::vec3& Bb,float Bradius)
    {
       
        float s,t;
        glm::vec3 c1,c2;

        float dist2 = ClosestPtSegmentSegment(Aa,Ab,Ba,Bb,s,t,c1,c2);

        float radius = Aradius+Bradius;

        return dist2 <= radius*radius;


    }

    bool CollisionSystem::TestSphereCapsule(glm::vec3& Acenter,float Aradius,glm::vec3& Ba,glm::vec3& Bb,float Bradius)
    {
        
        float dist2 = SqDistPointSegment(Ba,Bb,Acenter);

        float radius = Aradius + Bradius;

        return dist2<= radius*radius;

    }

    void CollisionSystem::printMat3(const glm::mat3& m)
    {
        std::cout << std::endl;

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                std::cout << m[j][i] << " ";
            }

            std::cout << std::endl;
        }

    }

    void CollisionSystem::printVec3(const glm::vec3& v)
    {
        std::cout << std::endl;

        for(int i=0;i<3;i++)
        {
            std::cout << v[i] << " ";
        }

        std::cout << std::endl;

    }

    void CollisionSystem::printVec4(const glm::vec4& v)
    {
         std::cout << std::endl;

        for(int i=0;i<4;i++)
        {
            std::cout << v[i] << " ";
        }

        std::cout << std::endl;

    }

    float CollisionSystem::vec3Length(const glm::vec3& v)
    {
         return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    }

    bool CollisionSystem::isUpperMatrix3(glm::mat3& A)
    {
        float v1 = A[0][1]*A[0][1];
        float v2 = A[0][2]*A[0][2];
        float v3 = A[1][2]*A[1][2];

        return v1<.0001 && v2<.0001 && v3<.0001;

    }

    float CollisionSystem::SqDistPointSegment(glm::vec3 a,glm::vec3 b,glm::vec3 c)
    {
        glm::vec3 ab = b-a;
        glm::vec3 ac = c-a;
        glm::vec3 bc = c-b;

        float e = glm::dot(ac,ab);

        if(e<=0.0f) return glm::dot(ac,ac);

        float f = glm::dot(ab,ab);

        if(e>=f)
        {
            return glm::dot(bc,bc);
        }

        return glm::dot(ac,ac)-e*e/f;

    }

    void CollisionSystem::findEigenMatrix3(glm::mat3& A,glm::mat3& E)
    {
        glm::vec3 l;
        glm::vec3 u,v,w;
        glm::mat3 I = glm::mat3(1);
        glm::mat3 U,V,W;
        
        glm::mat3 S;

        
        findEigenValuesTrig3(A,l);

        U = A;
        U -= I*l[0];

       

        
       

        if(10*U[0][0]*U[0][0]<U[1][1]*U[1][1] && 10*U[0][0]*U[0][0]<U[2][2]*U[2][2])
        {
            u = glm::vec3(1,0,0);
        }
        else if(10*U[1][1]*U[1][1]<U[0][0]*U[0][0] && 10*U[1][1]*U[1][1]<U[2][2]*U[2][2])
        {
            u = glm::vec3(0,1,0);
        }
        else if(10*U[2][2]*U[2][2]<U[0][0]*U[0][0] && 10*U[2][2]*U[2][2]<U[1][1]*U[1][1])
        {
            u = glm::vec3(0,0,1);
        }

        

        



        

    
        
    }

    bool CollisionSystem::EigenCheck(glm::mat3& A,glm::vec3& l)
    {
        if(A[0][0]==l[0] && A[0][1]*A[0][1]<.001 && A[0][2]*A[0][2]<.001 && 
            A[1][0]*A[1][0]<.001 && A[1][1]==l[1] && A[1][2]*A[1][2]<.001 && 
            A[2][0]*A[2][0]<.001 && A[2][1]*A[2][1]<.001 && A[2][2]==l[2])
            {
                return true;
            }

        else
        {
            return false;
        }
    }

    void CollisionSystem::findEigenVector3(glm::mat3& A,float l,glm::vec3& v)
    {
        glm::mat3 u = glm::mat3(1);
        u*= l;
        u = u-A;

        


        float max = -1000000;
        int index;
        float a = u[0][0]*u[0][0];
        float b = u[0][1]*u[0][1];
        float c = u[0][2]*u[0][2];
        if(a>max)
        {
            index = 0;
            max = a;
        }
        if(b>max)
        {
            index = 1;
            max = b;
        }
        if(c>max)
        {
            index = 2;
            max = c;
        }

        if(index!=0)
        {
            glm::vec3 temp;
            temp[0] = u[0][index];
            temp[1] = u[1][index];
            temp[2] = u[2][index];

            u[0][index] = u[0][0];
            u[1][index] = u[1][0];
            u[2][index] = u[2][0];

            u[0][0] = temp[0];
            u[1][0] = temp[1];
            u[2][0] = temp[2];

        }

    

        float lambda = -(u[0][1])/(u[0][0]);
        u[0][1] = u[0][0]*lambda + u[0][1];
        u[1][1] = u[1][0]*lambda + u[1][1];
        u[2][1] = u[2][0]*lambda + u[2][1];

        lambda = -(u[0][2])/(u[0][0]);
        u[0][2] = u[0][0]*lambda + u[0][2];
        u[1][2] = u[1][0]*lambda + u[1][2];
        u[2][2] = u[2][0]*lambda + u[2][2];

    
        max = -100000;
        a = u[1][1]*u[1][1];
        b = u[1][2]*u[1][2];
        if(a>max)
        {
            index = 1;
            max = a;
        }
        if(b>max)
        {
            index = 2;
            max = b;
        }

        if(index!=1)
        {
            glm::vec3 temp;
            temp[0] = u[0][index];
            temp[1] = u[1][index];
            temp[2] = u[2][index];

            u[0][index] = u[0][1];
            u[1][index] = u[1][1];
            u[2][index] = u[2][1];

            u[0][1] = temp[0];
            u[1][1] = temp[1];
            u[2][1] = temp[2];
        }

    
        lambda = -(u[1][2])/(u[1][1]);
        u[0][2] = u[0][1]*lambda + u[0][2];
        u[1][2] = u[1][1]*lambda + u[1][2];
        u[2][2] = u[2][1]*lambda + u[2][2];

    
        if(u[2][2]*u[2][2]<.001)
        {
            v[2] = 1;
        }
        else
        {
            v[2] = 0;
        }

        if(u[1][1]*u[1][1]<.001)
        {
            v[1] = 1;
        }
        else
        {
            v[1] = -(u[2][1])*v[2]/(u[1][1]);

        }

        if(u[0][0]*u[0][0]<.001)
        {
            v[0] = 1;
        }
        else
        {
            v[0] = (-u[1][0]*v[1]-u[2][0]*v[2])/u[0][0];
        }

        float norm = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);

        v[0] = v[0]/norm;
        v[1] = v[1]/norm;
        v[2] = v[2]/norm;


    }

    void CollisionSystem::findEigenValuesTrig3(glm::mat3& A,glm::vec3& l)
    {
        float p1 = A[1][0]*A[1][0]+A[2][0]*A[2][0]+A[2][1]*A[2][1];

        if(p1*p1<.0001)
        {
            l[0] = A[0][0];
            l[1] = A[1][1];
            l[2] = A[2][2];
        }
        else
        {
            float sum = A[0][0]+A[1][1]+A[2][2];
            float q = sum/3;
            float p2 = (A[0][0]-q)*(A[0][0]-q)+(A[1][1]-q)*(A[1][1]-q)+(A[2][2]-q)*(A[2][2]-q)+2*p1;
            float p = sqrt(p2/6);
            glm::mat3 B = glm::mat3(1);
            B*= q;
            B = A-B;
            B*= (1/p);

            float r = findDeterminant3(B)/2;
        

            float phi;

            if(r<=-1)
            {
                phi = 3.141516/3;
            }
            else if(r>=1)
            {
                phi = 0;
            }
            else
            {
                phi = acos(r)/3;
            }

            l[0] = q+2*p*cos(phi);
            l[2] = q+2*p*cos(phi+(2*3.141516/3));
            l[1] = 3*q-l[0]-l[2];

        }


    }

    float CollisionSystem::findDeterminant3(glm::mat3& A)
    {
        glm::mat3 u = A;


        float max = -1000000;
        int index;
        int rowChanged = 1;
        float a = u[0][0]*u[0][0];
        float b = u[0][1]*u[0][1];
        float c = u[0][2]*u[0][2];

        if(a>max)
        {
            index = 0;
            max = a;
        }
        if(b>max)
        {
            index = 1;
            max = b;
        }
        if(c>max)
        {
            index = 2;
            max = c;
        }

        if(index!=0)
        {
            glm::vec3 temp;
            temp[0] = u[0][index];
            temp[1] = u[1][index];
            temp[2] = u[2][index];

            u[0][index] = u[0][0];
            u[1][index] = u[1][0];
            u[2][index] = u[2][0];

            u[0][0] = temp[0];
            u[1][0] = temp[1];
            u[2][0] = temp[2];

            rowChanged = -1;

        }

    
    

        float lambda = -(u[0][1])/(u[0][0]);
        u[0][1] = u[0][0]*lambda + u[0][1];
        u[1][1] = u[1][0]*lambda + u[1][1];
        u[2][1] = u[2][0]*lambda + u[2][1];

        lambda = -(u[0][2])/(u[0][0]);
        u[0][2] = u[0][0]*lambda + u[0][2];
        u[1][2] = u[1][0]*lambda + u[1][2];
        u[2][2] = u[2][0]*lambda + u[2][2];

    
        max = -100000;
        a = u[1][1]*u[1][1];
        b = u[1][2]*u[1][2];
        if(a>max)
        {
            index = 1;
            max = a;
        }
        if(b>max)
        {
            index = 2;
            max = b;
        }

        if(index!=1)
        {
            glm::vec3 temp;
            temp[0] = u[0][index];
            temp[1] = u[1][index];
            temp[2] = u[2][index];

            u[0][index] = u[0][1];
            u[1][index] = u[1][1];
            u[2][index] = u[2][1];

            u[0][1] = temp[0];
            u[1][1] = temp[1];
            u[2][1] = temp[2];

            rowChanged*=-1;
        }

    
        lambda = -(u[1][2])/(u[1][1]);
        u[0][2] = u[0][1]*lambda + u[0][2];
        u[1][2] = u[1][1]*lambda + u[1][2];
        u[2][2] = u[2][1]*lambda + u[2][2];

    


        return u[0][0]*u[1][1]*u[2][2]*rowChanged;

    }

    void CollisionSystem::findCovarianceMatrix(std::vector<glm::vec3>& vertices,glm::mat3& C)
    {
        float miuX,miuY,miuZ;
        float totalX,totalY,totalZ;
        totalX = 0;
        totalY = 0;
        totalZ = 0;

        for(int i=0;i<vertices.size();i++)
        {
            totalX += vertices[i].x;
            totalY += vertices[i].y;
            totalZ += vertices[i].z;
        }

        miuX = totalX/vertices.size();
        miuY = totalY/vertices.size();
        miuZ = totalZ/vertices.size();

        float cxz;
        float totalcxz = 0;
        float cxx;
        float totalcxx = 0;
        float cyy;
        float totalcyy = 0;
        float czz;
        float totalczz = 0;
        float cxy;
        float totalcxy = 0;
        float cyz;
        float totalcyz = 0;

        for(int i=0;i<vertices.size();i++)
        {
            totalcxz += (vertices[i].x-miuX)*(vertices[i].z-miuZ);
            totalcxx += vertices[i].x*vertices[i].x;
            totalcyy += vertices[i].y*vertices[i].y;
            totalczz += vertices[i].z*vertices[i].z;
            totalcxy += (vertices[i].x-miuX)*(vertices[i].y-miuY);
            totalcyz += (vertices[i].y-miuY)*(vertices[i].z-miuZ);
        }

        cxz = totalcxz/vertices.size();
        cxy = totalcxy/vertices.size();
        cyz = totalcyz/vertices.size();
        cxx = totalcxx/vertices.size()-miuX*miuX;
        cyy = totalcyy/vertices.size()-miuY*miuY;
        czz = totalczz/vertices.size()-miuZ*miuZ;

        C[0][0] = cxx;
        C[0][1] = cxy;
        C[0][2] = cxz;
        C[1][0] = cxy;
        C[1][1] = cyy;
        C[1][2] = cyz;
        C[2][0] = cxz;
        C[2][1] = cyz;
        C[2][2] = czz;

    }

    bool CollisionSystem::isDiagonalMatrix3(glm::mat3& A)
    {
        if(A[0][0]*A[0][0]>.001 && A[0][1]*A[0][1]<.001 && A[0][2]*A[0][2]<.001 && 
       A[1][0]*A[1][0]<.001 && A[1][1]*A[1][1]>.001 && A[1][2]*A[1][2]<.001 && 
       A[2][0]*A[2][0]<.001 && A[2][1]*A[2][1]<.001 && A[2][2]*A[2][2]>.001)
       {
           
           return true;
       }

       return false;

    }

    void CollisionSystem::findEigenVectors33(glm::mat3& A,glm::vec3& l,glm::mat3& E)
    {

                bool isDiag = isDiagonalMatrix3(A);
            glm::vec3 e;

            if(isDiag)
            {
                e[0] = A[0][0];
                e[1] = A[1][1];
                e[2] = A[2][2];

                E = glm::mat3(1);
                l = e;

                return;
            }

            glm::mat3 I = glm::mat3(1);
            glm::mat3 U = A;
            glm::vec3 u;
            float uValue = l[0];
            U -= I*l[0];

            

            if(10*U[0][0]*U[0][0]<U[1][1]*U[1][1] && 10*U[0][0]*U[0][0]<U[2][2]*U[2][2])
            {
                
                u = glm::vec3(1,0,0);
            }
            else if(10*U[1][1]*U[1][1]<U[0][0]*U[0][0] && 10*U[1][1]*U[1][1]<U[2][2]*U[2][2])
            {
                u = glm::vec3(0,1,0);

            }
            else if(10*U[2][2]*U[2][2]<U[0][0]*U[0][0] && 10*U[2][2]*U[2][2]<U[1][1]*U[1][1])
            {
                u = glm::vec3(0,0,1);
            }
            else
            {
                glm::vec3 b = glm::vec3(0,0,0);
                GaussElimination3(U,b,u);

            }


            I = glm::mat3(1);
            glm::mat3 V = A;
            glm::vec3 v;
            float vValue = l[1];
            V -= I*l[1];

          

            if(10*V[0][0]*V[0][0]<V[1][1]*V[1][1] && 10*V[0][0]*V[0][0]<V[2][2]*V[2][2])
            {
                v = glm::vec3(1,0,0);
            }
            else if(10*V[1][1]*V[1][1]<V[0][0]*V[0][0] && 10*V[1][1]*V[1][1]<V[2][2]*V[2][2])
            {
                v = glm::vec3(0,1,0);
            }
            else if(10*V[2][2]*V[2][2]<V[0][0]*V[0][0] && 10*V[2][2]*V[2][2]<V[1][1]*V[1][1])
            {
                v = glm::vec3(0,0,1);
            }
            else
            {
                glm::vec3 b = glm::vec3(0,0,0);
                GaussElimination3(V,b,v);
                

            }


            I = glm::mat3(1);
            glm::mat3 W = A;
            glm::vec3 w;
            float wValue = l[2];
            W -= I*l[2];

           

            if(10*W[0][0]*W[0][0]<W[1][1]*W[1][1] && 10*W[0][0]*W[0][0]<W[2][2]*W[2][2])
            {
            w = glm::vec3(1,0,0);
            }
            else if(10*W[1][1]*W[1][1]<W[0][0]*W[0][0] && 10*W[1][1]*W[1][1]<W[2][2]*W[2][2])
            {
                w = glm::vec3(0,1,0);
            }
            else if(10*W[2][2]*W[2][2]<W[0][0]*W[0][0] && 10*W[2][2]*W[2][2]<W[1][1]*W[1][1])
            {
                w = glm::vec3(0,0,1);
            }
            else
            { 
                glm::vec3 b = glm::vec3(0,0,0);
                GaussElimination3(W,b,w);

            }

            if(u[0]*u[0]>=v[0]*v[0] && u[0]*u[0]>=w[0]*w[0])
            {
                e[0] = uValue;
                E[0][0] = u[0];
                E[1][0] = u[1];
                E[2][0] = u[2];

                if(v[1]*v[1]>=w[1]*w[1])
                {
                    e[1] = vValue;
                    E[0][1] = v[0];
                    E[1][1] = v[1];
                    E[2][1] = v[2];

                    e[2] = wValue;
                    E[0][2] = w[0];
                    E[1][2] = w[1];
                    E[2][2] = w[2];
                }
                else
                {
                    e[2] = vValue;
                    E[0][2] = v[0];
                    E[1][2] = v[1];
                    E[2][2] = v[2];

                    e[1] = wValue;
                    E[0][1] = w[0];
                    E[1][1] = w[1];
                    E[2][1] = w[2];

                }
            }
            else if(v[0]*v[0]>=u[0]*u[0] && v[0]*v[0]>=w[0]*w[0])
            {
                e[0] = vValue;
                E[0][0] = v[0];
                E[1][0] = v[1];
                E[2][0] = v[2];

                if(u[1]*u[1]>=w[1]*w[1])
                {
                    e[1] = uValue;
                    E[0][1] = u[0];
                    E[1][1] = u[1];
                    E[2][1] = u[2];

                    e[2] = wValue;
                    E[0][2] = w[0];
                    E[1][2] = w[1];
                    E[2][2] = w[2];
                }
                else
                {
                    e[2] = uValue;
                    E[0][2] = u[0];
                    E[1][2] = u[1];
                    E[2][2] = u[2];

                    e[1] = wValue;
                    E[0][1] = w[0];
                    E[1][1] = w[1];
                    E[2][1] = w[2];

                }
            }
            else if(w[0]*w[0]>=u[0]*u[0] && w[0]*w[0]>=v[0]*v[0])
            {
                e[0] = wValue;
                E[0][0] = w[0];
                E[1][0] = w[1];
                E[2][0] = w[2];

                if(u[1]*u[1]>=v[1]*v[1])
                {
                    e[1] = uValue;
                    E[0][1] = u[0];
                    E[1][1] = u[1];
                    E[2][1] = u[2];

                    e[2] = vValue;
                    E[0][2] = v[0];
                    E[1][2] = v[1];
                    E[2][2] = v[2];
                }
                else
                {
                    e[2] = uValue;
                    E[0][2] = u[0];
                    E[1][2] = u[1];
                    E[2][2] = u[2];

                    e[1] = vValue;
                    E[0][1] = v[0];
                    E[1][1] = v[1];
                    E[2][1] = v[2];

                }
            }

            l = e;



    }

    void CollisionSystem::GaussElimination3(glm::mat3& A,glm::vec3& bv,glm::vec3& x)
    {
                glm::mat3 u = A;


                    float max = -1000000;
                    int index;
                    int rowChanged = 1;
                    float a = u[0][0]*u[0][0];
                    float b = u[0][1]*u[0][1];
                    float c = u[0][2]*u[0][2];
                    if(a>max)
                    {
                        index = 0;
                        max = a;
                    }
                    if(b>max)
                    {
                        index = 1;
                        max = b;
                    }
                    if(c>max)
                    {
                        index = 2;
                        max = c;
                    }

                    if(index!=0)
                    {
                        glm::vec3 temp;
                        temp[0] = u[0][index];
                        temp[1] = u[1][index];
                        temp[2] = u[2][index];

                        u[0][index] = u[0][0];
                        u[1][index] = u[1][0];
                        u[2][index] = u[2][0];

                        u[0][0] = temp[0];
                        u[1][0] = temp[1];
                        u[2][0] = temp[2];

                        float btemp = bv[index];
                        bv[index] = bv[0];
                        bv[0] = btemp;


                        rowChanged = -1;

                    }

                
                

                    float lambda = -(u[0][1])/(u[0][0]);
                    u[0][1] = u[0][0]*lambda + u[0][1];
                    u[1][1] = u[1][0]*lambda + u[1][1];
                    u[2][1] = u[2][0]*lambda + u[2][1];

                    bv[1] = bv[0]*lambda + bv[1];

                    lambda = -(u[0][2])/(u[0][0]);
                    u[0][2] = u[0][0]*lambda + u[0][2];
                    u[1][2] = u[1][0]*lambda + u[1][2];
                    u[2][2] = u[2][0]*lambda + u[2][2];

                    bv[2] = bv[0]*lambda + bv[2];

                
                    max = -100000;
                    a = u[1][1]*u[1][1];
                    b = u[1][2]*u[1][2];
                    if(a>max)
                    {
                        index = 1;
                        max = a;
                    }
                    if(b>max)
                    {
                        index = 2;
                        max = b;
                    }

                    if(index!=1)
                    {
                        glm::vec3 temp;
                        temp[0] = u[0][index];
                        temp[1] = u[1][index];
                        temp[2] = u[2][index];

                        u[0][index] = u[0][1];
                        u[1][index] = u[1][1];
                        u[2][index] = u[2][1];

                        u[0][1] = temp[0];
                        u[1][1] = temp[1];
                        u[2][1] = temp[2];

                        float btemp = bv[index];
                        bv[index] = bv[1];
                        bv[1] = btemp;

                        rowChanged*=-1;
                    }

                
                    lambda = -(u[1][2])/(u[1][1]);
                    u[0][2] = u[0][1]*lambda + u[0][2];
                    u[1][2] = u[1][1]*lambda + u[1][2];
                    u[2][2] = u[2][1]*lambda + u[2][2];

                    bv[2] = bv[1]*lambda + bv[2];

                    if(bv[2]==0)
                    {
                        x[2] = 1;
                        x[1] = -u[2][1]*x[2]/(u[1][1])*rowChanged;
                        x[0] = (-u[2][0]*x[2]-u[1][0]*x[1])/(u[0][0]);

                        float norm = sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);

                        x[0] = x[0]/norm;
                        x[1] = x[1]/norm;
                        x[2] = x[2]/norm;
                    }

           

    }

    glm::vec3 CollisionSystem::ClosestPtPointPlane(glm::vec3& Q,glm::vec3& PlanePoint,glm::vec3& PlaneNormal)
    {
        float t = glm::dot(PlaneNormal,(Q-PlanePoint))/glm::dot(PlaneNormal,PlaneNormal);

        return Q-t*PlaneNormal;
    }

    glm::vec3 CollisionSystem::ClosestPtPointPlaneNormalized(glm::vec3& Q,glm::vec3& PlanePoint,glm::vec3& PlaneNormal)
    {
        float t = glm::dot(PlaneNormal,(Q-PlanePoint));

        return Q-t*PlaneNormal;
    }

    float CollisionSystem::DistPointPlane(glm::vec3& p,glm::vec3& PlanePoint,glm::vec3& PlaneNormal)
    {
        return glm::dot(PlaneNormal,(p-PlanePoint))/glm::dot(PlaneNormal,PlaneNormal);
    }

    void CollisionSystem::ClosestPtPointSegment(glm::vec3& p,glm::vec3& a,glm::vec3& b,float& t,glm::vec3& d)
    {
        glm::vec3 ab = b-a;

        t = glm::dot((p-a),ab);
        if(t<=0.0f)
        {
            t = 0.0f;
            d = a;
        }
        else
        {
            float denom = glm::dot(ab,ab);
            if(t>=denom)
            {
                t = 1.0f;
                d = b;
            }
            else
            {
                t = t/denom;

                d = a + t*ab;
            }
        }
 
    }

    void CollisionSystem::ClosestPtPointAABB(glm::vec3& p,glm::vec3& Bmin,glm::vec3& Bmax,glm::vec3& q)
    {
        for(int i=0;i<3;i++)
        {
            float v = p[i];
            if(v<Bmin[i]) v = Bmin[i];
            if(v>Bmax[i]) v = Bmax[i];

            q[i] = v;
        }
    }

    float CollisionSystem::SqDistPointAABB(glm::vec3& p,glm::vec3& Amin,glm::vec3& Amax)
    {
        float sqDist = 0.0f;

        for(int i=0;i<3;i++)
        {
            float v = p[i];

            if(v< Amin[i]) sqDist += (Amin[i]-v)*(Amin[i]-v);
            if(v>Amax[i]) sqDist += (v-Amax[i])*(v-Amax[i]);    

        }

        return sqDist;
    }

    void CollisionSystem::ClosestPtPointOBB(glm::vec3& p,glm::vec3& Acenter,glm::mat3& Arotations,glm::vec3& Ae,glm::vec3& q)
    {
        glm::vec3 d = p-Acenter;

        q = Acenter;

        

        for(int i=0;i<3;i++)
        {
            float dist = glm::dot(d,glm::vec3(Arotations[i][0],Arotations[i][1],Arotations[i][2]));

            if(dist>Ae[i]) dist = Ae[i];
            if(dist<-Ae[i]) dist = -Ae[i];

            q += dist*glm::vec3(Arotations[i][0],Arotations[i][1],Arotations[i][2]);

        }
    }

    float CollisionSystem::SqDistPointOBB(glm::vec3& p,glm::vec3& Acenter,glm::mat3& Arotations,glm::vec3& Ae)
    {
        glm::vec3 v = p-Acenter;

        float sqdist = 0.0f;

        for(int i=0;i<3;i++)
        {
                float d = glm::dot(v,glm::vec3(Arotations[i][0],Arotations[i][1],Arotations[i][2]));
                float excess = 0.0f;

                if(d<-Ae[i])
                {
                    excess = d + Ae[i];
                }
                else if(d > Ae[i])
                {
                    excess = d - Ae[i];
                }

                sqdist += excess*excess;
        }

        return sqdist;
    }

    void CollisionSystem::ClosestPtPointRect(glm::vec3& p,glm::vec3& Rcenter,glm::vec3& Ru,glm::vec3& Rv,float Rex,float Rey,glm::vec3& q)
    {
        glm::vec3 d = p-Rcenter;

        q = Rcenter;

        float dist = glm::dot(d,Ru);

        if(dist>Rex) dist = Rex;
        if(dist<-Rex)dist = -Rex;

        q += dist*Ru;

        dist = glm::dot(d,Rv);

        if(dist>Rey)dist = Rey;
        if(dist<-Rey)dist = -Rey;

        q+= dist*Rv;
    }

    void CollisionSystem::ClosestPtPointRect(glm::vec3& p,glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3& q)
    {
        glm::vec3 ab = b-a;
        glm::vec3 ac = c-a;

        glm::vec3 d = p-a;

        q = a;

        float dist = glm::dot(d,ab);
        float maxdist = glm::dot(ab,ab);
        if(dist>=maxdist)
        {
            q+=ab;
        }
        else if(dist>0.0f)
        {
            q +=(dist/maxdist)*ab;
        }


        dist = glm::dot(d,ac);
        maxdist = glm::dot(ac,ac);
        if(dist>=maxdist)
        {
            q+= ac;
        }
        else if(dist>0.0f)
        {
            q += (dist/maxdist)*ac;
        }

    }

    glm::vec3 CollisionSystem::ClosestPtPointTriangle(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c)
    {
        glm::vec3 ab = b-a;
        glm::vec3 ac = c-a;
        glm::vec3 ap = p-a;
        float d1 = glm::dot(ab,ap);
        float d2 = glm::dot(ac,ap);
        if(d1<=0.0f && d2<=0.0f) return a;

        glm::vec3 bp = p-b;
        float d3 = glm::dot(ab,bp);
        float d4 = glm::dot(ac,bp);
        if(d3>=0.0f && d4<=d3) return b;

        float vc = d1*d4 - d3*d2;
        if(vc<=0.0f && d1>=0.0f && d3<=0.0f)
        {
            float v = d1/(d1-d3);

            return a + v*ab;
        }

        glm::vec3 cp = p-c;
        float d5 = glm::dot(ab,cp);
        float d6 = glm::dot(ac,cp);
        if(d6>=0.0f && d5<=d6) return c;

        float vb = d5*d2 - d1*d6;
        if(vb<=0.0f && d2>=0.0f && d6<=0.0f)
        {
            float w = d2/(d2-d6);

            return a + w*ac;
        }

        float va = d3*d6 - d5*d4;
        if(va<=0.0f && (d4-d3)>=0.0f && (d5-d6)>=0.0f)
        {
            float w = (d4-d3)/((d4-d3)+(d5-d6));
            return b + w*(c-b);
        }

        float denom = 1.0f/(va+vb+vc);
        float v = vb*denom;
        float w = vc*denom;

        return a + ab*v + ac*w;

    }

    int CollisionSystem::PointOutsideOfPlane(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 d)
    {
        float signp = glm::dot(p-a,glm::cross(b-a,c-a));
        float signd = glm::dot(d-a,glm::cross(b-a,c-a));

        return signp*signd < 0.0f;
    }

    glm::vec3 CollisionSystem::ClosestPtPointTetrahedron(glm::vec3 p,glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 d)
    {
        glm::vec3 closestPt = p;
        float bestSqDst = 100000000.0f;

        if(PointOutsideOfPlane(p,a,b,c,d))
        {
            glm::vec3 q = ClosestPtPointTriangle(p,a,b,c);

            float sqDist = glm::dot(q-p,q-p);

            if(sqDist<bestSqDst)
            {
                bestSqDst = sqDist;
                closestPt = q;
            }
        }

        if(PointOutsideOfPlane(p,a,c,d,b))
        {
            glm::vec3 q = ClosestPtPointTriangle(p,a,c,d);

            float sqDist = glm::dot(q-p,q-p);

            if(sqDist<bestSqDst)
            {
                bestSqDst = sqDist;
                closestPt = q;
            }
        }

         if(PointOutsideOfPlane(p,a,d,b,c))
        {
            glm::vec3 q = ClosestPtPointTriangle(p,a,d,b);

            float sqDist = glm::dot(q-p,q-p);

            if(sqDist<bestSqDst)
            {
                bestSqDst = sqDist;
                closestPt = q;
            }
        }

        if(PointOutsideOfPlane(p,b,d,c,a))
        {
            glm::vec3 q = ClosestPtPointTriangle(p,b,d,c);

            float sqDist = glm::dot(q-p,q-p);

            if(sqDist<bestSqDst)
            {
                bestSqDst = sqDist;
                closestPt = q;
            }
        }

        return closestPt;

    }






}