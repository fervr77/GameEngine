#ifndef INPUT_H
#define INPUT_H
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"
#include <GLFW/glfw3.h>
#include "Window.h"


namespace qvt
{
    class Input
    {
        public:


            static bool IsKeyPressed(int key);
            static bool IsMouseButtonPressed(int button);
            static float GetMouseX();
            static float GetMouseY();
            static uint32_t GetWindowWidth();
            static uint32_t GetWindowHeight();
            static GLFWwindow* GetWindow();
    };

}
#endif