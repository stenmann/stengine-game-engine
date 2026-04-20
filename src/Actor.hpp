#ifndef ACTOR_H
#define ACTOR_H

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

using namespace std;

class Component{
    public:
        string type;
    
    
        shared_ptr<luabridge::LuaRef> componentRef;
    bool hasStarted = false;
    
    bool hasStart = false;
    bool hasUpdate = false;
    bool hasLateUpdate = false;
    bool hasDestroy = false;
    
};

class Actor {
public:
    string actor_name = "";
    int id = 0;
    bool destroyed = false;
    lua_State* lua_state = nullptr;
    static inline vector<Actor*>* all_actors = nullptr;
    static inline lua_State* lua_state_static = nullptr;
    static inline int n = 0;
    bool doNotDestroy = false;
    
    std::map<std::string, std::shared_ptr<Component>> components;
    static inline vector<pair<Actor*, shared_ptr<Component>>> pending_additions;
    static inline vector<pair<Actor*, std::string>> pending_removal;
    static inline vector<Actor*> pending_actors;
    static inline vector<Actor*> pending_destroys;
    static inline vector<Actor*> pending_actors_findable;
    
    string GetName()
    {
        return actor_name;
    }
    
    int GetID()
    {
        return id;
    }
    
    void InjectConvenienceReferences(shared_ptr<luabridge::LuaRef> componentRef)
    {
        (*componentRef)["actor"] = this;
    }
    
    luabridge::LuaRef GetComponentByKey(string key)
    {
        if(components.count(key) == 0)
            return luabridge::LuaRef(lua_state);  // nil
        return *components[key]->componentRef;
    }
    
    luabridge::LuaRef GetComponent(string type)
    {
        for (auto& [key, comp] : components)
        {
            luabridge::LuaRef& instance = *comp->componentRef;
            if (comp->type == type && (bool)instance["enabled"])
                return *comp->componentRef;
        }
        return luabridge::LuaRef(lua_state);  // nil if not found
    }
    
    luabridge::LuaRef GetComponents(string type)
    {
        luabridge::LuaRef result = luabridge::newTable(lua_state);
        int index = 1;  
        
        for (auto& [key, comp] : components)
        {
            luabridge::LuaRef& instance = *comp->componentRef;
            if (comp->type == type && (bool)instance["enabled"])
            {
                result[index] = *comp->componentRef;
                index++;
            }
        }
        return result;
    }
    
    static luabridge::LuaRef Find(string name)
    {
        for (Actor* guy : *all_actors)
        {
            if (guy->actor_name == name && !guy->destroyed)
                return luabridge::LuaRef(lua_state_static, guy);
        }
        
        for (Actor* guy : pending_actors)
        {
            if (guy->actor_name == name && !guy->destroyed)
                {return luabridge::LuaRef(lua_state_static, guy);}
        }
        
        return luabridge::LuaRef(lua_state_static);
    }
    
    static luabridge::LuaRef FindAll(string name)
    {
        luabridge::LuaRef result = luabridge::newTable(lua_state_static);
        int index = 1;
        
        for (Actor* guy : *all_actors)
        {
            if (guy->actor_name == name && !guy->destroyed)
            {
                result[index] = luabridge::LuaRef(lua_state_static, guy);
                index++;
            }
        }
        
        for (Actor* guy : pending_actors )
        {
            if (guy->actor_name == name && !guy->destroyed)
            {result[index] = luabridge::LuaRef(lua_state_static, guy);
                index++;
            }
        }
        
        return result;
    }
    
    luabridge::LuaRef AddComponent(std::string type);
    void RemoveComponent(luabridge::LuaRef ref);
    static luabridge::LuaRef Instantiate(std::string templateName);
    static void Destroy(Actor* actor);
    
    Actor() {}
};

#endif

