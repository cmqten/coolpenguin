#ifndef __PENGUIN_SPAWNER_H__
#define __PENGUIN_SPAWNER_H__

#include <mutex>
#include <unordered_set>
#include "cocos2d.h"
#include "Penguin.h"

#define MAX_SPAWN 2

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
    void penguinDispatcher(Penguin* penguin, int slot);

    virtual void onEnter() override;

private:
    /* Stores the slots that are not being used */
    std::unordered_set<int>* _spawnSlots;

    /* Mutex for shared resource across threads */
    std::mutex _spawnLock;
};

#endif