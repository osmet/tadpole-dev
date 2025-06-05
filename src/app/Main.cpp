#include "Precompiled.h"
#include "App.h"

int main()
{
    auto app = std::make_unique<app::App>();
    app->Initialize();
    app->Run();

    return 0;
}