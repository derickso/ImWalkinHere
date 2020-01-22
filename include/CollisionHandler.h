#pragma once

#include <atomic>

#include "RE/Skyrim.h"


class CollisionHandler
{
public:
	static void Install();
	static void SetInDialogue(bool a_inDialogue);

private:
	static void Hook_ApplyMovementDelta(RE::Actor* a_this, float a_delta);
	static bool TryProcess(RE::Actor* a_actor, float a_delta);
	static void ClearCollision(RE::bhkCharacterController* a_controller);
	static void RestoreCollision(RE::bhkCharacterController* a_controller);

	using ApplyMovementDelta_t = function_type_t<decltype(&CollisionHandler::Hook_ApplyMovementDelta)>;

	static constexpr auto COLLISION_FLAG = static_cast<UInt32>(RE::CFilter::Flag::kNoCollision);

	static inline ApplyMovementDelta_t* _ApplyMovementDelta = 0;
	static std::atomic_bool _inDialogue;
	static RE::hkRefPtr<RE::hkpWorldObject> _collisionObj;
};
