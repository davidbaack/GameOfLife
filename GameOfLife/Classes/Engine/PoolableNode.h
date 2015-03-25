#ifndef _ENGINE_POOLABLE_NODE_H
#define _ENGINE_POOLABLE_NODE_H

#include "cocos2d.h"

namespace engine
{
    
class PoolableNode : public cocos2d::Node
{
    
public:
    
    PoolableNode();
    
    bool isInPool() const;
    virtual void onPutInPool();
    virtual void onRetrieveFromPool();
    
private:
    
    bool mIsPooled;
    
};
    
}

#endif // _ENGINE_POOLABLE_NODE_H
