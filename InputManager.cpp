

#include "InputManager.h"

InputManager::InputManager() = default;

InputManager::~InputManager()
{
    Unregister();
}

void InputManager::Init(HWND hwnd)
{
    m_hwnd = hwnd;
}

bool InputManager::SetToggleHotkey(UINT vkCode, UINT modifiers)
{
    if (!m_hwnd)
        return false;

    
    Unregister();

    BOOL ok = ::RegisterHotKey(m_hwnd, HOTKEY_ID_TOGGLE, modifiers, vkCode);
    if (ok)
    {
        m_registered = true;
        m_currentVk = vkCode;
        m_currentMods = modifiers;
    }
    return ok == TRUE;
}

void InputManager::Unregister()
{
    if (m_registered && m_hwnd)
    {
        ::UnregisterHotKey(m_hwnd, HOTKEY_ID_TOGGLE);
        m_registered = false;
    }
}

void InputManager::BeginCapture()
{
    //created by nxthxndev(https://github.com/nxthxndev)
    Unregister();
    m_capturing = true;
}

void InputManager::OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    
    if (m_capturing && (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN))
    {
        UINT vkCode = static_cast<UINT>(wParam);


        if (vkCode == VK_CONTROL || vkCode == VK_MENU || vkCode == VK_SHIFT ||
            vkCode == VK_LWIN || vkCode == VK_RWIN ||
            vkCode == VK_LCONTROL || vkCode == VK_RCONTROL ||
            vkCode == VK_LMENU || vkCode == VK_RMENU ||
            vkCode == VK_LSHIFT || vkCode == VK_RSHIFT)
        {
            return;
        }


        if (vkCode == VK_ESCAPE)
        {
            m_capturing = false;
            SetToggleHotkey(m_currentVk, m_currentMods);
            return;
        }

        m_capturing = false;
        SetToggleHotkey(vkCode, 0);
        return;
    }


    if (msg == WM_HOTKEY && wParam == HOTKEY_ID_TOGGLE)
    {
        if (m_onToggle)
            m_onToggle();
    }
}

std::string InputManager::GetCurrentKeyName() const
{
    if (m_currentVk == 0)
        return "None";
    return VkCodeToName(m_currentVk);
}

std::string InputManager::VkCodeToName(UINT vkCode)
{

    if (vkCode >= VK_F1 && vkCode <= VK_F24)
        return "F" + std::to_string(vkCode - VK_F1 + 1);


    if (vkCode >= '0' && vkCode <= '9')
        return std::string(1, static_cast<char>(vkCode));


    if (vkCode >= 'A' && vkCode <= 'Z')
        return std::string(1, static_cast<char>(vkCode));


    if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9)
        return "Num" + std::to_string(vkCode - VK_NUMPAD0);

    switch (vkCode)
    {
    case VK_SPACE:      return "Space";
    case VK_RETURN:     return "Enter";
    case VK_TAB:        return "Tab";
    case VK_BACK:       return "Backspace";
    case VK_ESCAPE:     return "Escape";
    case VK_CAPITAL:    return "CapsLock";
    case VK_LEFT:       return "Left";
    case VK_RIGHT:      return "Right";
    case VK_UP:         return "Up";
    case VK_DOWN:       return "Down";
    case VK_INSERT:     return "Insert";
    case VK_DELETE:     return "Delete";
    case VK_HOME:       return "Home";
    case VK_END:        return "End";
    case VK_PRIOR:      return "PageUp";
    case VK_NEXT:       return "PageDown";
    case VK_OEM_3:      return "`";
    case VK_OEM_MINUS:  return "-";
    case VK_OEM_PLUS:   return "=";
    case VK_OEM_1:      return ";";
    case VK_OEM_2:      return "/";
    case VK_OEM_4:      return "[";
    case VK_OEM_6:      return "]";
    case VK_OEM_5:      return "\\";
    case VK_OEM_7:      return "'";
    case VK_OEM_COMMA:  return ",";
    case VK_OEM_PERIOD: return ".";
    case VK_XBUTTON1:   return "Mouse4";
    case VK_XBUTTON2:   return "Mouse5";
    default:
        return "Key(" + std::to_string(vkCode) + ")";
    }
}