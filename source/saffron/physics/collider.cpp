#include "saffron_pch.h"

#include "saffron/physics/collider.h"
#include "saffron/physics/physics_mgr.h"

namespace saffron
{
void Collider::BuildBase(b2BodyDef* bodyDef, b2FixtureDef* fixtureDef)
{
	PhysicsMgr::AddCollider(this, bodyDef);
	_fixture = _body.value()->CreateFixture(fixtureDef);
}
}
