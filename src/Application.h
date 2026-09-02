

#pragma once

#include <Windows.h>
#include "../core/AutoClicker.h"
#include "../core/InputManager.h"

class Application
{
public:
    Application();
    ~Application();


    void Init(HWND hwnd);
    //created by nxthxndev(https://github.com/nxthxndev)

    void Update();


    void OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    AutoClicker& GetAutoClicker() { return m_autoClicker; }
    InputManager& GetInputManager() { return m_inputManager; }

private:
    AutoClicker  m_autoClicker;
    InputManager m_inputManager;
};
