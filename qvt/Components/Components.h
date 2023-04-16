#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <glm/glm.hpp>
#include "Renderer/MaterialLibrary.h"
#include "Renderer/MeshLibrary.h"
#include "Systems/TerrainQuadTree.h"
#include "Scene/Camera.h"

#include <implot.h>
#include <implot_internal.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "Scene/ScriptableEntity.h"





namespace qvt
{
    struct TransformComponent
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        float angle;
        bool isStatic;

        TransformComponent();
        
    };

    struct ParentComponent
    {
        std::vector<Entity> children;

    };

    struct ChildComponent
    {
        Entity parent;
    };

    struct GameObjectNameComponent
    {
        std::string objectName;

        GameObjectNameComponent(std::string n);
    };

    struct CameraComponent
    {
        Camera* camera;

        CameraComponent(glm::vec3 pos);
        ~CameraComponent();

    };

    struct PbrStaticRendererComponent
    {
        MaterialLibrary::PbrMaterial material;
        uint32_t handle;
        std::string m_name;

        PbrStaticRendererComponent(const std::string& name,uint32_t id);
        ~PbrStaticRendererComponent();



    };

    struct UUIDComponent
    {
        uint32_t id;

    };

    struct TagComponent
    {
        std::string name;

    };

    struct MeshComponent
    {
        unsigned int VAO;
        unsigned int numIndices;
        unsigned int VBO; // used for static instancing
        std::string meshName;
        float radius;

       


        MeshComponent(const std::string& name);
    
    };

    struct SimpleRendererComponent
    {
        MaterialLibrary::SimpleMaterial material;
        uint32_t handle;
        std::string m_name;

        SimpleRendererComponent(const std::string& name,uint32_t id);
        ~SimpleRendererComponent();
    };

    struct PbrRendererComponent
    {
        MaterialLibrary::PbrMaterial material;
        uint32_t handle;
        std::string m_name;

        PbrRendererComponent(const std::string& name,uint32_t id);
        ~PbrRendererComponent();

    };

    struct MoveComponent
    {
        glm::vec4 dummy;

    };

    struct TerrainComponent
    {
        TerrainQuadTree quadTree;

        TerrainComponent(unsigned int numRootnodes,float scaleXZ,float scaleY);



    };

    struct Live2DGraphComponent
    {
        struct Buffer
        {
            int maxSize;
            int offset;
            ImVector<ImVec2> data;

            Buffer(int size=2000)
            {
                maxSize= size;
                offset = 0;
                data.reserve(maxSize);
            }

            void AddPoint(float x,float y)
            {
                if(data.size()<maxSize)
                {
                    data.push_back(ImVec2(x,y));
                }
                else
                {
                    data[offset] = ImVec2(x,y);
                    offset = (offset+1)%maxSize;
                }

            }

            void Erase()
            {
                if(data.size()>0)
                {
                    data.shrink(0);
                    offset = 0;

                }
            }

        };

        std::unordered_map<std::string,Buffer> dataMap;
        std::unordered_map<std::string,float*> valueMap;
        std::unordered_map<std::string,bool> boolMap;
        std::unordered_map<std::string,std::vector<std::string>> graphMap;
        std::unordered_map<std::string,std::pair<int,int>> rangesMap;
        bool showGraphs;
        

        Live2DGraphComponent();
        void AddGraph(const std::string& graphName,float* value,const std::string& valueName);
        void RemoveGraph(const std::string& graphName,const std::string& valueName);
        ~Live2DGraphComponent();
        void OnImGuiRenderer();





    };

    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;
        std::string behaviorName;
        std::function<ScriptableEntity*()> InstantiateScript;

        NativeScriptComponent(const std::string& b="")
        {
            behaviorName = b;

        }

        ~NativeScriptComponent()
        {

        }


    };

    struct ScriptComponent
    {
        std::vector<NativeScriptComponent> scripts;

        ScriptComponent() = default;

    };

    struct CircleShapeComponent
    {
        struct Vertex
        {
            float x;
            float y;
            float z;
        };

        float radius;
        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int numVertices;

        CircleShapeComponent(float r,glm::vec3 c=glm::vec3(1,1,1));


    };

    struct TriangleShapeComponent
    {
        glm::vec3 p1;
        glm::vec3 p2;
        glm::vec3 p3;
        glm::vec3 color;

        unsigned int VAO;
        unsigned int VBO;
        unsigned int numVertices;

        TriangleShapeComponent(glm::vec3 p11,glm::vec3 p22,glm::vec3 p33,glm::vec3 c = glm::vec3(1,1,1));

    };

    struct AABBoxComponent
    {
        glm::vec3 min;
        glm::vec3 max;
        std::string meshName;

        //For debuggig purposes
        unsigned int VAO;
        unsigned int VBO;
        unsigned int numVertices;
        glm::vec3 color;


        AABBoxComponent(const std::string& name);



    };

    struct SphereBVComponent
    {
        glm::vec3 center;
        float radius;
        std::string meshName;

        //For debugging purposes
        glm::vec3 color;

        SphereBVComponent(const std::string& name);
    };

    struct OBBoxComponent
    {
        glm::vec3 center;
        glm::mat3 rotations;
        glm::vec3 e;
        std::string meshName;

        //For debugging purposes
        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int numVertices;

        OBBoxComponent(const std::string& name);

    };

    struct CapsuleBVComponent
    {
        glm::vec3 a;
        glm::vec3 b;
        float radius;
        std::string meshName;

        //For debugging purposes
        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int numVertices;

        CapsuleBVComponent(const std::string& name);


    };

    struct PointsRendererComponent
    {
        std::vector<glm::vec3> points;
        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;

        PointsRendererComponent(glm::vec3 c=glm::vec3(1,1,1));
        void UpdatePoints();

    };

    struct LineStripRendererComponent
    {
        std::vector<glm::vec3> points;
        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;

        LineStripRendererComponent(glm::vec3 c=glm::vec3(1,1,1));
        void UpdatePoints();

    };

    struct LineSegmentPrimitiveComponent
    {
       glm::vec3 a;
       glm::vec3 b;
       glm::vec3 color;

       unsigned int VAO;
       unsigned int VBO;

       LineSegmentPrimitiveComponent(glm::vec3 pa,glm::vec3 pb,glm::vec3 c = glm::vec3(1,1,1));

    };

    struct PointPrimitiveComponent
    {
        
        glm::vec3 color;

        PointPrimitiveComponent(glm::vec3 c=glm::vec3(1,1,1));
    };

    struct PlanePrimitiveComponent
    {
        glm::vec3 normal;
        glm::vec3 color;

        PlanePrimitiveComponent(glm::vec3 n=glm::vec3(0,1,0),glm::vec3 c=glm::vec3(1,1,1));
    };

    struct Rectangle3DPrimitiveComponent
    {
        glm::vec3 u;
        glm::vec3 v;
        glm::vec2 e;
        glm::vec3 color;

        unsigned int VAO;
        unsigned int VBO;


        Rectangle3DPrimitiveComponent(glm::vec3 c = glm::vec3(1,1,1));
    

    };

    struct TrianglePrimitiveComponent
    {
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 c;

        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;

        TrianglePrimitiveComponent(glm::vec3 c = glm::vec3(1,1,1));

    };

    struct TetrahedronPrimitiveComponent
    {
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 c;
        glm::vec3 d;

        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;

        TetrahedronPrimitiveComponent(glm::vec3 c = glm::vec3(1,1,1));

    };

    struct LinePrimitiveComponent
    {
        glm::vec3 a;
        glm::vec3 b;

        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;

        LinePrimitiveComponent(glm::vec3 aP=glm::vec3(0,0,0),glm::vec3 bP=glm::vec3(1,1,1),glm::vec3 c = glm::vec3(1,1,1));


    };

    struct RayPrimitiveComponent
    {
        glm::vec3 a;
        glm::vec3 b;

        glm::vec3 color;
        unsigned int VAO;
        unsigned int VBO;

        RayPrimitiveComponent(glm::vec3 aP = glm::vec3(0,0,0),glm::vec3 bP=glm::vec3(1,1,1),glm::vec3 c = glm::vec3(1,1,1));

    };

   

    

}
#endif