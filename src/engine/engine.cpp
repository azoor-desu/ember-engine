#include "pch-engine.h"

#include <GLFW/glfw3.h>

#include "util/defines.h"

#include "engine.h"
#include "graphics.h"
#include "window.h"


EMB_NAMESPACE_START

void Engine::Init()
{
    m_IsEngineRunning = true;

    WindowManager::Instance().Init();
    Graphics::Instance().Init();
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

EMB_NAMESPACE_END
