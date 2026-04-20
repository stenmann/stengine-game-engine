
#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <optional>
#include "glm/glm.hpp"
#include <unordered_set>
#include <map>
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "box2d/box2d.h"

class EventBus
{
public:
    static inline unordered_map <string,vector<pair<luabridge::LuaRef, luabridge::LuaRef>>> subscriptions;
    
    static inline std::vector<std::tuple<std::string, luabridge::LuaRef, luabridge::LuaRef>> pending_subscribes;
    static inline std::vector<std::tuple<std::string, luabridge::LuaRef, luabridge::LuaRef>> pending_unsubscribes;
    
    
    static void Subscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function) {
        pending_subscribes.push_back({event_type, component, function});
    }

    static void Unsubscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function) {
        pending_unsubscribes.push_back({event_type, component, function});
    }

    static void Publish(std::string event_type, luabridge::LuaRef event_object) {
        if (subscriptions.find(event_type) == subscriptions.end()) return;
        
        auto subs_copy = subscriptions[event_type];
        for (auto& [component, function] : subs_copy) {
            try {
                function(component, event_object);
            } catch (luabridge::LuaException& e) {
                // handle error
            }
        }
    }
};





#endif
