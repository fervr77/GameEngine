#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H
#include "Application.h"

extern qvt::Application* qvt::CreateApplication();

int main(int argc,char** argv)
{

    auto app = qvt::CreateApplication();
    app->Run();

    delete app;
    
    return 0;
}


#endif