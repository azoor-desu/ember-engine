/// \file
/// <pre>
/// <para>    <b>File:</b>    DebuggingTools.cpp</para>
/// <para>    <b>Authors:</b>    Jan Tiong Shu Ling (75%)</para>
/// <para>						Loo Jun Yi (25%)</para>
/// <para>    <b>Emails:</b>    tiong.j@digipen.edu</para>
/// <para>					    l.junyi@digipen.edu</para>
/// <para>    <b>Date:</b>    07/09/2022</para>
/// </pre>
/// <summary>
/// This file contains the definitions of the functions used for the various
/// debugging tools, such as Asserts, Logging, Debug Draw and System Performance Checker
/// \n All content (c) 2022 DigiPen Institute of Technology Singapore. All rights reserved.
/// </summary>

#include "Tools/DebuggingTools.h"
#include "Tools/Logger.h"
#include "Engine/FrameController.h"
#include "Utils/Utils.h"

#include <chrono>
#include <map>
#include <sstream>
#include <iomanip>

// For drawing debug shapes
#include "Graphics/Graphics.h"
#include "Components/Shape.h"
#include "Graphics/TextSystem.h" // for rendering debug text
#include "Engine/InputManager.h" // For toggling on/off debug mode

// Always avail in editor. in game app, only in debug mode.
//#if !defined(NDEBUG) || defined(EDITOR)

bool showDebugText = false;
bool debugForces = false; //shows the object force vector
std::stringstream output1; // the calculated text for the performance checker

namespace Debug {
	using Clock = std::chrono::steady_clock; // steady clock over highres clock https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
	using ClockDuration = std::chrono::duration<double>;

	// misc variables to keep track of timing calcs.
	Clock::time_point frameBeginTime{}; // Save a time to start counting from.
	double totalTime = 0.f;
	const float displayInterval = 0.5f;
	float dispIntervalElapsed = displayInterval;
	//std::stringstream output1; // the calculated text for the performance checker

	// struct to encapsulate the time taken for
	// a system. To be stored in a map so there is a name
	// to pair with.
	struct SystemTracker {

		SystemTracker(): isRunning{false}, beginTime{}, duration{0.}{}

		bool isRunning = false; // for error catching.
		Clock::time_point beginTime{}; // Save a time to start counting from.
		double duration = 0.;
	};

	std::unordered_map<std::string, SystemTracker> systemTrackers{};

	/// Resets all trackers and signals the start of the frame to track
	void _TrackerStartFrame() {

		// rest global vars
		totalTime = 0.;
		frameBeginTime = Clock::now();

		// Reset each tracker
		for (auto& tracker : systemTrackers) {
			tracker.second.isRunning = false;
			tracker.second.duration = 0.;
		}
	}

	/// Starts the counter to see how long each system takes to complete
	void _TrackerStart(const std::string& systemName) {
		// If system does not exist, add it!
		if (systemTrackers.find(systemName) == systemTrackers.end()) {
			systemTrackers.emplace(systemName, SystemTracker());
		}
		// Set time to current, calculate time when stopping counter.
		systemTrackers[systemName].beginTime = Clock::now();
		systemTrackers[systemName].isRunning = true;
	}


	/// Stops tracking the time taken for this system, and adds to the internal time
	void _TrackerStop(const std::string& systemName) {
		// If system does not exist, skip
		if (systemTrackers.find(systemName) == systemTrackers.end()) return;

		LE_ASSERT_MSG(systemTrackers[systemName].isRunning == true, "A tracking timer was stopped when it wasn't running.");
		
		// Calc time diff and add to duration.
		ClockDuration delta = Clock::now() - systemTrackers[systemName].beginTime;
		systemTrackers[systemName].duration += delta.count();
		systemTrackers[systemName].isRunning = false;
	}

	/// Signals the end of the frame to track, and calculates the system timings.
	void _TrackerEndFrame() {
		ClockDuration delta = Clock::now() - frameBeginTime;
		totalTime = delta.count();
	}

	/// <summary>
	/// Shows the calculated results on-screen. (with ctrl+p)
	/// </summary>
	void _TrackersShow() {
		
		if (!showDebugText) return;

		// timer to display the stuff once every x seconds.
		if (dispIntervalElapsed >= displayInterval) {
			dispIntervalElapsed = 0.f;

			// Display the fun stuff
			// clear the ostringstream
			output1 = std::stringstream();

			//now we can start inputting the values again
			output1 << "FPS: " << (int)std::ceil(LE::FrameController::GetFPS()) << "\n"
				<< "Frame Delta Time: " << std::fixed << std::setprecision(3) << LE::FrameController::GetDeltaTime() << "s\n"
#ifdef NDEBUG
				;
#else
				<< "TimeTaken for all systems: " << totalTime << "s\n"
				<< "% of Frame Delta Time: " << (totalTime / LE::FrameController::GetDeltaTime() * 100.) << "%";
#endif  // NDEBUG


			// display each system's values.
			// display in format: Time in ms, % of total.
			//double sumTotalTime = 0.;
			//for (auto& tracker : systemTrackers) {
			//	//output1 << tracker.first << ": " << std::fixed << std::setprecision(3) << tracker.second.duration << "ms (" << (tracker.second.duration / totalTime * 100.) << "%)\n";
			//	output1 << tracker.first << ": (" << (tracker.second.duration / totalTime * 100.) << "%)\n";
			//	sumTotalTime += tracker.second.duration;
			//}

			//// Extra thing: track if there are missing timings
			//if (LE::AlmostEqualDp(totalTime, sumTotalTime, 4))
			//	output1 << "Unaccounted timing: " << std::fixed << std::setprecision(3) << (totalTime - sumTotalTime) << "(" << ((totalTime - sumTotalTime) / totalTime * 100.) << "%)\n";
		}
		else {
			dispIntervalElapsed += (float)LE::FrameController::GetDeltaTime();
		}

		// Keep displaying debug text.
		LE::TextRenderer::DebugText({-88.5f, 50.f, 0.f}, 2, output1.str());
	}
}
//#endif

namespace LE {
	/// Checks to see if debug mode should be turned on for the drawing of the debug lines
	bool debugMode = false;

	///// <summary>
	///// Updates the window title with the current FPS. 
	///// NOTE: Value is rounded up to the nearest int.
	///// </summary>
	//void PerfTools::ShowSystemFPS() {
	//	if (DEBUG_ON) {
	//		std::ostringstream temp;
	//		//must reset title to Luna Engine, or fps will be continuously appended at the end
	//		temp << "Luna Engine" << ' ' << round(LE::FrameController::fps) << "fps"; //casting to int will result in rounding down
	//		std::string title = temp.str();
	//		LE::WindowManager::SetWindowTitle(title.c_str());
	//	}
	//}

	void DebugGraphicsTools::SystemInit() {
		//debugText1 = ECSsys.CreateEntity();
	}

	/// <summary>
	/// Shows the forcecs affecting the various items and the AABB boundaries
	/// </summary>
	void DebugGraphicsTools::Update() {
		if (InputManager::isKeyDown(Button_List::KEY_LEFT_CONTROL) && InputManager::isKeyTriggered(Button_List::KEY_P)) {
			debugMode = !debugMode;
			showDebugText = !showDebugText; // for debug text
			switch (debugMode) {
				case true:
					std::cout << "[Debug Mode ON]" << std::endl;
					break;
				case false:
					std::cout << "[Debug Mode OFF]" << std::endl;
					break;
			}
		}
#if defined(EDITOR) || !defined(NDEBUG)
		if (debugMode == true || debugForces == true) {
			ShowDebugForceLines();
		}
#endif
		// Always show in editor, only show in game when ctrl p is toggled.

		// Always show debug collider if in Editor mode
#ifdef EDITOR
		ShowDebugCollider();
#else
		// If in game, only enable debug collider in debug mode
#ifndef NDEBUG
		if (debugMode == true) ShowDebugCollider();
#endif
#endif

	}


	DebugGraphicsTools::IntersectionCheck collisionChecker[1000][1000]; //used for the debugging of shapes
	void DebugGraphicsTools::ShowDebugForceLines() {
#if defined(EDITOR) || !defined(NDEBUG)

		std::vector<Rect*> rects = GetComponentArray<Rect>();
		std::vector<Circle*> circles = GetComponentArray<Circle>();

		for (auto rect : rects) {
			if (!rect->isActive) continue;
			if (!ECSsys.GetEntity(rect->entityID)->GetIsActive()) continue;

			//Renderer::DebugDraw(rect->globalPos, rect->body.vel); //line
			Renderer::DebugDraw(rect->transPtr->GetPosition()); //point
		}
		
		for (auto circle : circles) {
			if (!circle->isActive) continue;
			if (!ECSsys.GetEntity(circle->entityID)->GetIsActive()) continue;

			//Renderer::DebugDraw(circle->center, circle->body.vel);
			if (circle->transPtr)
			Renderer::DebugDraw(circle->transPtr->GetPosition() - circle->offset.ToVec3());
		}
#endif
	}

	void DebugGraphicsTools::ShowDebugCollider() {
#if defined(EDITOR) || !defined(NDEBUG)
		std::vector<Rect*> rects = GetComponentArray<Rect>();
		std::vector<Circle*> circles = GetComponentArray<Circle>();
		std::vector<LineSegment*> lines = GetComponentArray<LineSegment>();

		for (auto line : lines)
		{
			if (!line->isActive) continue;
			if (!ECSsys.GetEntity(line->entityID)->GetIsActive()) continue;

			if (line->transPtr)
			Renderer::DebugDraw(line->transPtr->GetPosition() - line->offset.ToVec3() - (line->scale/2.f).ToVec3(), line->scale, LE::Color::green);
		}

		for (auto rect : rects) {
			if (!rect->isActive) continue;
			if (!ECSsys.GetEntity(rect->entityID)->GetIsActive()) continue;

			if (rect->isCollidedSwitch == true) { //if collide == true
				if (rect->transPtr)
				Renderer::DebugDraw(rect->transPtr->GetPosition() - rect->offset.ToVec3(), rect->size, 0, Color::red);
			}
			else {
				if (rect->transPtr)
				Renderer::DebugDraw(rect->transPtr->GetPosition() - rect->offset.ToVec3(), rect->size, 0, Color::green);
			}
		}
		
		for (auto circle : circles) {
			if (!circle->isActive) continue;
			if (!ECSsys.GetEntity(circle->entityID)->GetIsActive()) continue;

			if (circle->isCollidedSwitch == true) { //if collide == true
				if (circle->transPtr)
				Renderer::DebugDraw(circle->transPtr->GetPosition() - circle->offset.ToVec3(), circle->radius, Color::red);
			}
			else {
				if (circle->transPtr)
				Renderer::DebugDraw(circle->transPtr->GetPosition() - circle->offset.ToVec3(), circle->radius, Color::green);
			}
		}
#endif
	}

	void DebugGraphicsTools::SetCollisionTracker(int shape1, int shape2, bool status) {
#if defined(EDITOR) || !defined(NDEBUG)
		if (shape1 < 0 || shape2 < 0) {
			return;
		}

		if (status == true) {
			if (collisionChecker[shape1][shape2] == IntersectionCheck::DEFAULT) { //this is a new collision
				collisionChecker[shape1][shape2] = IntersectionCheck::COLLIDED;
				collisionChecker[shape2][shape1] = IntersectionCheck::COLLIDED;
			}
			else { //collision was already accounted for, COLLIDED or ALREADY_COLLIDED
				collisionChecker[shape1][shape2] = IntersectionCheck::ALREADY_COLLIDED;
				collisionChecker[shape2][shape1] = IntersectionCheck::ALREADY_COLLIDED;
			}
		}
		else { //status == false
			collisionChecker[shape1][shape2] = IntersectionCheck::DEFAULT;
			collisionChecker[shape2][shape1] = IntersectionCheck::DEFAULT;
		}
#else
		(void)shape1;
		(void)shape2;
		(void)status;
#endif
	}

	void DebugGraphicsTools::SystemExit() {
		//ECSsys.DeleteEntity(debugText1);
	}

}