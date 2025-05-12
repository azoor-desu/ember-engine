#pragma once

#include "util/defines.h"
#include "util/types.h"

EMB_NAMESPACE_START

class Engine
{
  public:
    // Initializes engine. Run once.
    void Init();

    // Run once to update game logic once. Possible to block due to framerate cap.
    void Update();

    // Run right after Update. Depends on the framerate controller in Update.
    void Render();

    // Destroys all resources and exits.
    void Exit() noexcept;

    // Is the application running
    bool IsEngineRunning() const noexcept;
    // Is the simulation currently running? Setting to false suspends simulation, and can be resumed later.
    bool IsSimulationActive() const noexcept;
    // Sets simulation to run or suspend.
    void SetSimulationActive(embBool active) noexcept;

  private:
    embBool m_IsEngineRunning = false;
    embBool m_IsSimulationActive = false;
};

EMB_NAMESPACE_END
