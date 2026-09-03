

#pragma once

#include <atomic>
#include <thread>
#include <cstdint>


enum class MouseButton
{
    Left = 0,
    Right = 1,
    Middle = 2
};

// Mode de clic
enum class ClickMode
{
    Single = 0,  
    Hold = 1    //created by nxthxndev(github.com/nxthxndev)
};


enum class SpeedMode
{
    Interval = 0, 
    ClicksPerSecond = 1 
};

// Parametres configurables de l'autoclicker
struct AutoClickerSettings
{
    SpeedMode   speedMode = SpeedMode::Interval;
    int         intervalMinMs = 50;   
    int         intervalMaxMs = 50;   
    int         clicksPerSecond = 10;    
    MouseButton button = MouseButton::Left;
    ClickMode   mode = ClickMode::Single;
    int         repeatCount = 0;   
    bool        clickAtCursor = true; 
};

class AutoClicker
{
public:
    AutoClicker();
    ~AutoClicker();

  
    void Start();

    
    void Stop();

  
    void Toggle();

    
    bool IsRunning() const { return m_running.load(); }

    //created by nxthxndev(https://github.com/nxthxndev)
    uint64_t GetClickCount() const { return m_clickCount.load(); }


    AutoClickerSettings& Settings() { return m_settings; }
    const AutoClickerSettings& Settings() const { return m_settings; }

private:
    void ThreadLoop();              
    void PerformClick();          
    long long ComputeNextIntervalUs() const; 

    AutoClickerSettings m_settings;

    std::atomic<bool>      m_running;
    std::atomic<uint64_t>  m_clickCount;
    std::thread            m_thread;
};
