#ifndef __IRESET_H__
#define __IRESET_H__

/**
 * An interface for resetting game objects to their original state, i.e., 
 * starting a new game.
 */
class IReset {
public:
    virtual void reset() = 0;
};

#endif // !__IRESET_H__
