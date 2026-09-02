// ============================================================================
// IFM - InFinite Macro
// core/AutoClicker.h
//
// Coeur logique de l'autoclicker : gere un thread qui simule des clics
// souris a intervalle regulier (avec randomisation optionnelle),
// independamment de toute UI.
//
// Timing haute precision : Sleep()/sleep_for() standard sur Windows a une
// granularite par defaut d'environ 15.6ms (le "system timer resolution"),
// ce qui rendait impossible d'atteindre des frequences elevees (au-dela
// d'environ 60-65 clics/sec, la boucle etait "bridee" par cette imprecision,
// pas par SendInput lui-meme qui supporte des dizaines de milliers de clics/sec).
// On utilise donc un timing base sur QueryPerformanceCounter (haute precision,
// resolution microseconde) combine a timeBeginPeriod(1) pour reduire la
// granularite du timer systeme au minimum pendant que l'autoclicker tourne.
// ============================================================================

#pragma once

#include <atomic>
#include <thread>
#include <cstdint>

// Bouton de souris a simuler
enum class MouseButton
{
    Left = 0,
    Right = 1,
    Middle = 2
};

// Mode de clic
enum class ClickMode
{
    Single = 0,   // 1 clic (down+up) a chaque tick
    Hold = 1    // maintient le bouton enfonce tant que actif
};

// Comment la vitesse de clic est exprimee/reglee par l'utilisateur
enum class SpeedMode
{
    Interval = 0, // l'utilisateur regle un intervalle fixe en millisecondes
    ClicksPerSecond = 1  // l'utilisateur regle directement un nombre de clics/seconde
};

// Parametres configurables de l'autoclicker
struct AutoClickerSettings
{
    SpeedMode   speedMode = SpeedMode::Interval;
    int         intervalMinMs = 50;    // intervalle minimum entre deux clics (ms), utilise si speedMode == Interval
    int         intervalMaxMs = 50;    // intervalle maximum (si == min, pas de randomisation)
    int         clicksPerSecond = 10;    // utilise si speedMode == ClicksPerSecond (1 a 1000)
    MouseButton button = MouseButton::Left;
    ClickMode   mode = ClickMode::Single;
    int         repeatCount = 0;     // 0 = infini, sinon nombre de clics avant arret auto
    bool        clickAtCursor = true;  // true = position actuelle du curseur (le seul mode supporte pour l'instant)
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
