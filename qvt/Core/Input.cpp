#include "Input.h"
#include "Application.h"


namespace qvt
{
    bool Input::IsKeyPressed(int key)
    {
        auto window = Application::Get().GetActiveWindow()->GetNativeWindow();
        bool state = glfwGetKey(window,key);
        return state==GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        auto window = Application::Get().GetActiveWindow()->GetNativeWindow();
        bool state = glfwGetMouseButton(window,button);
        return state==GLFW_PRESS;
    }

    float Input::GetMouseX()
    {
        auto window = Application::Get().GetActiveWindow()->GetNativeWindow();
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        return xpos;
    }

    float Input::GetMouseY()
    {
        auto window = Application::Get().GetActiveWindow()->GetNativeWindow();
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);

        return ypos;
    }

    uint32_t Input::GetWindowWidth()
    {
        auto window = Application::Get().GetActiveWindow();

        return window->GetWidth();
    }

    uint32_t Input::GetWindowHeight()
    {
        auto window = Application::Get().GetActiveWindow();

        return window->GetHeight();
    }

    GLFWwindow* Input::GetWindow()
    {
        auto window = Application::Get().GetActiveWindow()->GetNativeWindow();

        return window;
    }

}