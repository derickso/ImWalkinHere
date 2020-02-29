#include "CollisionHandler.h"

#include "SKSE/API.h"
#include "SKSE/Trampoline.h"

#include "Events.h"
#include "Settings.h"


namespace
{
	constexpr auto COLLISION_FLAG = static_cast<UInt32>(RE::CFilter::Flag::kNoCollision);
}


void ICollider::PreCollide(RE::Actor* a_actor)
{
	if (ShouldIgnoreCollision(a_actor)) {
		SetCollisionObject(a_actor);
		SetCollisionOnObject(false);
	}
}


void ICollider::PostCollide()
{
	if (_collisionObj) {
		SetCollisionOnObject(true);
		ClearCollisionObject();
	}
}


void ICollider::SetCollisionObject(RE::Actor* a_actor)
{
	auto controller = a_actor->GetCharController();
	_collisionObj = controller->bumpedCharCollisionObject;
}


void ICollider::ClearCollisionObject()
{
	_collisionObj.reset();
}


void ICollider::SetCollisionOnObject(bool a_set)
{
	auto& filter = _collisionObj->collidable.broadPhaseHandle.collisionFilterInfo;
	if (!a_set) {
		filter |= COLLISION_FLAG;
	} else {
		filter &= ~COLLISION_FLAG;
	}
}


void CollisionHandler::Install()
{
	if (!Init()) {
		_ERROR("%s did not have any collision handlers", typeid(CollisionHandler).name());
		return;
	}

	REL::Offset<std::uintptr_t> hookPoint(REL::ID(36359), 0xF0);

	auto trampoline = SKSE::GetTrampoline();
	_ApplyMovementDelta = trampoline->Write5CallEx(hookPoint.GetAddress(), Hook_ApplyMovementDelta);

	_MESSAGE("Installed hooks for %s", typeid(CollisionHandler).name());
}


bool CollisionHandler::Init()
{
	if (*Settings::disableDialogueCollision) {
		_colliders.push_back(std::make_unique<DialogueCollider>());
	}

	if (*Settings::disableAllyCollision) {
		_colliders.push_back(std::make_unique<AllyCollider>());
	}

	return !_colliders.empty();
}


void CollisionHandler::Hook_ApplyMovementDelta(RE::Actor* a_actor, float a_delta)
{
	if (!CanProcess(a_actor, a_delta)) {
		_ApplyMovementDelta(a_actor, a_delta);
	}
}


bool CollisionHandler::CanProcess(RE::Actor* a_actor, float a_delta)
{
	if (!a_actor->IsPlayerRef()) {
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

	PreCollide(a_actor);

	_ApplyMovementDelta(a_actor, a_delta);

	PostCollide(a_actor);

	return true;
}


void CollisionHandler::PreCollide(RE::Actor* a_actor)
{
	for (auto& collider : _colliders) {
		collider->PreCollide(a_actor);
	}
}


void CollisionHandler::PostCollide([[maybe_unused]] RE::Actor* a_actor)
{
	for (auto& collider : _colliders) {
		collider->PostCollide();
	}
}


decltype(CollisionHandler::_colliders) CollisionHandler::_colliders;


bool DialogueCollider::ShouldIgnoreCollision([[maybe_unused]] RE::Actor* a_actor)
{
	auto menuHandler = Events::MenuOpenCloseHandler::GetSingleton();
	return menuHandler->IsInDialogue();
}


bool AllyCollider::ShouldIgnoreCollision(RE::Actor* a_actor)
{
	auto controller = a_actor->GetCharController();
	auto collisionObj = controller->bumpedCharCollisionObject.get();
	auto colRef = RE::TESHavokUtilities::FindCollidableRef(collisionObj->collidable);
	if (!colRef) {
		return false;
	}

	if (colRef->IsNot(RE::FormType::ActorCharacter)) {
		return false;
	}

	auto colActor = static_cast<RE::Actor*>(colRef);
	return colActor->IsPlayerTeammate() && !colActor->IsHorse();
}
