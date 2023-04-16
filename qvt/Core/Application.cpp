#include <glad/glad.h>
#include "Application.h"
#include "Logger.h"
#include "Core/Input.h"
#include "Scene/Scene.h"
#include "Applications/App1/App1.h"

namespace qvt
{
    Application* Application::instance = nullptr;
    bool Application::RunTimeMode = false;

    Application::Application()
    {
        instance = this;
        scene = new Scene();
        Logger::Init();
        window = new Window("QVT ENGINE",1920,1080);
        window->Init();
        window->SetEventCallback(std::bind(&Application::OnEvent,this,std::placeholders::_1));
        window->SetVSync(false);
        gameLayer = new GameLayer(scene);
        imguiLayer = new ImGuiLayer();
        userApp = new App1(scene);
        PushLayer(gameLayer);
        PushOverlay(imguiLayer);
        PushLayer(userApp);



    }


    Application::~Application()
    {
        delete window;
    }

    void Application::Run()
    {
        while(m_running)
        {
            float time = (float)glfwGetTime();
            float deltaTime = time-lastTime;
            lastTime = time;

            if(!m_minimized)
            {
                for(Layer* layer:layerStack)
                {
                    layer->OnUpdate(deltaTime);
                }
            }

            if(!RunTimeMode)
            {
                imguiLayer->Begin();

                for(Layer* layer:layerStack)
                {
                    layer->OnImGuiRender();
                }



                imguiLayer->End();
            }

            window->OnUpdate();
            
            if(Input::IsKeyPressed(Key::Escape))
            {
                m_running = false;
            }
        }


    }

    void Application::OnEvent(Event& e)
    {
        EventHandler handler(e);
        handler.Handle<WindowCloseEvent>(std::bind(&Application::OnWindowClose,this,std::placeholders::_1));
        handler.Handle<WindowResizedEvent>(std::bind(&Application::OnWindowResize,this,std::placeholders::_1));
        handler.Handle<KeyPressedEvent>(std::bind(&Application::OnKeyPressed,this,std::placeholders::_1));

        for(auto it = layerStack.rbegin();it!=layerStack.rend();it++)
        {
            if(e.Handled)
            {
                return;
            }

            (*it)->OnEvent(e);
        }

    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        
        return true;
    }

    bool Application::OnWindowResize(WindowResizedEvent& e)
    {
        if(e.GetWidth()<=0 || e.GetHeight()<=0)
        {
            m_minimized = true;
        }
        else
        {
            m_minimized = false;
        }

        

        return false;
    }

    bool Application::OnKeyPressed(KeyPressedEvent& e)
    {
        if(e.GetKeyCode()==Key::F10)
        {
            if(RunTimeMode==false)
            {
                RunTimeMode = true;
                glViewport(0,0,1920,1080);
                gameLayer->SetMainCameraAspectRatio((float)1920/(float)1080);

            }
            else
            {
                gameLayer->OnViewportResizeEvent();
                RunTimeMode = false;
                

            }
        }
        
        

        return false;
    }

    Window* Application::GetActiveWindow()
    {
        return window;
    }

    void Application::PushLayer(Layer* layer)
    {
        layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        layerStack.PushOverlay(layer);
        layer->OnAttach();
    }






}