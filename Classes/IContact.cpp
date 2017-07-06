#include "IContact.h"

using namespace cocos2d;

IContact::~IContact() {}

bool IContact::onContactBegin(PhysicsContact& contact) { return true; };

bool IContact::onContactPreSolve(PhysicsContact& contact, 
    PhysicsContactPreSolve& solve) { return true; }
