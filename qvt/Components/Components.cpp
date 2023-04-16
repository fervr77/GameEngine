#include <glad/glad.h>
#include "Components.h"
#include "Systems/RendererSystem.h"
#include "Game/GameLayer.h"
#include "Scene/Scene.h"
#include <fstream>
#include <iostream>
#include "Core/Application.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace qvt
{
    TransformComponent::TransformComponent()
    {
        position = glm::vec3(0,0,0);
        rotation = glm::vec3(0,0,0);
        scale = glm::vec3(1,1,1);
        angle = 0.0f;
        isStatic = false;
    }

    MeshComponent::MeshComponent(const std::string& name)
    {
        std::pair<unsigned int,unsigned int> mesh = MeshLibrary::GetMesh(name);
        float r = MeshLibrary::GetMeshRadius(name);

            VAO = mesh.first;
            numIndices = mesh.second;
            meshName = name;
            radius = r;
           

    }

    GameObjectNameComponent::GameObjectNameComponent(std::string n)
    {
        this->objectName = n;
    }

    


    SimpleRendererComponent::SimpleRendererComponent(const std::string& name,uint32_t id)
    {
            material = *MaterialLibrary::GetSimpleMaterial(name);

            m_name = name;

            RendererSystem::AddToSimpleRenderMap(name,id);

            handle = id;

    }

    SimpleRendererComponent::~SimpleRendererComponent()
    {

        

    }

    PbrRendererComponent::PbrRendererComponent(const std::string& name,uint32_t id)
    {
            material = *MaterialLibrary::GetPbrMaterial(name);

            m_name = name;

            RendererSystem::AddToPbrRenderMap(name,id);

            handle = id;

    }

    PbrRendererComponent::~PbrRendererComponent()
    {

      
    }

    PbrStaticRendererComponent::PbrStaticRendererComponent(const std::string& name,uint32_t id)
    {
            material = *MaterialLibrary::GetPbrMaterial(name);

            m_name = name;

            RendererSystem::AddToPbrStaticRenderMap(name,id);

            handle = id;

    }

    PbrStaticRendererComponent::~PbrStaticRendererComponent()
    {

        

    }

    TerrainComponent::TerrainComponent(unsigned int numNodes,float scaleXZ,float scaleY):quadTree(numNodes,scaleXZ,scaleY)
    {
        
    }

    CameraComponent::CameraComponent(glm::vec3 pos)
    {
        int windowWidth = Application::Get().GetActiveWindow()->GetWidth();
        int windowHeight = Application::Get().GetActiveWindow()->GetHeight();

        camera = new Camera(pos,(float)windowWidth/(float)windowHeight);
    }

    CameraComponent::~CameraComponent()
    {
        delete camera;
    }

    Live2DGraphComponent::Live2DGraphComponent()
    {
       
        showGraphs = false;
    }

    Live2DGraphComponent::~Live2DGraphComponent()
    {

    }

    void Live2DGraphComponent::OnImGuiRenderer()
    {
        if(showGraphs)
        {

        
        ImGui::Begin("Parameters");
        static float t = 0;
        t += ImGui::GetIO().DeltaTime;
        
                 
        for(auto& graph:graphMap)
        {


            if(graph.second.size()>0)
            {
                ImPlot::SetNextPlotLimitsX(t-10,t,ImGuiCond_Always);
                ImPlot::SetNextPlotLimitsY(-100,100);
                if(ImPlot::BeginPlot(graph.first.c_str()))
                {
                    for(auto& v:graph.second)
                    {
                        if(boolMap[v])
                        {
                            dataMap[v].AddPoint(t,*valueMap[v]);
                            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
                            ImPlot::PlotLine(v.c_str(),&dataMap[v].data[0].x,&dataMap[v].data[0].y,dataMap[v].data.size(),dataMap[v].offset,2*sizeof(float));
                            
                        }

                    }




                    ImPlot::EndPlot();
                }

                 




                 
                
            }
                

        }

        ImGui::End();


        }
        
        
        
    }

    void Live2DGraphComponent::AddGraph(const std::string& graphName,float* value,const std::string& valueName)
    {
        if(graphMap.count(graphName)==0)
        {
            graphMap[graphName] = std::vector<std::string>();
        }

        if(dataMap.count(valueName)==0)
        {
            dataMap[valueName] = Buffer();
        }

        if(valueMap.count(valueName)==0)
        {
            valueMap[valueName] = value;
        }

        if(boolMap.count(valueName)==0)
        {
            boolMap[valueName] = false;
        }

        graphMap[graphName].push_back(valueName);

    }

    void Live2DGraphComponent::RemoveGraph(const std::string& graphName,const std::string& valueName)
    {
        if(dataMap.count(valueName)!=0)
        {
            
            dataMap.erase(valueName);
        }

        if(valueMap.count(valueName)!=0)
        {
            

            valueMap.erase(valueName);
        }

        if(boolMap.count(valueName)!=0)
        {
            

            boolMap.erase(valueName);
        }

        if(graphMap.count(graphName)!=0)
        {
            for(int i=0;i<graphMap[graphName].size();i++)
            {
                if(strcmp(valueName.c_str(),graphMap[graphName][i].c_str())==0)
                {
                    

                    graphMap[graphName].erase(graphMap[graphName].begin()+i);
                    break;
                }
            }
        }

        if(graphMap[graphName].size()==0)
        {
            
            graphMap.erase(graphName);   
        }



        
    }


    CircleShapeComponent::CircleShapeComponent(float r,glm::vec3 c)
    {
        radius = r;
        color = c;
        float PI = 3.141516;

        int numPoints = radius*100;

        float step = 2*PI/numPoints;

        std::vector<Vertex> vertices;
        float start = 0.0f;
        for(int i=0;i<numPoints;i++)
        {
            float x = radius*glm::cos(start);
            float z = radius*glm::sin(start);

            start += step;

            vertices.push_back(Vertex{x,0,z});
            
        }

        vertices.push_back(Vertex{radius,0,0});
        numVertices = vertices.size();





        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

    }

    TriangleShapeComponent::TriangleShapeComponent(glm::vec3 p11,glm::vec3 p22,glm::vec3 p33,glm::vec3 c)
    {
        p1 = p11;
        p2 = p22;
        p3 = p33;
        color  = c;

        std::vector<glm::vec3> vertices;
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
        vertices.push_back(p1);
        numVertices = vertices.size();

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

    }

    AABBoxComponent::AABBoxComponent(const std::string& name)
    {
        std::pair<glm::vec3,glm::vec3> ranges = MeshLibrary::GetAABBox(name);
        min = ranges.first;
        max = ranges.second;
        meshName = name;

        // For debugging purposes we draw the mesh

        float vertices[] = 
        {
            // Line strip
            min.x,max.y,min.z, //(1)
            min.x,max.y,max.z, //(2)
            max.x,max.y,max.z, //(3)
            max.x,max.y,min.z, //(4)
            min.x,max.y,min.z, //(1)
            min.x,min.y,min.z, //(5)
            min.x,min.y,max.z, //(6)
            max.x,min.y,max.z, //(7)
            max.x,min.y,min.z, //(8)
            max.x,max.y,min.z, //(4)
            max.x,max.y,max.z, //(3)
            max.x,min.y,max.z, //(7)
            min.x,min.y,max.z, //(6)
            min.x,max.y,max.z, //(2)
            min.x,max.y,min.z, //(1)
            min.x,min.y,min.z, //(5)
            max.x,min.y,min.z //(8)
            

        };

        numVertices = 17;
        color = glm::vec3(1,1,1);

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0); 
    }

    SphereBVComponent::SphereBVComponent(const std::string& name)
    {
        std::pair<glm::vec3,glm::vec3> ranges = MeshLibrary::GetAABBox(name);
        meshName = name;
        this->color = glm::vec3(1,1,1);
        
        glm::vec3 max = ranges.second;
        glm::vec3 min = ranges.first;

        float a = (max.x-min.x)/2.0f;
        float b = (max.y-min.y)/2.0f;
        float c = (max.z-min.z)/2.0f;
        center.x = (a)+min.x;
        center.y = (b)+min.y;
        center.z = (c)+min.z;

        float r = -100000;
        if(a>r)
        {
            r = a;
        }
        if(b>r)
        {
            r = b;
        }
        if(c>r)
        {
            r = c;
        }

        this->radius = r;

    }

    OBBoxComponent::OBBoxComponent(const std::string& name)
    {
        meshName = name;
        color = glm::vec3(1,1,1);

        std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> data = MeshLibrary::GetOBBox(name);

        this->center = data.first;
        this->e = data.second.first;
        this->rotations = data.second.second;

        
        

        glm::vec3 u = e.x*(glm::vec3(rotations[0][0],rotations[0][1],rotations[0][2]));
        glm::vec3 v = e.y*(glm::vec3(rotations[1][0],rotations[1][1],rotations[1][2]));
        glm::vec3 w = e.z*(glm::vec3(rotations[2][0],rotations[2][1],rotations[2][2]));
        

        float minx = center.x-(u.x+v.x+w.x);
        float miny = center.y-(u.y+v.y+w.y);
        float minz = center.z-(u.z+v.z+w.z);
        float maxx = center.x+(u.x+v.x+w.x);
        float maxy = center.y+(u.y+v.y+w.y);
        float maxz = center.z+(u.z+v.z+w.z);

        


        

       
        float vertices[] = 
        {
            // Line strip
            minx,maxy,minz, //(1)
            minx,maxy,maxz, //(2)
            maxx,maxy,maxz, //(3)
            maxx,maxy,minz, //(4)
            minx,maxy,minz, //(1)
            minx,miny,minz, //(5)
            minx,miny,maxz, //(6)
            maxx,miny,maxz, //(7)
            maxx,miny,minz, //(8)
            maxx,maxy,minz, //(4)
            maxx,maxy,maxz, //(3)
            maxx,miny,maxz, //(7)
            minx,miny,maxz, //(6)
            minx,maxy,maxz, //(2)
            minx,maxy,minz, //(1)
            minx,miny,minz, //(5)
            maxx,miny,minz //(8)
        
        };

        numVertices = 17;
        color = glm::vec3(1,1,1);

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0); 

    }

    CapsuleBVComponent::CapsuleBVComponent(const std::string& name)
    {
        this->meshName = name;
        this->color = glm::vec3(1,1,1);

        std::pair<glm::vec3,std::pair<glm::vec3,glm::mat3>> obb = MeshLibrary::GetOBBox(name);

        float maxAxis = -100000;
        unsigned int index = -1;
        unsigned int secIndex = -1;
        unsigned int thirdIndex = -1;

        if(obb.second.first[0]>maxAxis)
        {
            maxAxis = obb.second.first[0];
            index = 0;

            if(obb.second.first[1]>obb.second.first[2])
            {
                this->radius = obb.second.first[1];
                secIndex = 1;
                thirdIndex = 2;
            }
            else
            {
                this->radius = obb.second.first[2];
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
                this->radius = obb.second.first[0];
                secIndex = 0;
                thirdIndex = 2;
            }
            else
            {
                this->radius = obb.second.first[2];
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
                this->radius = obb.second.first[0];
                secIndex = 0;
                thirdIndex = 1;
            }
            else
            {
                this->radius = obb.second.first[1];
                secIndex = 1;
                thirdIndex = 0;
            }
        }



        this->b = obb.first + maxAxis*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);
        this->a = obb.first - maxAxis*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);

        glm::vec3 diff = this->b-obb.first;

        glm::vec3 p1 = obb.first+this->radius*glm::vec3(obb.second.second[secIndex][0],obb.second.second[secIndex][1],obb.second.second[secIndex][2]);
        glm::vec3 p2 = obb.first-this->radius*glm::vec3(obb.second.second[secIndex][0],obb.second.second[secIndex][1],obb.second.second[secIndex][2]);
        glm::vec3 p3 = p1+diff;
        glm::vec3 p4 = p2+diff;
        glm::vec3 p5 = p1-diff;
        glm::vec3 p6 = p2-diff;
        glm::vec3 p7 = b+this->radius*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);
        glm::vec3 p8 = a-this->radius*glm::vec3(obb.second.second[index][0],obb.second.second[index][1],obb.second.second[index][2]);
        glm::vec3 p9 = obb.first+this->radius*glm::vec3(obb.second.second[thirdIndex][0],obb.second.second[thirdIndex][1],obb.second.second[thirdIndex][2]);
        glm::vec3 p10 = obb.first-this->radius*glm::vec3(obb.second.second[thirdIndex][0],obb.second.second[thirdIndex][1],obb.second.second[thirdIndex][2]);
        glm::vec3 p11 = p9+diff;
        glm::vec3 p12 = p10+diff;
        glm::vec3 p13 = p9-diff;
        glm::vec3 p14 = p10-diff;

        std::vector<glm::vec3> vertices;

        vertices.push_back(p1);
        vertices.push_back(p3);
        vertices.push_back(p7);
        vertices.push_back(p4);
        vertices.push_back(p2);
        vertices.push_back(p6);
        vertices.push_back(p8);
        vertices.push_back(p5);
        vertices.push_back(p1);

        vertices.push_back(p9);
        vertices.push_back(p11);
        vertices.push_back(p7);
        vertices.push_back(p12);
        vertices.push_back(p10);
        vertices.push_back(p14);
        vertices.push_back(p8);
        vertices.push_back(p13);
        vertices.push_back(p9);


        
        numVertices = 18;

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0); 


        
    }

    PointsRendererComponent::PointsRendererComponent(glm::vec3 c)
    {
        this->color = c;
        this->points = std::vector<glm::vec3>();

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,100*sizeof(glm::vec3),nullptr,GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
    }

    void PointsRendererComponent::UpdatePoints()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,points.size()*sizeof(glm::vec3),points.data());


    }

    LineStripRendererComponent::LineStripRendererComponent(glm::vec3 c)
    {
        this->color = c;
        this->points = std::vector<glm::vec3>();

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,100*sizeof(glm::vec3),nullptr,GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

    }

    void LineStripRendererComponent::UpdatePoints()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,points.size()*sizeof(glm::vec3),points.data());

    }

    LineSegmentPrimitiveComponent::LineSegmentPrimitiveComponent(glm::vec3 pa,glm::vec3 pb,glm::vec3 c)
    {
        this->a = pa;
        this->b = pb;
        this->color = c;

        std::vector<glm::vec3> vertices;

        vertices.push_back(a);
        vertices.push_back(b);

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);  
    }

    PointPrimitiveComponent::PointPrimitiveComponent(glm::vec3 c)
    {
        this->color = c;
    }

    PlanePrimitiveComponent::PlanePrimitiveComponent(glm::vec3 n,glm::vec3 c)
    {
        this->normal = n;
        this->color = c;
  
    }

    Rectangle3DPrimitiveComponent::Rectangle3DPrimitiveComponent(glm::vec3 c)
    {
        this->color = c;
        this->u = glm::vec3(1,0,0);
        this->v = glm::vec3(0,1,0);
        this->e = glm::vec2(0.5f,0.5f);

        std::vector<glm::vec3> vertices;

        vertices.push_back(glm::vec3(-0.5,0.5f,0.0f));
        vertices.push_back(glm::vec3(0.5f,0.5f,0.0f));
        vertices.push_back(glm::vec3(0.5f,-0.5f,0.0f));
        vertices.push_back(glm::vec3(-0.5f,-0.5f,0.0f));
        vertices.push_back(glm::vec3(-0.5f,0.5f,0.0f));

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

    }

    TrianglePrimitiveComponent::TrianglePrimitiveComponent(glm::vec3 c)
    {
        this->color = c;

        a = glm::vec3(-0.5f,0.0f,0.0f);
        b = glm::vec3(0.0f,0.5f,0.0f);
        c = glm::vec3(0.5f,0.0f,0.0f);

        std::vector<glm::vec3> vertices;

        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
        vertices.push_back(a);

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        
    }

    TetrahedronPrimitiveComponent::TetrahedronPrimitiveComponent(glm::vec3 co)
    {
        this->color = co;

        this->a = glm::vec3(-0.5f,0.0f,0.0f);
        this->b = glm::vec3(0.0f,0.0f,-0.5f);
        this->c = glm::vec3(0.5f,0.0f,0.0f);
        this->d = glm::vec3(0.0f,0.5f,-0.25f);

        std::vector<glm::vec3> vertices;

        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
        vertices.push_back(a);
        vertices.push_back(d);
        vertices.push_back(b);
        vertices.push_back(d);
        vertices.push_back(c);

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

    }

    LinePrimitiveComponent::LinePrimitiveComponent(glm::vec3 aP,glm::vec3 bP,glm::vec3 c)
    {
        this->color = c;
        this->a = aP;
        this->b = bP;

        
        std::vector<glm::vec3> vertices;

        glm::vec3 d = bP-aP;

        glm::vec3 aNew = aP + glm::vec3(d.x*-1000000,d.y*-1000000,d.z*-1000000);
        vertices.push_back(aNew);

        for(int i=-300;i<=300;i+=100)
        {
            glm::vec3 newPoint = aP+glm::vec3(d.x*i,d.y*i,d.z*i);
            vertices.push_back(newPoint);
        }
       

        glm::vec3 bNew = aP + glm::vec3(d.x*1000000,d.y*1000000,d.z*1000000);


        vertices.push_back(bNew);

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
    }

    RayPrimitiveComponent::RayPrimitiveComponent(glm::vec3 aP,glm::vec3 bP,glm::vec3 c)
    {
        this->color = c;
        this->a = aP;
        this->b = bP;

        std::vector<glm::vec3> vertices;

        glm::vec3 d = bP-aP;

        glm::vec3 aNew = aP;
        vertices.push_back(aNew);

        for(int i=1000;i<=6000;i+=1000)
        {
            glm::vec3 newPoint = aP+glm::vec3(d.x*i,d.y*i,d.z*i);
            vertices.push_back(newPoint);
        }
       

        glm::vec3 bNew = aP + glm::vec3(d.x*1000000,d.y*1000000,d.z*1000000);


        vertices.push_back(bNew);

        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),vertices.data(),GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);


    }



    







}