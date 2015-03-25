#include "PoolableNode.h"

using namespace engine;
using namespace std;

PoolableNode::PoolableNode()
    : mIsPooled(false)
{}

bool PoolableNode::isInPool() const
{
    return mIsPooled;
}

void PoolableNode::onPutInPool()
{
    mIsPooled = true;
}

void PoolableNode::onRetrieveFromPool()
{
    mIsPooled = false;
}
