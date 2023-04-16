#ifndef WINDOW_H
#define WINDOW_H
#include <GLFW/glfw3.h>
#include <string>
#include "Events/Event.h"
#include <functional>


namespace qvt
{
    class Window
    {
        using EventCallback = std::function<void(Event&)>;

        public:

        Window(const std::string& name,uint32_t width,uint32_t height);
        ~Window();
        uint32_t GetWidth()const;
        uint32_t GetHeight()const;
        GLFWwindow* GetNativeWindow();
        void SetVSync(bool);
        bool IsVSync();
        void OnUpdate();
        void SetEventCallback(const EventCallback&);
        void Init();
        void Shutdown();



        private:
            struct WindowData
            {
                uint32_t width;
                uint32_t height;
                std::string title;
                EventCallback callback;
                bool vsync;

            };

        GLFWwindow* window;
        WindowData m_data;

        friend class Application;




    };

}

#endif