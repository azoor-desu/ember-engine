#pragma once

#include <chrono>

#include "util/defines.h"
#include "util/types.h"

EMB_NAMESPACE_START

class Timer
{
  public:
    using Clock = std::chrono::high_resolution_clock;  // the thing to pull timings from
    using ClockDuration = Clock::duration; // the time difference stored in std::chrono
    using ClockDurationType = ClockDuration::rep; // the underlying type of the value stored in the clock.
    using ClockTimePoint = Clock::time_point; // the time pulled from the Clock.

    Timer() : m_LastUpdateTimePointEpoch{Clock::now().time_since_epoch().count()}, m_AppStartTimePoint{Clock::now().time_since_epoch().count()} {};

    static Timer& Instance()
    {
        // thread safe singeton
        static Timer timer;
        return timer;
    }

    // Run in a while loop to constantly check if engine should update at this moment in time.
    embBool Tick() noexcept;
    // Run with every engine update to check if Fixed update needs to happen or not this frame.
    // Returns the number of fixed updates to carry out for this frame (if updates were missed)
    embU32 ShouldFixedUpdate() const noexcept;
    // Run every update. Returns the ratio of time between the last fixed update and the next fixed update.
    // Used to interpolate physics objects in-between fixed updates so that they don't look laggy due to a low fixed update value.
    embF32 GetFixedUpdateInterpAmount() const noexcept;

    void SetTargetFramerate(embU32 targetFPS) noexcept;
    void SetTargetSimRate(embU32 targetTPS) noexcept;

    embF32 GetTargetFramerate() const noexcept;
    embF32 GetTargetSimRate() const noexcept;
    embF32 GetTargetFrameTime() const noexcept;
    embF32 GetTargetSimTime() const noexcept;

    embF32 GetDT() const noexcept;
    embF32 GetFixedDT() const noexcept;

    embF32 GetRealTimeElapsed() const noexcept;
    embF32 GetSimTimeElapsed() const noexcept;

    void SetSimTimeScale(embF32 timeScale) noexcept;
    embF32 GetSimTimeScale() const noexcept;

  private:
    ClockDurationType m_DT = 0; // time since last update
    ClockDurationType m_SimTimeElapsed = 0; // time elapsed in simulation, affected by time scale.
    ClockDurationType m_RealTimeElapsed = 0;

    ClockDurationType m_TargetFrameTime = ClockDuration(Clock::period::den / 60).count(); // time denomination (nano,micro,milli) is dependent on clock. Default is nano.
    ClockDurationType m_TargetSimTime = ClockDuration(Clock::period::den / 20).count(); // time denomination (nano,micro,milli) is dependent on clock. Default is nano.

    ClockDurationType m_LastUpdateTimePointEpoch; // last saved time point since last frame update.
    ClockDurationType m_LastFixedUpdateTime = 0; // last updated time of fixed update, to be incremented in fixed steps. Should be "in the future"
    const ClockDurationType m_AppStartTimePoint; // time that the application started

    embF32 m_TimeScale = 1.0f;
    embU32 m_SimStepCount = 0; // number of sim steps to do for this frame
};

EMB_NAMESPACE_END
