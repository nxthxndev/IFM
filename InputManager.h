

#pragma once

#include <Windows.h>
#include <functional>
#include <cstdint>
#include <string>

class InputManager
{
public:
    InputManager();
    ~InputManager();


    void Init(HWND hwnd);


    bool SetToggleHotkey(UINT vkCode, UINT modifiers = 0);


    void OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    void SetOnToggleCallback(std::function<void()> callback) { m_onToggle = std::move(callback); }



    void BeginCapture();

    //created by nxthxndev(https://github.com/nxthxndev)
    bool IsCapturing() const { return m_capturing; }

    UINT GetCurrentVkCode() const { return m_currentVk; }
    UINT GetCurrentModifiers() const { return m_currentMods; }

    
    std::string GetCurrentKeyName() const;

private:
    static constexpr int HOTKEY_ID_TOGGLE = 1;

    void Unregister();
    static std::string VkCodeToName(UINT vkCode);

    HWND m_hwnd = nullptr;
    bool m_registered = false;
    UINT m_currentVk = 0;
    UINT m_currentMods = 0;

    bool m_capturing = false;

    std::function<void()> m_onToggle;
};