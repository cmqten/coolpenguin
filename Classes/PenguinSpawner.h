#ifndef __PENGUIN_SPAWNER_H__
#define __PENGUIN_SPAWNER_H__

#include <queue>
#include <unordered_set>
#include "cocos2d.h"
#include "Penguin.h"

/**
 * Handles spawning penguins at the top of the screen
 */
class PenguinSpawner : public cocos2d::Node {
public:
    PenguinSpawner();

    virtual ~PenguinSpawner();

    CREATE_FUNC(PenguinSpawner);

    void spawnPenguin();

    /**
     * Action dispatcher for spawned penguin. Tells penguin when to waddle in
     * or waddle out.
     * @param penguin : penguin to be dispatched
     * @param slot : spawn slot where the penguin was spawned
     */
    //void penguinDispatcher(Penguin* penguin, int slot);

    virtual void onEnter() override;

private:
    /* Maximum number of spawned penguins */
    int _maxSpawn;

    /* Stores penguins that are not dispatched */
    std::queue<Penguin*>* _penguins;

    /* Stores the slots that are not being used */
    std::unordered_set<int>* _spawnSlots;
};

#endif