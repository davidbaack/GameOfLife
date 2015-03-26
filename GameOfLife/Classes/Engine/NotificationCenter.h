#ifndef _ENGINE_NOTIFICATION_CENTER_H
#define _ENGINE_NOTIFICATION_CENTER_H

#include <memory>
#include <unordered_map>

namespace engine
{
    
/** @brief A singleton for mapping function object callbacks to a string notification name.
 **        Use subscribe to register a function object to a notification.
 **        Use notify to invoke all callbacks currently subscribed to a notification.
 **        This class takes weak pointers so it automatically unsubscribes a function when its weak pointer expires.
 */
class NotificationCenter final
{
    
public:
    
    static NotificationCenter& getInstance();
    
    void notify(const std::string& notificationName);
    void subscribe(const std::string& notificationName, std::weak_ptr<std::function<void()>> callback);
    
private:
    
    NotificationCenter();
    ~NotificationCenter();
    NotificationCenter(const NotificationCenter&) = delete;
    NotificationCenter& operator=(const NotificationCenter&) = delete;
    
    std::unordered_map<std::string, std::list<std::weak_ptr<std::function<void()>>>> mNotificationNameToCallbacksMap;
    
};
    
}

#endif // _ENGINE_NOTIFICATION_CENTER_H
