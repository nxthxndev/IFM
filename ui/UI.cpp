

#include "UI.h"
#include "../src/Application.h"
#include "imgui.h"
#include <Windows.h>

void UI::Draw(Application& app)
{
    DrawMainWindow(app);
}

void UI::DrawMainWindow(Application& app)
{
    AutoClicker& clicker = app.GetAutoClicker();
    AutoClickerSettings& settings = clicker.Settings();
    InputManager& input = app.GetInputManager();

    ImGuiIO& io = ImGui::GetIO();


    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("IFM - InFinite Macro | https://github.com/nxthxndev", nullptr, flags);


    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.35f, 0.60f, 1.0f));
    ImGui::BeginChild("##titlebar", ImVec2(0, 24), false, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(8, 4));
    ImGui::TextUnformatted("IFM - InFinite Macro | https://github.com/nxthxndev");


    float closeBtnWidth = 22.0f;
    ImGui::SameLine(ImGui::GetWindowWidth() - closeBtnWidth - 6);
    ImGui::SetCursorPosY(2);
    if (ImGui::Button("X", ImVec2(closeBtnWidth, 20)))
    {
        ::PostQuitMessage(0);
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Spacing();


    bool running = clicker.IsRunning();
    ImGui::TextColored(
        running ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImVec4(0.9f, 0.3f, 0.3f, 1.0f),
        running ? "STATUS: RUNNING" : "STATUS: STOPPED"
    );
    ImGui::SameLine();
    ImGui::Text("(clicks: %llu)", (unsigned long long)clicker.GetClickCount());

    ImGui::Separator();


    if (running)
    {
        if (ImGui::Button("Stop", ImVec2(120, 32)))
            clicker.Stop();
    }
    else
    {
        if (ImGui::Button("Start", ImVec2(120, 32)))
            clicker.Start();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();


    ImGui::Text("Hotkey");
    ImGui::SameLine();

    if (input.IsCapturing())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "[ Press any key... ]");
        ImGui::TextDisabled("(Esc to cancel)");
    }
    else
    {
        ImGui::Text(": %s", input.GetCurrentKeyName().c_str());
        ImGui::SameLine();
        if (ImGui::Button("Set Hotkey"))
        {
            input.BeginCapture();
        }
    }
    ImGui::TextDisabled("Toggles Start/Stop from anywhere, even when the window isn't focused.");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();


    ImGui::Text("Speed mode:");
    ImGui::SameLine();
    int speedModeIdx = static_cast<int>(settings.speedMode);
    const char* speedModeNames[] = { "Interval (ms)", "Clicks/sec" };
    ImGui::SetNextItemWidth(160);
    if (ImGui::Combo("##speedmode", &speedModeIdx, speedModeNames, IM_ARRAYSIZE(speedModeNames)))
        settings.speedMode = static_cast<SpeedMode>(speedModeIdx);

    ImGui::Spacing();

    if (settings.speedMode == SpeedMode::Interval)
    {
        ImGui::Text("Interval (ms)");
        ImGui::SetNextItemWidth(150);
        ImGui::InputInt("##interval", &settings.intervalMinMs);
        if (settings.intervalMinMs < 1) settings.intervalMinMs = 1;
        //created by nxthxndev(https://github.com/nxthxndev)
        settings.intervalMaxMs = settings.intervalMinMs;
    }
    else 
    {
        ImGui::Text("Clicks per second (1 - 1000)");
        ImGui::SetNextItemWidth(150);
        ImGui::InputInt("##cps", &settings.clicksPerSecond);
        if (settings.clicksPerSecond < 1)    settings.clicksPerSecond = 1;
        if (settings.clicksPerSecond > 1000) settings.clicksPerSecond = 1000;


        if (settings.clicksPerSecond >= 400)
        {
            ImGui::TextColored(
                ImVec4(1.0f, 0.55f, 0.15f, 1.0f),
                "Warning: values this high may cause instability or bugs\n"
                "in some applications/games. Use with caution."
            );
        }
    }

    ImGui::Spacing();


    ImGui::Text("Button");
    int buttonIdx = static_cast<int>(settings.button);
    const char* buttonNames[] = { "Left", "Right", "Middle" };
    ImGui::SetNextItemWidth(150);
    if (ImGui::Combo("##button", &buttonIdx, buttonNames, IM_ARRAYSIZE(buttonNames)))
        settings.button = static_cast<MouseButton>(buttonIdx);

    ImGui::SameLine();


    ImGui::Text("Mode");
    ImGui::SameLine();
    int modeIdx = static_cast<int>(settings.mode);
    const char* modeNames[] = { "Single", "Hold" };
    ImGui::SetNextItemWidth(150);
    if (ImGui::Combo("##mode", &modeIdx, modeNames, IM_ARRAYSIZE(modeNames)))
        settings.mode = static_cast<ClickMode>(modeIdx);

    ImGui::Spacing();


    ImGui::Text("Number of clicks (0 = infinite)");
    ImGui::SetNextItemWidth(150);
    ImGui::InputInt("##repeat", &settings.repeatCount);
    if (settings.repeatCount < 0) settings.repeatCount = 0;

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::TextDisabled("FPS: %.1f", io.Framerate);

    ImGui::End();
}
