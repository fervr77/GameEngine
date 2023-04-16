#include <glad/glad.h>
#include "TerrainNode.h"
#include <iostream>
#include "Systems/RendererSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace qvt
{
    unsigned int TerrainNode::counter = 0;

    TerrainNode::TerrainNode(TerrainNode* parent,int lod,float* index,int numRootNodes,unsigned int VAO,glm::vec3 localPosition,float scaleXZ,float scaleY)
    {
       
        this->parent = parent;
        this->lod = lod;
        this->VAO = VAO;
        this->localPosition = localPosition;
        this->index[0] = index[0];
        this->index[1] = index[1];
        this->numRootNodes = numRootNodes;
        this->gap = (1.0f/(numRootNodes*pow(2,lod)));
        this->isLeaf = true;
        this->localScale = glm::vec3(gap,0,gap);
        this->scaleXZ = scaleXZ;
        this->scaleY = scaleY;

       



    }

    TerrainNode::~TerrainNode()
    {
        
    }

    void TerrainNode::addChildren()
    {
        

        if(isLeaf)
        {
            isLeaf = false;
        }

        if(children.size()==0)
        {
            
           


            for(int i=0;i<2;i++)
            {
                for(int j=0;j<2;j++)
                {
                    float indices[2];
                    indices[0] = i;
                    indices[1] = j;
                    TerrainNode node(this,(lod+1),indices,numRootNodes,VAO,glm::vec3(localPosition.x+i*gap/2.0f,0.0f,localPosition.z+j*gap/2.0f),scaleXZ,scaleY);
                    children.push_back(node);
                    counter++;
                    

                }
            }

            

        }



    }


    void TerrainNode::removeChildren()
    {
        if(!isLeaf)
        {
            isLeaf  = true;
        }

        if(children.size()!=0)
        {
          
            counter -= 4;

            children.clear();
           
        }

    }

    void TerrainNode::render()
    {
        if(isLeaf)
        {
            glm::mat4 localMatrix = glm::mat4(1.0f);
            localMatrix = glm::translate(localMatrix,localPosition);
            localMatrix = glm::scale(localMatrix,localScale);

            glUniformMatrix4fv(RendererSystem::pbrUniformMap["terrain_localMatrix"],1,GL_FALSE,glm::value_ptr(localMatrix));

            glUniform2f(RendererSystem::pbrUniformMap["terrain_index"],index[0],index[1]);
            glUniform1f(RendererSystem::pbrUniformMap["terrain_gap"],gap);
            glUniform1i(RendererSystem::pbrUniformMap["terrain_lod"],lod);
            glUniform1f(RendererSystem::pbrUniformMap["terrain_scaleY"],scaleY);
            glUniform2f(RendererSystem::pbrUniformMap["terrain_location"],localPosition.x,localPosition.z);
            

            glBindVertexArray(VAO);
            glDrawArrays(GL_PATCHES,0,9);

            
        }

        for(auto& child:children)
        {
            child.render();
        }
    }

    void TerrainNode::update(glm::vec3 distance,int* lod_ranges,glm::vec3 worldPosition)
    {
        
        glm::vec3 d = distance-glm::vec3(localPosition.x*scaleXZ+worldPosition.x,localPosition.y*scaleY+worldPosition.y,localPosition.z*scaleXZ+worldPosition.z);

        float dist = d.x*d.x+d.y*d.y+d.z*d.z;



        float ldr = lod_ranges[lod]*lod_ranges[lod];
       
        

        if(dist<ldr)
        {
            addChildren();
        }

        else if(dist>ldr)
        {
            removeChildren();
        }

        for(auto& child:children)
        {
           

            child.update(distance,lod_ranges,worldPosition);
        }


    }
}