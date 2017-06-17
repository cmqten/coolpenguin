#ifndef __IPHYSICS_H__
#define __IPHYSICS_H__

#include "cocos2d.h"

/**
An interface for physics callbacks
*/
class IPhysics {
public:
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) = 0;

    virtual bool onContactPreSolve(cocos2d::PhysicsContact& contact, 
        cocos2d::PhysicsContactPreSolve& solve) = 0;

    virtual void onContactPostSolve(cocos2d::PhysicsContact& contact, 
        const cocos2d::PhysicsContactPostSolve& solve) = 0;

    virtual void onContactSeparate(cocos2d::PhysicsContact& contact) = 0;
};

#endif 
