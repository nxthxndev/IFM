

#pragma once

class Application;

class UI
{
public:
    UI() = default;

    //created by nxthxndev(https://github.com/nxthxndev)
    void Draw(Application& app);

private:
    void DrawMainWindow(Application& app);


    bool m_capturingHotkey = false;
};
