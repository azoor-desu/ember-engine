#include "engine/engineclock.h"
#include "engine/resourcemanager.h"
#include "pch-engine.h"

#include <GLFW/glfw3.h>

#include "util/macros.h"

#include "engine.h"
#include "graphics.h"
#include "util/types.h"
#include "window.h"

EMB_NAMESPACE_START

void Engine::Init()
{
    m_IsEngineRunning = true;

    // init all managers
    // TODO: Probably make them all inherit IManager class and then do a loop to init.
    WindowManager::Instance().Init();
    Graphics::Instance().Init();

    // Rest of Engine init logic here
    // Registering RESOURCE stuffs.

    ResourceHandle test = ResourceManager::Instance().GetResourceHandle(1234, 1234);

    // Post-init stuff
    EngineClock::Instance()
        .ResetTimer(); // avoid the "skipping tick" warning due to long startup
}

void Engine::Update()
{
}

void Engine::Render()
{
    Graphics::Instance().Render(); // do i need this layer lmao
}

void Engine::Destroy() noexcept
{
    Graphics::Instance().Destroy();
    WindowManager::Instance().Destroy();
}

bool Engine::IsEngineRunning() const noexcept
{
    return m_IsEngineRunning;
}

bool Engine::IsSimulationActive() const noexcept
{
    return m_IsSimulationActive;
}

void Engine::SetSimulationActive(embBool active) noexcept
{
    m_IsSimulationActive = active;
}

void Engine::SignalEngineStop()
{
    m_IsEngineRunning = false;
}

EMB_NAMESPACE_END
