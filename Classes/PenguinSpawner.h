#ifndef __PENGUIN_SPAWNER_H__
#define __PENGUIN_SPAWNER_H__

#include <queue>
#include <unordered_set>
#include "cocos2d.h"
#include "IReset.h"
#include "Penguin.h"

/**
 * Handles spawning penguins at the top of the screen
 */
class PenguinSpawner : public cocos2d::Node, public IReset {
public:
    PenguinSpawner();

    virtual ~PenguinSpawner();

    CREATE_FUNC(PenguinSpawner);

    void spawnPenguin();

    virtual void reset();

    virtual bool init() override;

private:
    bool _gameOver;

    /* Maximum number of spawned penguins */
    int _maxSpawn;

    /* Array of all the penguins */
    Penguin** _penguins;

    /* Stores penguins that are waiting to be dispatched */
    std::queue<Penguin*>* _penguinQueue;

    /* Stores the slots that are not being used */
    std::unordered_set<int>* _spawnSlots;
};

#endif