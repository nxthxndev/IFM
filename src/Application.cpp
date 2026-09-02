

#include "Application.h"

Application::Application() = default;
Application::~Application() = default;

void Application::Init(HWND hwnd)
{
    m_inputManager.Init(hwnd);


    m_inputManager.SetToggleHotkey(VK_F6, 0);

    m_inputManager.SetOnToggleCallback([this]()
        {   //created by nxthxndev(https://github.com/nxthxndev)
            m_autoClicker.Toggle();
        });
}

void Application::Update()
{

}

void Application::OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    m_inputManager.OnWindowsMessage(msg, wParam, lParam);
}
