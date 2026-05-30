#include "saffron_pch.h"

#include "saffron/physics/physics_mgr.h"

namespace saffron
{
std::unique_ptr<b2World> PhysicsMgr::_world;

PhysicsMgr::PhysicsMgr()
{
	_world = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
}

void PhysicsMgr::Step()
{
	_world->Step(1.0f / 60.0f, 6, 2);
}

void PhysicsMgr::AddCollider(Collider* collider, const b2BodyDef* bodyDef)
{
	collider->AssignBody(_world->CreateBody(bodyDef));
}
}
