#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include "stb_image.h"
#include "Game/GameLayer.h"
#include "Components/Components.h"
#include <functional>
#include "Core/Logger.h"
#include <iostream>
#include "Renderer/ShaderLibrary.h"
#include "Core/Application.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include <cstdlib>
#include "Core/Input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer/ShaderLoader.h"
#include "Panel.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>


namespace qvt
{
    float GameLayer::m_deltaTime;
    int GameLayer::hoveredEntity;
    int GameLayer::gizmotype = -1;
    Camera* GameLayer::editorCamera;
    Camera* GameLayer::mainCamera;
    glm::vec2 GameLayer::m_viewportSize = { 0.0f,0.0f };
    Framebuffer* GameLayer::framebuffer;
    Scene* GameLayer::scene;
    bool GameLayer::isCameraOn = false;
    unsigned int GameLayer::cameraQuadVAO;
    unsigned int GameLayer::cameraQuadVBO;
    unsigned int GameLayer::cameraQuadEBO;
    unsigned int GameLayer::cameraQuadTexture;
    
   
   
    GameLayer::GameLayer(Scene* sc):Layer("GameLayer")
    {
         scene = sc;
         editorCamera = new Camera(glm::vec3(0,10,0),(float)Application::Get().GetActiveWindow()->GetWidth()/(float)Application::Get().GetActiveWindow()->GetHeight());
         rendererSystem = new RendererSystem(scene);
         cameraSystem = new CameraSystem(editorCamera,scene);
         lightSystem = new LightSystem();
         terrainSystem = new TerrainSystem(scene);
         scriptSystem = new ScriptSystem(scene);
         collisionSystem = new CollisionSystem(scene);
         mainCamera = editorCamera;
         mainCamera->SetAsPrimary();

         panel = new Panel(mainCamera,scene);

          spec.width = 1920;
          spec.height = 1080;
          framebuffer = Framebuffer::Create(spec);

        m_viewportSize.x = spec.width;
        m_viewportSize.y = spec.height;
        framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);

        //zedCameraInitParameters.camera_resolution = sl::RESOLUTION::HD720;
        //zedCameraInitParameters.camera_fps = 60;
        
        //pthread_create(&zedThread,nullptr,getZedImage,nullptr);
       


        float vertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};

    unsigned int indices[] = {0,3,1,
                            3,2,1};

    glGenVertexArrays(1,&cameraQuadVAO);
    glGenBuffers(1,&cameraQuadVBO);
    glGenBuffers(1,&cameraQuadEBO);

    glBindVertexArray(cameraQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER,cameraQuadVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cameraQuadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


    glGenTextures(1,&cameraQuadTexture);
    glBindTexture(GL_TEXTURE_2D,cameraQuadTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_BGRA, GL_UNSIGNED_BYTE,nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);
    

    




    




        


                




    }

    GameLayer::~GameLayer()
    {
        delete scene;
    }

    void GameLayer::OnAttach()
    {

       
        
        
        rendererSystem->PbrStaticPrepare();

    
        
    }

    void GameLayer::SetMainCameraAspectRatio(float ar)
    {
        mainCamera->UpdateAspectRatio(ar);
    }
        
    

    void GameLayer::OnDetach()
    {
        
    }

    void GameLayer::OnViewportResizeEvent()
    {
        framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
        mainCamera->UpdateAspectRatio((float)m_viewportSize.x/(float)m_viewportSize.y);
    }

    
    void GameLayer::OnUpdate(float ts)
    {
        //std::cout << ts << std::endl; 
        
            if (!Application::RunTimeMode)
            {
                framebuffer->Bind();

                //Resize
                spec = framebuffer->GetSpecification();
               
                
                if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && (m_viewportSize.x != spec.width))
                {
                    
                    
                    framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);


                    mainCamera->UpdateAspectRatio((float)m_viewportSize.x/(uint32_t)m_viewportSize.y);
                
                }
        
                int value = -1;
                glClearBufferiv(GL_COLOR, 1, &value);


                
            }

        if(m_viewporthovered || Application::RunTimeMode)
        {
            cameraSystem->OnUpdate(ts);
        }
        

        terrainSystem->Update(ts);
        scriptSystem->Update(ts);
        collisionSystem->OnUpdate(ts);

            
        /*if(isCameraOn && isZedCameraOpen)
        {
            
            sl::Mat zedImage(zedCamera.getCameraInformation().camera_resolution,sl::MAT_TYPE::U8_C4);
            cv::Mat cvImageFlipped;
            cv::Mat cvImage;

            if(zedCamera.grab()==sl::ERROR_CODE::SUCCESS)
            {
               
                zedCamera.retrieveImage(zedImage,sl::VIEW::LEFT);
                cvImageFlipped = slMat2cvMat(zedImage);
                cv::flip(cvImageFlipped,cvImage,0);

                int width = zedImage.getWidth();
                int height = zedImage.getHeight();

                glBindTexture(GL_TEXTURE_2D,cameraQuadTexture);
                glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_BGRA,GL_UNSIGNED_BYTE,cvImage.data);
                glGenerateMipmap(GL_TEXTURE_2D); 
               
            }


        }*/




        rendererSystem->OnUpdate(ts);

         
            if (!Application::RunTimeMode)
            {
                

                framebuffer->Unbind();

            }

        



    }

    void GameLayer::OnEvent(Event& event)
    {
        EventHandler handler(event);
        handler.Handle<KeyPressedEvent>(std::bind(&GameLayer::OnKeyPressedEvent,this,std::placeholders::_1));
        handler.Handle<MouseButtonPressedEvent>(std::bind(&GameLayer::OnMousePressedEvent,this,std::placeholders::_1));
        handler.Handle<MouseMovedEvent>(std::bind(&GameLayer::OnMouseMovedEvent,this,std::placeholders::_1));
        handler.Handle<WindowResizedEvent>(std::bind(&GameLayer::OnWindowResizedEvent,this,std::placeholders::_1));
        handler.Handle<MouseScrolledEvent>(std::bind(&GameLayer::OnMouseScrolledEvent,this,std::placeholders::_1));
        handler.Handle<MouseButtonReleasedEvent>(std::bind(&GameLayer::OnMouseReleasedEvent,this,std::placeholders::_1));
    }

    void GameLayer::OnImGuiRender()
    {
            static bool dockspaceopen = true;
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceopen, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }


            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.

                    if (ImGui::MenuItem("New","Ctrl+N"))
                    {
                        //NewScene();

                    }

                    if (ImGui::MenuItem("Open...","Ctrl+O"))
                    {
                        //OpenScene();

                    }

                    if (ImGui::MenuItem("Save As...","Ctrl+Shift+S"))
                    {
                        //SaveSceneAs();
        
                    }


                    if (ImGui::MenuItem("Exit"))Application::Get().Close();

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("Camera"))
                {
                    if(ImGui::MenuItem("Switch camera ON"))
                    {
                        /*isCameraOn = true;
                        sl::ERROR_CODE err = zedCamera.open(zedCameraInitParameters);

                        if(err!=sl::ERROR_CODE::SUCCESS)
                        {
                            std::cout << "Zed camera not opened" << std::endl;
                            isZedCameraOpen = false;
                        }
                        else
                        {
                           
                            isZedCameraOpen = true;
                            zedCameraImage = sl::Mat(zedCamera.getCameraInformation().camera_resolution,sl::MAT_TYPE::U8_C4);

                            
                            
                        }*/

                        


                    }

                    if(ImGui::MenuItem("Switch camera OFF"))
                    {
                        /*isCameraOn = false;

                        zedCamera.close();
                        isZedCameraOpen = false;*/

                    }


                    ImGui::EndMenu();
                }



                ImGui::EndMenuBar();
            }

           
            panel->OnImGuiRender();

            

            
            

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
            ImGui::Begin("Level editor");
            auto viewportoffset = ImGui::GetCursorPos();

            m_viewportfocused = ImGui::IsWindowFocused();
            m_viewporthovered = ImGui::IsWindowHovered();
            
            Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportfocused && !m_viewporthovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            
            if (m_viewportSize != *(glm::vec2*)&viewportPanelSize)
            {
                
                m_viewportSize = { viewportPanelSize.x,viewportPanelSize.y };
                
            }

            uint32_t textureID = framebuffer->GetColorAttachmentRendererID();
            
            ImGui::Image((void*)textureID, ImVec2{ m_viewportSize.x,m_viewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

            auto windowSize = ImGui::GetWindowSize();
            ImVec2 minBound = ImGui::GetWindowPos();
            minBound.x += viewportoffset.x;
            minBound.y += viewportoffset.y;

            ImVec2 maxBound = { minBound.x + windowSize.x,minBound.y + windowSize.y };
            m_viewportBounds[0] = { minBound.x,minBound.y };
            m_viewportBounds[1] = { maxBound.x,maxBound.y };

        

            // GIZMOS
            Entity selectedEntity = panel->GetSelectedEntity();
            if ((int)selectedEntity.GetEntityID()>=0 && gizmotype != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                float windowWidth = (float)ImGui::GetWindowWidth();
                float windowHeight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                glm::mat4 cameraView = CameraSystem::viewMatrix;
                glm::mat4 cameraProjection = CameraSystem::projectionMatrix;

                // Entity transform

                auto& tr = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform,tr.position);
                glm::mat4 rot = glm::toMat4(glm::quat(tr.rotation));
                transform = transform*rot; 
                transform = glm::scale(transform,tr.scale);
                
                
                //Snapping
                bool snap = Input::IsKeyPressed(Key::LeftControl);
                float snapvalue = 0.5f;
                if (gizmotype == ImGuizmo::OPERATION::ROTATE)
                {
                    snapvalue = 45.0f;
                }

                float snapValues[3] = { snapvalue,snapvalue,snapvalue };

            
                
                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),(ImGuizmo::OPERATION)gizmotype, ImGuizmo::LOCAL,
                    glm::value_ptr(transform),nullptr,snap ? snapValues : nullptr);

                if (ImGuizmo::IsUsing() && !tr.isStatic)
                {
                

                    glm::vec3 translation, rotation, scale;
                        DecomposeTransform(transform, translation, rotation, scale);

                        glm::vec3 deltaRotation = rotation - tr.rotation;


                            tr.position = translation;
                            tr.scale = scale;
                            tr.rotation.x += deltaRotation.x;
                            tr.rotation.y += deltaRotation.y;
                            tr.rotation.z += deltaRotation.z;

                            
                    
                    
                
                }


            }




            ImGui::PopStyleVar();

            ImGui::End();

            ImGui::End();
            

    }

    bool GameLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        

        bool cameraSystemStatus = cameraSystem->OnKeyPressedEvent(e);

        bool wireFrameMode = false;

        if(e.GetKeyCode()==Key::KP0)
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

            wireFrameMode = true;


        }

        if(e.GetKeyCode()==Key::KP1)
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

            wireFrameMode = true;
        }

        if(e.GetKeyCode()==Key::KP2)
        {
            RendererSystem::renderBVs = true;
        }

        if(e.GetKeyCode()==Key::KP3)
        {
            RendererSystem::renderBVs = false;
        }

        if(e.GetKeyCode()==Key::KP4)
        {
            RendererSystem::renderPrimitives = true;
        }

        if(e.GetKeyCode()==Key::KP5)
        {
            RendererSystem::renderPrimitives = false;
        }

        if(!Application::RunTimeMode)
        {
            if(e.GetKeyCode()==Key::F)
            {
                panel->FocusCameraOnObject();
            }

            if(e.GetKeyCode()==Key::Q)
            {
                gizmotype = -1;
                
            }

            else if(e.GetKeyCode()==Key::T)
            {
                gizmotype = 0;

            }

            else if(e.GetKeyCode()==Key::E)
            {
                gizmotype = 2;
            }

            else if(e.GetKeyCode()==Key::R)
            {
                gizmotype= 1;
            }

            if(e.GetKeyCode()==Key::Delete)
           {

            
            panel->DeleteEntity();
            gizmotype = -1;
            

           }

           

           if(Input::IsKeyPressed(Key::LeftControl))
           {
               if(e.GetKeyCode()==Key::C)
               {
                   panel->CopySelectedEntity();
               }
           }
        }

        
        
        return cameraSystemStatus || wireFrameMode;
    }

    bool GameLayer::OnMousePressedEvent(MouseButtonPressedEvent& e)
    {
       
            
        if(e.GetMouseButton()==Mouse::ButtonLeft && !Application::RunTimeMode && m_viewporthovered && (!ImGuizmo::IsOver() || gizmotype==-1))
        {
            

            auto [mx,my] = ImGui::GetMousePos();

            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;
            my = -1*(my-m_viewportSize.y);

            mousePressedPos[0] = (int)mx;
            mousePressedPos[1] = (int)my;

            
        }
         

        return false;
    }

    bool GameLayer::OnMouseReleasedEvent(MouseButtonReleasedEvent& e)
    {
        
        
        if(e.GetMouseButton()==Mouse::ButtonLeft && !Application::RunTimeMode && m_viewporthovered && (!ImGuizmo::IsOver() || gizmotype==-1))
        {
            

            auto [mx,my] = ImGui::GetMousePos();

            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;
            my = -1*(my-m_viewportSize.y);

            mouseReleasedPos[0] = (int)mx+1;
            mouseReleasedPos[1] = (int)my-1;

            framebuffer->Bind();

            

            bool valueFound = false;

            for(int i=mousePressedPos[1];i>=mouseReleasedPos[1];i--)
            {
                for(int j=mousePressedPos[0];j<=mouseReleasedPos[0];j++)
                {
                    if(j>=0 && i>=0 && j<m_viewportSize.x && i<m_viewportSize.y)
                    {
                        int pixel = framebuffer->ReadPixel(j,i)-1;

                        if(pixel!=-1)
                        {
                            hoveredEntity = pixel;
                            valueFound = true;
                            break;
                            
                        }

                        
                    }
                }

                if(valueFound)
                {
                    break;
                }

                hoveredEntity = -1;
            }

            if(scene->m_registry.valid((entt::entity)hoveredEntity))
            {
                Entity entity{(entt::entity)hoveredEntity,scene};
                panel->SetSelectedEntity(entity);
                gizmotype = 0;
                return true;
            }
            else
            {
                panel->SetSelectedEntity(Entity{(entt::entity)-1,scene});
                gizmotype = -1;
                return true;
            }


            framebuffer->Unbind();

            

            
        }

    }

    bool GameLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        bool cameraSystemStatus = cameraSystem->OnMouseScrolledEvent(e);

        return cameraSystemStatus;
    }

    bool GameLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
       
               
        bool cameraSystemStatus = cameraSystem->OnMouseMovedEvent(e);
            

        

        return cameraSystemStatus;
    }

    bool GameLayer::OnWindowResizedEvent(WindowResizedEvent& e)
    {
        bool cameraSystemStatus = cameraSystem->OnWindowResizedEvent(e);
        bool renderSystemStatus = rendererSystem->OnWindowResizedEvent(e);

        return cameraSystemStatus || renderSystemStatus;

    }

    bool GameLayer::DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
    {
        // From glm::decompose in matrix_decompose.inl

			using namespace glm;
			using T = float;

			mat4 LocalMatrix(transform);

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (
				epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3] = static_cast<T>(1);
			}

			// Next take care of translation (easy).
			translation = vec3(LocalMatrix[3]);
			LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

			vec3 Row[3], Pdum3;

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					Row[i][j] = LocalMatrix[i][j];

			// Compute X scale factor and normalize first row.
			scale.x = length(Row[0]);
			Row[0] = detail::scale(Row[0], static_cast<T>(1));
			scale.y = length(Row[1]);
			Row[1] = detail::scale(Row[1], static_cast<T>(1));
			scale.z = length(Row[2]);
			Row[2] = detail::scale(Row[2], static_cast<T>(1));

			// At this point, the matrix (in rows[]) is orthonormal.
			// Check for a coordinate system flip.  If the determinant
			// is -1, then negate the matrix and the scaling factors.
#if 0
			Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
			if (dot(Row[0], Pdum3) < 0)
			{
				for (length_t i = 0; i < 3; i++)
				{
					scale[i] *= static_cast<T>(-1);
					Row[i] *= static_cast<T>(-1);
				}
			}
#endif

			rotation.y = asin(-Row[0][2]);
			if (cos(rotation.y) != 0) {
				rotation.x = atan2(Row[1][2], Row[2][2]);
				rotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else {
				rotation.x = atan2(-Row[2][0], Row[1][1]);
				rotation.z = 0;
			}


			return true;
			


		
    }

    

   

   
    


    
		


}