#include "CollisionHandler.h"

#include "SKSE/API.h"
#include "SKSE/Trampoline.h"


void CollisionHandler::Install()
{
	// E8 ? ? ? ? 48 8B 06 48 8B CE FF 90 ? ? ? ? 8B 86 ? ? ? ?
	constexpr std::uintptr_t FUNC_ADDR = 0x005D8170;	// 1_5_97
	REL::Offset<std::uintptr_t> funcBase(FUNC_ADDR);

	auto trampoline = SKSE::GetTrampoline();
	_ApplyMovementDelta = trampoline->Write5CallEx(funcBase.GetAddress() + 0xF0, Hook_ApplyMovementDelta);

	_MESSAGE("Installed hooks for %s", typeid(CollisionHandler).name());
}


void CollisionHandler::SetInDialogue(bool a_inDialogue)
{
	_inDialogue = a_inDialogue;
}


void CollisionHandler::Hook_ApplyMovementDelta(RE::Actor* a_this, float a_delta)
{
	if (!TryProcess(a_this, a_delta)) {
		_ApplyMovementDelta(a_this, a_delta);
	}
}


bool CollisionHandler::TryProcess(RE::Actor* a_actor, float a_delta)
{
	if (!_inDialogue || !a_actor->IsPlayerRef()) {
		return false;
	}

	auto controller = a_actor->GetCharController();
	if (!controller) {
		return false;
	}

	auto collisionObj = controller->bumpedCharCollisionObject.get();
	if (!collisionObj) {
		return false;
	}

	auto& filter = collisionObj->collidable.broadPhaseHandle.collisionFilterInfo;
	if (filter & COLLISION_FLAG) {
		return false;
	}

	ClearCollision(controller);

	_ApplyMovementDelta(a_actor, a_delta);

	RestoreCollision(controller);

	return true;
}


void CollisionHandler::ClearCollision(RE::bhkCharacterController* a_controller)
{
	_collisionObj = a_controller->bumpedCharCollisionObject;
	auto& filter = _collisionObj->collidable.broadPhaseHandle.collisionFilterInfo;
	filter |= COLLISION_FLAG;
}


void CollisionHandler::RestoreCollision(RE::bhkCharacterController* a_controller)
{
	auto& filter = _collisionObj->collidable.broadPhaseHandle.collisionFilterInfo;
	filter &= ~COLLISION_FLAG;
	_collisionObj.reset();
}


decltype(CollisionHandler::_inDialogue) CollisionHandler::_inDialogue = false;
decltype(CollisionHandler::_collisionObj) CollisionHandler::_collisionObj;
