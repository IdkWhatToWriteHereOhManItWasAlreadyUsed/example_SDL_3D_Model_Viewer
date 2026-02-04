#include "core/Engine.h"
#include <iostream>
#include <thread>

#include <OpenGLSomethingFrameDisplayerEVO/OpenGLSomethingFrameDisplayerEVO.h>
#include <Global.h>

constexpr int WINDOW_WIDTH = 240;
constexpr int WINDOW_HEIGHT = 180;

void Lab1Thread()
{
    /*
    Global::frameDisplayer = std::make_unique<OpenGLSomethingFrameDisplayerEVO::OpenGLSomethingFrameDisplayerEVO>();
    Global::frameDisplayer->SetVideoSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    Global::frameDisplayer->InitialiseGame(1200, 800);
    Global::frameDisplayer->Start();
    */
    std::string path = "model.obj";
    std::cout << "Enter path to model:\n";
    //  std::getline(std::cin, path);
    Engine engine;
    engine.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    engine.LoadModel(path);
    Global::frameDisplayer->WaitForGameInit();
    engine.MainLoop();
}

int main(int, char**)
{   
    /*
    std::string path = "model.obj";
    std::cout << "Enter path to model:\n";
    //  std::getline(std::cin, path);
    Engine engine;
    engine.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    engine.LoadModel(path);
    */
    
    Global::frameDisplayer = std::make_unique<OpenGLSomethingFrameDisplayerEVO::OpenGLSomethingFrameDisplayerEVO>();
    Global::frameDisplayer->SetVideoSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    Global::frameDisplayer->InitialiseGame(1200, 800);

    std::thread mainThread([] {Lab1Thread();});

    Global::frameDisplayer->Start();
    
    //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    //engine.MainLoop();

    mainThread.join();

    return 0;
}
