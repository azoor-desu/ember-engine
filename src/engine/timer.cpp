#include "timer.h"
#include "util/types.h"
#include <chrono>
#include <ctime>

EMB_NAMESPACE_START

Timer::Timer() noexcept
{
    ResetTimer();
}

void Timer::SetTargetFramerate(embU32 targetFPS) noexcept
{
    if (targetFPS <= 0)
        return;

    if (targetFPS > Clock::period::den)  // don't pass 0 into time
        return;

    m_TargetFrameTime = ClockDuration(Clock::period::den / targetFPS).count();
}

void Timer::SetTargetSimRate(embU32 targetTPS) noexcept
{
    if (targetTPS <= 0)
        return;

    if (targetTPS > Clock::period::den) // don't pass 0 into time
        return;

    m_TargetSimTime = ClockDuration(Clock::period::den / targetTPS).count();
}

embF32 Timer::GetTargetFramerate() const noexcept
{
    return (embF32)Clock::period::den / (embF32)m_TargetFrameTime;
}

embF32 Timer::GetTargetSimRate() const noexcept
{
    return (embF32)Clock::period::den / (embF32)m_TargetSimTime;
}

embF32 Timer::GetTargetFrameTime() const noexcept
{
    return (embF32)m_TargetFrameTime / (embF32)Clock::period::den;
}
embF32 Timer::GetTargetSimTime() const noexcept
{
    return (embF32)m_TargetSimTime / (embF32)Clock::period::den;
}

embF32 Timer::GetDT() const noexcept
{
    return (embF32)m_DT / (embF32)Clock::period::den;
}
embF32 Timer::GetFixedDT() const noexcept
{
    return (embF32)m_TargetSimTime / (embF32)Clock::period::den;
}

embF32 Timer::GetRealTimeElapsed() const noexcept
{
    return (embF32)m_RealTimeElapsed / (embF32)Clock::period::den;
}

embF32 Timer::GetSimTimeElapsed() const noexcept
{
    return (embF32)m_SimTimeElapsed / (embF32)Clock::period::den;
}

void Timer::SetSimTimeScale(embF32 timeScale) noexcept
{
    if (timeScale < 0)
        return;

    m_TimeScale = timeScale;
}
embF32 Timer::GetSimTimeScale() const noexcept
{
    return m_TimeScale;
}

embBool Timer::ShouldUpdate(embBool simActive) noexcept
{
    // How work:
    // Engine constantly checks if is able to update. If yes, run update, and fixed update if possible too.
    // Updates will always run at inconsistent timings due to the intervals always being slightly longer than the framerate cap
    // Not a problem for rendering, but a problem for simulation.
    // Fixed Updates will run only on Updates. These special updates track exactly how much time was "missed out" during these intervals.
    // It will strive to only run as close to a fixed time step as possible. It will never run exactly on a perfect time step due to updates being inconsistent.
    // Therefore, there is a interp amount variable that is provided to help provide a more accurate physics value in between 2 fixed time steps.

    // In the event of missed fixed time steps due to Update lagging, the engine will attempt to run FixedUpdate multiple times in the next Update.
    // In such an event, the interp amount will default to 1 for all the fixed updates except for the last fixed update, where it predicts one more fixed update into the future.
    // If the time taken for each fixed update is too long, the engine will skip some fixed updates to avoid an infinite lag loop.

    // calculate frame DT each cpu clock cycle
    const ClockDurationType currentTimeEpoch = Clock::now().time_since_epoch().count();
    ClockDurationType currentDT = currentTimeEpoch - m_LastUpdateTimePointEpoch;

    // every cpu cycle check if can render frame or continue waiting.
    if (currentDT < m_TargetFrameTime)
        return false;

    m_DT = currentDT;
    m_LastUpdateTimePointEpoch = currentTimeEpoch;
    m_RealTimeElapsed = currentTimeEpoch - m_AppStartTimePoint; // save real time now to keep subsequent calls for real time consistent

    // Update simulation time variables
    // When time scaled, the number of fixed steps increase/decrease
    //      > Implication: If slow down, lesser steps are taken, making workload lighter. If increase, more steps are taken, increasing workload.
    //      > Speeding up time implies can imply that physics sims can happen multiple times per frame if cranked up too high.
    if (simActive)
        m_SimTimeElapsed += m_DT * m_TimeScale;

    // Perform sim if any
    m_SimStepCount = 0; // reset step count for frame
    constexpr embU32 MAX_SIM_STEPS = 2;

    if (m_TimeScale > 0.f && m_SimTimeElapsed >= m_LastFixedUpdateTime)
    {
        // Check how many steps to do. Need at least 1 so +1, and add any additional counts with the division
        // -1 before division to avoid rolling back 0 nanoseconds.
        m_SimStepCount = 1 + (embU32)((m_SimTimeElapsed - m_LastFixedUpdateTime - 1) / m_TargetSimTime);

        // if too many simulations to be done for this frame, perform only max sims
        // roll back simulated time to the max sim time.
        if (m_SimStepCount > MAX_SIM_STEPS)
        {
            printf("Warning: Skipping %d planned tick(s), rolled back by %ldns (%lf ticks)\n",
                   m_SimStepCount - MAX_SIM_STEPS,
                   m_SimTimeElapsed - (m_LastFixedUpdateTime + m_TargetSimTime * MAX_SIM_STEPS),
                   (embF64)(m_SimTimeElapsed - (m_LastFixedUpdateTime + m_TargetSimTime * MAX_SIM_STEPS)) / (embF64)m_TargetSimTime);
            // Note: by right, if m_SimTimeElapsed is directly equal to Max Sim time, the engine should
            // be simulating the next tick, making the engine simulate MAX_SIM_STEPS + 1 this frame.
            // To be pedantic, m_SimTimeElapsed should be set to a little BEFORE the max sim time.
            // However, we'd want to reduce the time skip as much as possible. Plus, the missing timestep would be triggered the next frame anyway.
            // Logic may seem inconsistent but this is an edge case and the side effects are not consiquential.
            m_SimTimeElapsed = m_LastFixedUpdateTime + m_TargetSimTime * MAX_SIM_STEPS;
            m_SimStepCount = MAX_SIM_STEPS;
        }

        // Compute next update time
        m_LastFixedUpdateTime += m_TargetSimTime * m_SimStepCount;
    }

    // printf("real time %lf, dt: %f, fixeddt: %f, timescale %f, sim time %lf, sim count %u, interp: %f\n",
    //        GetRealTimeElapsed(),
    //        GetDT(),
    //        GetFixedDT(),
    //        GetSimTimeScale(),
    //        GetSimTimeElapsed(),
    //        ShouldFixedUpdate(),
    //        GetFixedUpdateInterpAmount());

    return true;
}

embU32 Timer::ShouldFixedUpdate() const noexcept
{
    return m_SimStepCount;
}

embF32 Timer::GetFixedUpdateInterpAmount() const noexcept
{
    return 1.0f - ((embF32)(m_LastFixedUpdateTime - m_SimTimeElapsed) / (embF32)m_TargetSimTime);
}

void Timer::ResetTimer() noexcept
{
    m_LastUpdateTimePointEpoch = Clock::now().time_since_epoch().count();
    m_AppStartTimePoint = Clock::now().time_since_epoch().count();
    m_SimTimeElapsed = 0;
    m_RealTimeElapsed = 0;
    m_LastFixedUpdateTime = 0;
    m_TimeScale = 1.0f;
    m_SimStepCount = 0;
}

EMB_NAMESPACE_END
