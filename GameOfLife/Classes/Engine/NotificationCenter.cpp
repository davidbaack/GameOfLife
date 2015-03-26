#include "NotificationCenter.h"

using namespace engine;
using namespace std;

NotificationCenter& NotificationCenter::getInstance()
{
    static NotificationCenter instance;
    return instance;
}

NotificationCenter::NotificationCenter()
{}

NotificationCenter::~NotificationCenter()
{}

void NotificationCenter::notify(const string& notificationName)
{
    if (mNotificationNameToCallbacksMap.count(notificationName) == 0)
    {
        return;
    }
    auto& callbackList = mNotificationNameToCallbacksMap.at(notificationName);
    
    // Invoke all subscribed callback functions, and remove any expired callback functions to avoid bloat
    for (auto iter = callbackList.begin(); iter != callbackList.end();)
    {
        auto callbackPointer = *iter;
        if (callbackPointer.expired())
        {
            iter = callbackList.erase(iter);
        }
        else
        {
            (*callbackPointer.lock())();
            ++iter;
        }
    }
}

void NotificationCenter::subscribe(const string& notificationName, weak_ptr<function<void()>> callback)
{
    auto& callbackList = mNotificationNameToCallbacksMap[notificationName];
    callbackList.push_back(callback);
}
