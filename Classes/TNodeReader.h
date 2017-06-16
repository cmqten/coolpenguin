#ifndef __T_NODE_READER_H__
#define __T_NODE_READER_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CocosStudioExport.h"
#include "editor-support/cocostudio/WidgetReader/NodeReader/NodeReader.h"

/**
A generic reader for Node and its subclasses. The class type passed into the 
template parameted must define the a static method create() which takes no 
arguments and returns a pointer to a Node or a Node subclass. For example:

static cocos2d::Node* create();
*/
template <class T>
class TNodeReader : public cocostudio::NodeReader {
public:
    TNodeReader() { };
    ~TNodeReader() { };
    static TNodeReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOp);

private:
    static TNodeReader* _instance;
};

template <class T>
TNodeReader<T>* TNodeReader<T>::_instance = nullptr;

template<class T>
inline TNodeReader<T>* TNodeReader<T>::getInstance() {
    if (!_instance) _instance = new TNodeReader();
    return _instance;
}

template<class T>
inline void TNodeReader<T>::purge() {
    CC_SAFE_DELETE(_instance);
}

template<class T>
inline cocos2d::Node* TNodeReader<T>::createNodeWithFlatBuffers
(const flatbuffers::Table* nodeOp) {
    cocos2d::Node* node = T::create();
    setPropsWithFlatBuffers(node, nodeOp);
    return node;
}

#endif 