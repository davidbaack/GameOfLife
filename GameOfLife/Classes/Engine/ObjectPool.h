#ifndef _ENGINE_OBJECT_POOL_H
#define _ENGINE_OBJECT_POOL_H

#include <list>

namespace engine
{

template<typename T>
class ObjectPool final
{
    
public:
    
    ~ObjectPool();
    
    const std::list<T*>& getPool() const;
    void putInPool(T* node);
    T* retrieveFromPool();
    
private:
    
    std::list<T*> mPooledNodes;
    
};
    
template<typename T>
ObjectPool<T>::~ObjectPool()
{
    for (auto node : mPooledNodes)
    {
        node->release();
    }
}

template<typename T>
const std::list<T*>& ObjectPool<T>::getPool() const
{
    return mPooledNodes;
}

template<typename T>
void ObjectPool<T>::putInPool(T* node)
{
    mPooledNodes.push_back(node);
    node->retain();
    node->onPutInPool();
}
    
template<typename T>
T* ObjectPool<T>::retrieveFromPool()
{
    T* node = nullptr;
    if (!mPooledNodes.empty())
    {
        node = mPooledNodes.back();
        mPooledNodes.pop_back();
        node->autorelease();
        node->onRetrieveFromPool();
    }
    return node;
}
    
}

#endif // _ENGINE_OBJECT_POOL_H
