#ifndef APPLICATION_H
#define APPLICATION_H
#include "Core/Window.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Game/GameLayer.h"
#include "ImGui/ImGuiLayer.h"

namespace qvt
{
    class Scene;
    
    class Application
    {
        public:
            Application();
            virtual ~Application();
            void Run();
            void OnEvent(Event& e);
            static Application& Get(){return *instance;}
            bool OnWindowClose(WindowCloseEvent&);
            bool OnWindowResize(WindowResizedEvent&);
            bool OnKeyPressed(KeyPressedEvent&);
            void PushLayer(Layer* layer);
            void PushOverlay(Layer* layer);
            ImGuiLayer* GetImGuiLayer(){return imguiLayer;}
            Window* GetActiveWindow();
            void Close(){m_running = false;}





        private:
            static Application* instance;
            Window* window;
            bool m_running = true;
            bool m_minimized = false;
            LayerStack layerStack;
            GameLayer* gameLayer;
            Scene* scene;
            Layer* userApp;
            ImGuiLayer* imguiLayer;
            float lastTime = 0.0f;
            static bool RunTimeMode;

            friend class GameLayer;



            



    };

    static Application* CreateApplication();

}
#endif