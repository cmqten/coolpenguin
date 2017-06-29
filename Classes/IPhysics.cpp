#include "IPhysics.h"

using namespace cocos2d;

IPhysics::~IPhysics() {}

bool IPhysics::onContactBegin(PhysicsContact& contact) { return true; };

bool IPhysics::onContactPreSolve(PhysicsContact& contact, 
    PhysicsContactPreSolve& solve) { return true; }
