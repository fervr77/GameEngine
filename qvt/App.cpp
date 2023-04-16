#include "Core/Application.h"
#include "Core/EntryPoint.h"

class UserApp: public qvt::Application
{
    public:
        UserApp()
        {

        }

        ~UserApp()
        {

        }

};


qvt::Application* qvt::CreateApplication()
{
    return new UserApp();
}