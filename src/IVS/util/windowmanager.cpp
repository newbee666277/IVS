#include "windowmanager.h"
WindowManager* WindowManager::instance = nullptr;

WindowManager *WindowManager::getInstance()
{
    if(WindowManager::instance == nullptr){
        WindowManager::instance = new WindowManager;
    }

    return WindowManager::instance;
}

void WindowManager::releaseInstance()
{
    if(WindowManager::instance != nullptr){
        delete WindowManager::instance;
        WindowManager::instance = nullptr;
    }
}

WindowManager::WindowManager()
{

}

WindowManager::~WindowManager()
{

}

