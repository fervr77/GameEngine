
#include <glad/glad.h>
#include "Window.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

namespace qvt
{
    Window::Window(const std::string& name,uint32_t width,uint32_t height)
    {
        m_data.title = name;
        m_data.width = width;
        m_data.height = height;
    }

    Window::~Window()
    {
        Shutdown();
    }

    uint32_t Window::GetWidth()const
    {
        return m_data.width;
    }

    uint32_t Window::GetHeight()const
    {
        return m_data.height;
    }

    GLFWwindow* Window::GetNativeWindow()
    {
        return window;
    }

    void Window::SetVSync(bool value)
    {
        m_data.vsync = value;
        glfwSwapInterval(value);
    }

    bool Window::IsVSync()
    {
        return m_data.vsync;
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();

        glfwSwapBuffers(window);


    }

    void Window::SetEventCallback(const EventCallback& f)
    {
        m_data.callback = f;
    }

    void Window::Init()
    {
        glfwInit();
        window = glfwCreateWindow(m_data.width,m_data.height,m_data.title.c_str(),nullptr,nullptr);
        glfwMakeContextCurrent(window);

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSetWindowUserPointer(window,&m_data);

        

        glfwSetWindowCloseCallback(window,[](GLFWwindow* window)
        {
            WindowCloseEvent e;
            auto data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.callback(e);

        });

        glfwSetFramebufferSizeCallback(window,[](GLFWwindow* window,int width,int height)
        {
            auto data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;
            WindowResizedEvent e(width,height);
            
            data.callback(e);

        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
                    data.callback(e);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.callback(e);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.callback(e);
					break;
				}
				}

				

			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				auto data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.callback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.callback(e);
					break;

				}
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent e((float)xoffset, (float)yoffset);

				data.callback(e);


			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent e((float)xpos, (float)ypos);
				data.callback(e);

			});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent e(character);
				data.callback(e);


			});


        glViewport(0,0,m_data.width,m_data.height);

       //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);











    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(window);
        glfwTerminate();

    }

}