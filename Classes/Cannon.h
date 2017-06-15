#ifndef __CANNON_H__
#define __CANNON_H__

#include "GameEntity.h"

class Cannon : public GameEntity {
public:
    Cannon();

    CREATE_FUNC(Cannon);

    virtual void updateEntity(float delta) {};
};

#endif // !__CANNON_H__
