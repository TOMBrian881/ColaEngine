#include "log/LogSystem.h"
#include "Window.h"
#include "Application.h"



int main()
{
    Cola::WindowCreateInfo windowInfo;
    std::shared_ptr<Cola::Application> app = std::make_shared<Cola::Application>(windowInfo);

    app->Run();    
}