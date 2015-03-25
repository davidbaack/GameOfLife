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
    
    const std::list<T*>& getPooledObjects() const;
    void put(T* object);
    T* retrieve();
    
private:
    
    std::list<T*> mPooledObjects;
    
};
    
template<typename T>
ObjectPool<T>::~ObjectPool()
{
    for (auto object : mPooledObjects)
    {
        object->release();
    }
}

template<typename T>
const std::list<T*>& ObjectPool<T>::getPooledObjects() const
{
    return mPooledObjects;
}

template<typename T>
void ObjectPool<T>::put(T* object)
{
    mPooledObjects.push_back(object);
    object->retain();
    object->onPutInPool();
}
    
template<typename T>
T* ObjectPool<T>::retrieve()
{
    T* object = nullptr;
    if (!mPooledObjects.empty())
    {
        object = mPooledObjects.back();
        mPooledObjects.pop_back();
        object->autorelease();
        object->onRetrieveFromPool();
    }
    return object;
}
    
}

#endif // _ENGINE_OBJECT_POOL_H
