
#include "AutoClicker.h"

#ifndef NOMINMAX
#define NOMINMAX 
#endif
#include <Windows.h>
#include <random>
#include <algorithm>

#pragma comment(lib, "winmm.lib") 

AutoClicker::AutoClicker()
    : m_running(false)
    , m_clickCount(0)
{
}

AutoClicker::~AutoClicker()
{
    Stop();
}

void AutoClicker::Start()
{
    if (m_running.load())
        return; 

    m_clickCount.store(0);
    m_running.store(true);


    if (m_thread.joinable())
        m_thread.join();

    m_thread = std::thread(&AutoClicker::ThreadLoop, this);
}

void AutoClicker::Stop()
{
    m_running.store(false);
    if (m_thread.joinable())
        m_thread.join();
}

void AutoClicker::Toggle()
{
    if (IsRunning())
        Stop();
    else
        Start();
}

long long AutoClicker::ComputeNextIntervalUs() const
{

    if (m_settings.speedMode == SpeedMode::ClicksPerSecond)
    {
        int cps = m_settings.clicksPerSecond;
        if (cps < 1) cps = 1;
        if (cps > 1000) cps = 1000;
        long long us = 1000000LL / cps;
        return std::max<long long>(1, us);
    }

    int lo = m_settings.intervalMinMs;
    int hi = m_settings.intervalMaxMs;

    if (hi < lo)
        std::swap(lo, hi);

    lo = std::max(1, lo);

    long long resultMs;
    if (lo == hi)
    {
        resultMs = lo;
    }
    else
    {
     
        static thread_local std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(lo, hi);
        resultMs = dist(rng);
    }

    return resultMs * 1000LL; 
}

void AutoClicker::PerformClick()
{
    INPUT input[2] = {};
    DWORD downFlag = 0, upFlag = 0;

    switch (m_settings.button)
    {
    case MouseButton::Left:
        downFlag = MOUSEEVENTF_LEFTDOWN;
        upFlag = MOUSEEVENTF_LEFTUP;
        break;
    case MouseButton::Right:
        downFlag = MOUSEEVENTF_RIGHTDOWN;
        upFlag = MOUSEEVENTF_RIGHTUP;
        break;
    case MouseButton::Middle:
        downFlag = MOUSEEVENTF_MIDDLEDOWN;
        upFlag = MOUSEEVENTF_MIDDLEUP;
        break;
    }

    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = downFlag;

    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = upFlag;

    if (m_settings.mode == ClickMode::Single)
    {
       
        ::SendInput(2, input, sizeof(INPUT));
        m_clickCount.fetch_add(1);
    }
    else 
    {

        ::SendInput(1, &input[0], sizeof(INPUT));
        m_clickCount.fetch_add(1);
    }
}

void AutoClicker::ThreadLoop()
{
    
    if (m_settings.mode == ClickMode::Hold)
    {
        INPUT downInput = {};
        INPUT upInput = {};
        downInput.type = INPUT_MOUSE;
        upInput.type = INPUT_MOUSE;

        switch (m_settings.button)
        {
        case MouseButton::Left:
            downInput.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            upInput.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        case MouseButton::Right:
            downInput.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            upInput.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        case MouseButton::Middle:
            downInput.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            upInput.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            break;
        }

        ::SendInput(1, &downInput, sizeof(INPUT));
        m_clickCount.fetch_add(1);

        while (m_running.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        ::SendInput(1, &upInput, sizeof(INPUT));
        return;
    }

    ::timeBeginPeriod(1);

    LARGE_INTEGER freq;
    ::QueryPerformanceFrequency(&freq);

    LARGE_INTEGER nextTick;
    ::QueryPerformanceCounter(&nextTick);

    while (m_running.load())
    {
        PerformClick();


        if (m_settings.repeatCount > 0 &&
            m_clickCount.load() >= static_cast<uint64_t>(m_settings.repeatCount))
        {
            m_running.store(false);
            break;
        }

        long long waitUs = ComputeNextIntervalUs();


        long long ticksToAdd = (waitUs * freq.QuadPart) / 1000000LL;
        nextTick.QuadPart += ticksToAdd;

        LARGE_INTEGER now;
        ::QueryPerformanceCounter(&now);


        while (now.QuadPart < nextTick.QuadPart && m_running.load())
        {
            long long remainingUs = ((nextTick.QuadPart - now.QuadPart) * 1000000LL) / freq.QuadPart;

            if (remainingUs > 2000)
            {
                //created by nxthxndev(https://github.com/nxthxndev)
                std::this_thread::sleep_for(std::chrono::microseconds(remainingUs - 1000));
            }
            else
            {
                
                std::this_thread::yield();
            }

            ::QueryPerformanceCounter(&now);
        }


        if (now.QuadPart > nextTick.QuadPart)
            nextTick = now;
    }

    ::timeEndPeriod(1);
}