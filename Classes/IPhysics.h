#ifndef __IPHYSICS_H__
#define __IPHYSICS_H__

#include "cocos2d.h"

/**
 * An interface for physics callbacks. In this interface, contact refers to
 * two objects touching, and collision refers to the physics response to two
 * objects touching. A collision cannot happen without a contact.
 */
class IPhysics {
public:
    /**
     * Called when two objects begin contact
     * @param contact : contact data
     * @return true to process contact, false otherwise
     */
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) = 0;

    /**
     * Called after two objects begin contact and before processing collision
     * @param contact : contact data
     * @param solve : collision data
     * @return true to process collision, false otherwise
     */
    virtual bool onContactPreSolve(cocos2d::PhysicsContact& contact, 
        cocos2d::PhysicsContactPreSolve& solve) = 0;

    /**
     * Called after processing collision, and for as long as the two objects are
     * colliding
     * @param contact : contact data
     * @param solve : collision data
     */
    virtual void onContactPostSolve(cocos2d::PhysicsContact& contact, 
        const cocos2d::PhysicsContactPostSolve& solve) = 0;

    /**
     * Called after two objects separate
     * @param contact : contact data
     */
    virtual void onContactSeparate(cocos2d::PhysicsContact& contact) = 0;
};

#endif 
