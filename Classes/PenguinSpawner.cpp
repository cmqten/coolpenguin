#include "PenguinSpawner.h"
#include <chrono>
#include <cstdlib>
#include <thread>

using namespace cocos2d;
using namespace std;

PenguinSpawner::PenguinSpawner() {
    // Initializes _spawnSlots as a hashmap of 4 buckets with a maximum load 
    // factor of 1
    _spawnSlots = new unordered_set<int>();
    _spawnSlots->max_load_factor(1.0f);
    _spawnSlots->insert(0);
    _spawnSlots->insert(1);
    _spawnSlots->insert(2);
    _spawnSlots->insert(3);
}

PenguinSpawner::~PenguinSpawner() {
    delete _spawnSlots;
}

void PenguinSpawner::spawnPenguin() {
    if (_spawnSlots->empty()) return;

    /**
     * Random number generation algorithm:
     * 1. Generate a random number between 0 and 3
     * 2. If the number is in the hash table, spawn the penguin at that slot
     *    and delete it from the hash table
     * 3. If the number is not in the hash table, add it to the hash table
     *    temporarily, spawn the penguin at the next element in the hash table,
     *    then delete the original number and the element after it
     */
    _spawnLock.lock(); // Thread safety, generates a random slot
    if ((4 - _spawnSlots->size()) >= MAX_SPAWN) {
        // Max number of penguins have been spawned
        _spawnLock.unlock();
        return;
    }

    int randomSlot = rand() & 3; // Generates a random number from 0 to 3
    auto penguin = Penguin::create();

    if (_spawnSlots->find(randomSlot) != _spawnSlots->end()) {
        // If slot is not being used, spawn the penguin there
        _spawnSlots->erase(randomSlot);
    }
    else { // If the slot is being used, generate a new number
        int oldRandomSlot = randomSlot;
        _spawnSlots->insert(oldRandomSlot);

        if (++(_spawnSlots->find(oldRandomSlot)) == _spawnSlots->end()) {
            // end of hash table
            randomSlot = *(_spawnSlots->begin());
        }
        else randomSlot = *(++(_spawnSlots->find(randomSlot)));

        _spawnSlots->erase(randomSlot);
        _spawnSlots->erase(oldRandomSlot);
    }
    _spawnLock.unlock();

    addChild(penguin);
    switch (randomSlot) { // slot position
        case 0: penguin->setPositionX(-204); break;
        case 1: penguin->setPositionX(-68); break;
        case 2: penguin->setPositionX(68); break;
        case 3: penguin->setPositionX(204); break;
        default: break;
    }

    thread threadPenguinDispatcher(&PenguinSpawner::penguinDispatcher, this,
        penguin, randomSlot);
    threadPenguinDispatcher.detach();
}

void PenguinSpawner::penguinDispatcher(Penguin* penguin, int slot) {
    int sec = 0;
    auto scheduler = Director::getInstance()->getScheduler();

    // Waddles the penguin in
    scheduler->performFunctionInCocosThread([penguin]() {
        penguin->waddleIn();
    });

    // Waits for the penguin to finish waddling in, then starts the timer
    while (penguin->_state != Penguin::State::RECV);
    while (sec < 10) {
        this_thread::sleep_for(chrono::seconds(1));
        sec++;
        if (penguin->_state != Penguin::State::RECV) break;
    }

    // The main thread may not have called waddleOut() when the timer runs
    // out because the penguin never received an item, so this thread
    // dispatches a waddleOut()
    if (penguin->_state == Penguin::State::RECV) {
        scheduler->performFunctionInCocosThread([penguin]() {
            penguin->waddleOut();
        });
    }

    // Waits for the penguin to finish waddling out, then deletes it
    while (penguin->_state != Penguin::State::DESPAWN);
    scheduler->performFunctionInCocosThread([this, penguin, slot]() {
        penguin->removeFromParentAndCleanup(true);
        penguin->release();
        this->_spawnLock.lock();
        this->_spawnSlots->insert(slot);
        this->_spawnLock.unlock();
    });

    scheduler->performFunctionInCocosThread([this]() {
        this->spawnPenguin();
    });
}

void PenguinSpawner::onEnter() {
    Node::onEnter();
    spawnPenguin();
    spawnPenguin();
}
