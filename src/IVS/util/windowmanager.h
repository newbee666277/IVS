#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#include "../view/mainwidget.h"
#include "../view/loginwidget.h"
#include "../view/settingwidget.h"
#include "../view/logwidget.h"
#include "../view/registerwidget.h"

class WindowManager
{
public:
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
    static WindowManager* getInstance();
    static void releaseInstance();
    MainWidget mainwidget;
    loginWidget loginwidget;
    SettingWidget settingwidget;
    LogWidget logwidget;
    RegisterWidget registerwidget;
private:
    WindowManager();
    ~WindowManager();
    static WindowManager* instance;
};

#endif // WINDOWMANAGER_H
