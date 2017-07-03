#include "PenguinSpawner.h"
#include <cstdlib>
#include <ctime>

using namespace cocos2d;
using namespace std;

PenguinSpawner::PenguinSpawner() {
    // Initializes _spawnSlots as a hashmap with 4 integers from 0 to 3 
    _spawnSlots = new unordered_set<int>();
    _spawnSlots->insert(0);
    _spawnSlots->insert(1);
    _spawnSlots->insert(2);
    _spawnSlots->insert(3);

    // Creates as many penguins as there are slots
    _penguins = new queue<Penguin*>();
    for (int i = 0; i < 4; i++) {
        auto penguin = dynamic_cast<Penguin*>(CSLoader::createNode(
            "csb/penguin.csb"));
        penguin->retain();
        _penguins->push(penguin);
        addChild(penguin);
    }
}

PenguinSpawner::~PenguinSpawner() {
    delete _spawnSlots;
    for (int i = 0; i < 4; i++) {
        auto penguin = _penguins->front();
        _penguins->pop();
        delete penguin;
    }
    delete _penguins;
}

void PenguinSpawner::spawnPenguin() {
    if (_spawnSlots->empty()) return;

    // Max number spawned
    if ((4 - _spawnSlots->size()) >= MAX_SPAWN) return;

    /* Chooses a penguin to dispatch. No need to check if penguin queue is 
    empty or not because there always has to be penguins in the queue, and if
    there are no penguins, the maximum number have been spawned, and the call
    would have return before reaching this part. */
    auto penguin = _penguins->front();
    _penguins->pop();

    /**
     * Random number generation algorithm:
     * 1. Generate a random number between 0 and 3
     * 2. If the number is in the hash table, spawn the penguin at that slot
     *    and delete it from the hash table
     * 3. If the number is not in the hash table, add it to the hash table
     *    temporarily, spawn the penguin at the next element in the hash table,
     *    then delete the original number and the element after it
     */
    srand(time(nullptr));
    int randomSlot = rand() & 3; // Generates a random number from 0 to 3

    if (_spawnSlots->find(randomSlot) != _spawnSlots->end()) {
        // If slot is not being used, spawn the penguin there
        _spawnSlots->erase(randomSlot);
    }
    else { 
        /* If the slot is being used, generate a new number by getting the
        next non-empty bucket. If the bucket is at the end, get the first
        bucket. */
        int oldRandomSlot = randomSlot;
        _spawnSlots->insert(oldRandomSlot);

        if (++(_spawnSlots->find(oldRandomSlot)) == _spawnSlots->end()) {
            randomSlot = *(_spawnSlots->begin());
        }
        else randomSlot = *(++(_spawnSlots->find(randomSlot)));

        _spawnSlots->erase(randomSlot);
        _spawnSlots->erase(oldRandomSlot);
    }

    switch (randomSlot) { // slot position
        case 0: penguin->setPosition(-204, 0); break;
        case 1: penguin->setPosition(-68, 0); break;
        case 2: penguin->setPosition(68, 0); break;
        case 3: penguin->setPosition(204, 0); break;
        default: break;
    }

    // Prepares the penguin for dispatch, binds slot to penguin for access 
    // after the penguin has returned
    penguin->resetPenguin();
    penguin->setUserData(new int(randomSlot));
    penguin->waddleIn();
}

void PenguinSpawner::onEnter() {
    Node::onEnter();

    /* Listener for any penguins that have returned, so they can be added to 
    the queue again and their slots added back to the hash table. */
    getEventDispatcher()->addCustomEventListener(PENGUIN_DONE, 
        [this](EventCustom* event) {
            // Adds slot back to hash table, frees to prevent memory leak
            int* slot = (int*)((Penguin*)event->getUserData())->getUserData();
            this->_spawnSlots->insert(*slot);
            delete slot;
            ((Penguin*)event->getUserData())->setUserData(nullptr);

            // Adds penguin back to the queue and tries to spawn a new penguin
            this->_penguins->push((Penguin*)event->getUserData());
            this->spawnPenguin();
        });

    spawnPenguin();
    spawnPenguin();
}
