#include "pch-engine.h"
#include "util/defines.h"

#include "engine.h"

EMB_NAMESPACE_START

void Engine::Init()
{
    m_IsEngineRunning = true;
}

void Engine::Update()
{
}

void Engine::Render()
{
}

void Engine::Exit() noexcept
{
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
