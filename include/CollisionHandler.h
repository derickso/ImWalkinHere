#pragma once

#include <atomic>
#include <memory>

#include "RE/Skyrim.h"


class ICollider
{
public:
	ICollider() = default;
	virtual ~ICollider() = default;

	void PreCollide(RE::Actor* a_actor);
	void PostCollide();

protected:
	virtual bool ShouldIgnoreCollision(RE::Actor* a_actor) = 0;

private:
	void SetCollisionObject(RE::Actor* a_actor);
	void ClearCollisionObject();
	void SetCollisionOnObject(bool a_set);

	RE::hkRefPtr<RE::hkpWorldObject> _collisionObj;
};


class CollisionHandler
{
public:
	static void Install();

private:
	static bool Init();

	static void Hook_ApplyMovementDelta(RE::Actor* a_actor, float a_delta);

	static bool CanProcess(RE::Actor* a_actor, float a_delta);
	static void PreCollide(RE::Actor* a_actor);
	static void PostCollide(RE::Actor* a_actor);

	using ApplyMovementDelta_t = function_type_t<decltype(&CollisionHandler::Hook_ApplyMovementDelta)>;

	static inline ApplyMovementDelta_t* _ApplyMovementDelta = 0;
	static std::vector<std::unique_ptr<ICollider>> _colliders;
};


class DialogueCollider : public ICollider
{
public:
	DialogueCollider() = default;
	virtual ~DialogueCollider() = default;

protected:
	virtual bool ShouldIgnoreCollision(RE::Actor* a_actor) override;
};


class AllyCollider : public ICollider
{
public:
	AllyCollider() = default;
	virtual ~AllyCollider() = default;

protected:
	virtual bool ShouldIgnoreCollision(RE::Actor* a_actor) override;
};
