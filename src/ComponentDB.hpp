#ifndef COMPONENTDB_H
#define COMPONENTDB_H

#include <string>
#include <optional>
#include "glm/glm.hpp"
#include <unordered_set>
#include <map>
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "Actor.hpp"
#include "Application.hpp"
#include "Input.hpp"
#include "TextDB.hpp"
#include "AudioDB.hpp"
#include "ImageDB.hpp"
#include "SceneDB.hpp"
#include "box2d/box2d.h"
#include "physics.hpp"
#include "Particle.hpp"
#include "EventBus.hpp"

using namespace std;

class componentDB {
    public:
    lua_State* lua_state;
    static inline std::unordered_set<std::string> loaded_types;
    static componentDB& getInstance() {
        static componentDB instance;  
        return instance;
    }
    componentDB()
    {
        lua_state = luaL_newstate();
        luaL_openlibs(lua_state);
        physics::lua_state = lua_state;
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Debug")
            .addFunction("Log", componentDB::CppLog)
            .addFunction("LogError", componentDB::CppLogError)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginClass<collision>("collision")
                .addProperty("other", &collision::other)
                .addProperty("point", &collision::point)
                .addProperty("normal", &collision::normal)
                .addProperty("relative_velocity", &collision::relative_velocity)
            .endClass();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginClass<ParticleSystem>("ParticleSystem")
            .addConstructor<void(*)(void)>()
            .addProperty("key", &ParticleSystem::key)
            .addProperty("x", &ParticleSystem::x)
            .addProperty("y", &ParticleSystem::y)
            .addProperty("frames_between_bursts", &ParticleSystem::frames_between_bursts)
            .addProperty("burst_quantity", &ParticleSystem::burst_quantity)
            .addProperty("start_scale_min", &ParticleSystem::start_scale_min)
            .addProperty("start_scale_max", &ParticleSystem::start_scale_max)
            .addProperty("rotation_min", &ParticleSystem::rotation_min)
            .addProperty("rotation_max", &ParticleSystem::rotation_max)
            .addProperty("start_color_r", &ParticleSystem::start_color_r)
            .addProperty("start_color_g", &ParticleSystem::start_color_g)
            .addProperty("start_color_b", &ParticleSystem::start_color_b)
            .addProperty("start_color_a", &ParticleSystem::start_color_a)
            .addProperty("emit_angle_min", &ParticleSystem::emit_angle_min)
            .addProperty("emit_angle_max", &ParticleSystem::emit_angle_max)
            .addProperty("emit_radius_min", &ParticleSystem::emit_radius_min)
            .addProperty("emit_radius_max", &ParticleSystem::emit_radius_max)
            .addProperty("image", &ParticleSystem::image)
            .addProperty("sorting_order", &ParticleSystem::sorting_order)
            .addProperty("duration_frames", &ParticleSystem::duration_frames)
            .addProperty("start_speed_min", &ParticleSystem::start_speed_min)
            .addProperty("start_speed_max", &ParticleSystem::start_speed_max)
            .addProperty("rotation_speed_min", &ParticleSystem::rotation_speed_min)
            .addProperty("rotation_speed_max", &ParticleSystem::rotation_speed_max)
            .addProperty("gravity_scale_x", &ParticleSystem::gravity_scale_x)
            .addProperty("gravity_scale_y", &ParticleSystem::gravity_scale_y)
            .addProperty("drag_factor", &ParticleSystem::drag_factor)
            .addProperty("angular_drag_factor", &ParticleSystem::angular_drag_factor)
            .addProperty("end_scale", &ParticleSystem::end_scale)
            .addProperty("end_color_r", &ParticleSystem::end_color_r)
            .addProperty("end_color_g", &ParticleSystem::end_color_g)
            .addProperty("end_color_b", &ParticleSystem::end_color_b)
            .addProperty("end_color_a", &ParticleSystem::end_color_a)
            .addProperty("enabled", &ParticleSystem::enabled)
            .addFunction("Stop",  &ParticleSystem::Stop)
            .addFunction("Play",  &ParticleSystem::Play)
            .addFunction("Burst", &ParticleSystem::Burst)
            .endClass();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginClass<Rigidbody>("Rigidbody")
                .addConstructor<void(*)(void)>()
                .addProperty("key", &Rigidbody::key)
                .addProperty("enabled", &Rigidbody::enabled)
                .addProperty("actor", &Rigidbody::actor)
                .addProperty("x", &Rigidbody::x)
                .addProperty("y", &Rigidbody::y)
                .addProperty("body_type", &Rigidbody::body_type)
                .addProperty("precise", &Rigidbody::precise)
                .addProperty("gravity_scale", &Rigidbody::gravity_scale)
                .addProperty("density", &Rigidbody::density)
                .addProperty("angular_friction", &Rigidbody::angular_friction)
                .addProperty("rotation", &Rigidbody::rotation)
                .addProperty("has_collider", &Rigidbody::has_collider)
                .addProperty("has_trigger", &Rigidbody::has_trigger)
                .addProperty("collider_type", &Rigidbody::collider_type)
                .addProperty("width", &Rigidbody::width)
                .addProperty("height", &Rigidbody::height)
                .addProperty("radius", &Rigidbody::radius)
                .addProperty("friction", &Rigidbody::friction)
                .addProperty("bounciness", &Rigidbody::bounciness)
                .addProperty("trigger_type", &Rigidbody::trigger_type)
                .addProperty("trigger_width", &Rigidbody::trigger_width)
                .addProperty("trigger_height", &Rigidbody::trigger_height)
                .addProperty("trigger_radius", &Rigidbody::trigger_radius)
                .addFunction("OnStart", &Rigidbody::OnStart)
                .addFunction("OnDestroy", &Rigidbody::OnDestroy)
                .addFunction("GetPosition", &Rigidbody::GetPosition)
                .addFunction("GetRotation", &Rigidbody::GetRotation)
                .addFunction("AddForce", &Rigidbody::AddForce)
                .addFunction("SetVelocity", &Rigidbody::SetVelocity)
                .addFunction("SetPosition", &Rigidbody::SetPosition)
                .addFunction("SetRotation", &Rigidbody::SetRotation)
                .addFunction("SetAngularVelocity", &Rigidbody::SetAngularVelocity)
                .addFunction("SetGravityScale", &Rigidbody::SetGravityScale)
                .addFunction("SetUpDirection", &Rigidbody::SetUpDirection)
                .addFunction("SetRightDirection", &Rigidbody::SetRightDirection)
                .addFunction("GetVelocity", &Rigidbody::GetVelocity)
                .addFunction("GetAngularVelocity", &Rigidbody::GetAngularVelocity)
                .addFunction("GetGravityScale", &Rigidbody::GetGravityScale)
                .addFunction("GetUpDirection", &Rigidbody::GetUpDirection)
                .addFunction("GetRightDirection", &Rigidbody::GetRightDirection)
        
            .endClass();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginClass<Actor>("Actor")
            .addFunction("GetName",&Actor::GetName)
            .addFunction("GetID",&Actor::GetID)
            .addFunction("GetComponentByKey", &Actor::GetComponentByKey)
            .addFunction("GetComponent", &Actor::GetComponent)
            .addFunction("GetComponents", &Actor::GetComponents)
            .addFunction("AddComponent", &Actor::AddComponent)
            .addFunction("RemoveComponent", &Actor::RemoveComponent)
            .endClass();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Actor")
            .addFunction("Find", &Actor::Find)
            .addFunction("FindAll", &Actor::FindAll)
            .addFunction("Instantiate", &Actor::Instantiate)
            .addFunction("Destroy", &Actor::Destroy)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Application")
            .addFunction("Quit", &Application::Quit)
            .addFunction("Sleep", &Application::Sleep)
            .addFunction("GetFrame", &Application::GetFrame)
            .addFunction("OpenURL", &Application::OpenURL)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginClass<glm::vec2>("vec2")
                .addProperty("x", &glm::vec2::x)
                .addProperty("y", &glm::vec2::y)
            .endClass();

        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Input")
                .addFunction("GetKey", static_cast<bool(*)(std::string)>(&Input::GetKey))
                .addFunction("GetKeyDown", static_cast<bool(*)(std::string)>(&Input::GetKeyDown))
                .addFunction("GetKeyUp", static_cast<bool(*)(std::string)>(&Input::GetKeyUp))
                .addFunction("GetMousePosition", &Input::GetMousePosition)
                .addFunction("GetMouseButton", &Input::GetMouseButton)
                .addFunction("GetMouseButtonDown", &Input::GetMouseButtonDown)
                .addFunction("GetMouseButtonUp", &Input::GetMouseButtonUp)
                .addFunction("GetMouseScrollDelta", &Input::GetMouseScrollDelta)
                .addFunction("HideCursor", &Input::HideCursor)
                .addFunction("ShowCursor", &Input::ShowCursor)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Text")
                .addFunction("Draw", &TextDB::Draw)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Audio")
                .addFunction("Play", &musicDB::Play)
                .addFunction("Halt", &musicDB::Halt)
                .addFunction("SetVolume", &musicDB::SetVolume)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Image")
                .addFunction("Draw", &imagesDB::Draw)
                .addFunction("DrawEx", &imagesDB::DrawEx)
                .addFunction("DrawUI", &imagesDB::DrawUI)
                .addFunction("DrawUIEx", &imagesDB::DrawUIEx)
                .addFunction("DrawPixel", &imagesDB::DrawPixel)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Camera")
            .addFunction("SetPosition", &imagesDB::SetPosition)
            .addFunction("GetPositionX", &imagesDB::GetPositionX)
            .addFunction("GetPositionY", &imagesDB::GetPositionY)
            .addFunction("SetZoom", &imagesDB::SetZoom)
            .addFunction("GetZoom", &imagesDB::GetZoom)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Scene")
                .addFunction("Load", &SceneDB::Load)
                .addFunction("GetCurrent", &SceneDB::GetCurrent)
                .addFunction("DontDestroy", &SceneDB::DontDestroy)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginClass<b2Vec2>("Vector2")
            .addConstructor<void(*) (float,float)>()
            .addProperty("x", &b2Vec2::x)
            .addProperty("y", &b2Vec2::y)
            .addFunction("Normalize", &b2Vec2::Normalize)
            .addFunction("Length", &b2Vec2::Length)
            .addFunction("__add", &b2Vec2::operator_add)
            .addFunction("__sub", &b2Vec2::operator_sub)
            .addFunction("__mul", &b2Vec2::operator_mul)
            .addStaticFunction("Dot", static_cast<float (*)(const b2Vec2&,const b2Vec2&)>(&b2Dot))
            .addStaticFunction("Distance",b2Distance)
            .endClass();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginClass<HitResult>("HitResult")
                .addProperty("actor", &HitResult::actor)
                .addProperty("point", &HitResult::point)
                .addProperty("normal", &HitResult::normal)
                .addProperty("is_trigger", &HitResult::is_trigger)
            .endClass();

        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Physics")
                .addFunction("Raycast", &physics::Raycast)
                .addFunction("RaycastAll", &physics::RaycastAll)
            .endNamespace();
        
        luabridge::getGlobalNamespace(lua_state)
            .beginNamespace("Event")
                .addFunction("Publish", &EventBus::Publish)
                .addFunction("Subscribe", &EventBus::Subscribe)
                .addFunction("Unsubscribe", &EventBus::Unsubscribe)
            .endNamespace();
    }
    
    shared_ptr<Component> createComponent(string key, string type,string typePath)
    {
        if (type == "ParticleSystem")
        {
            ParticleSystem* ps = new ParticleSystem();
            
            ps->key = key;
            
            shared_ptr<Component> comp = make_shared<Component>();
            comp->type = type;
            comp->hasStart = true;
            comp->hasUpdate = true;
            luabridge::LuaRef ref(lua_state, ps);
            comp->componentRef = make_shared<luabridge::LuaRef>(ref);
            return comp;
        }
        
        if (type == "Rigidbody") {
            Rigidbody* rb = new Rigidbody();
            rb->key = key;
            rb->enabled = true;
            
            shared_ptr<Component> comp = make_shared<Component>();
            comp->type = type;
            comp->hasStart = true;
            luabridge::LuaRef ref(lua_state, rb);
            ref["key"] = key;
            ref["enabled"] = true;
            comp->componentRef = make_shared<luabridge::LuaRef>(ref);
            return comp;
        }
        
        if (loaded_types.count(type) == 0) {
            if(luaL_dofile(lua_state, typePath.c_str()) != LUA_OK) {
                cout << "problem with lua file " << type;
                exit(0);
            }
            loaded_types.insert(type);
        }
        
        luabridge::LuaRef baseTable = luabridge::getGlobal(lua_state, type.c_str());
        
        
        luabridge::LuaRef instance = luabridge::newTable(lua_state);
        
        establishInheritance(instance, baseTable);
        
        shared_ptr<Component> comp = make_shared<Component>();
        
        instance["key"] = key;
        instance["enabled"] = true;
        
        comp->type = type;
        comp->componentRef = make_shared<luabridge::LuaRef>(instance);
        
        comp->hasStart = instance["OnStart"].isFunction();
        comp->hasUpdate = instance["OnUpdate"].isFunction();
        comp->hasLateUpdate = instance["OnLateUpdate"].isFunction();
        comp->hasDestroy = instance["OnDestroy"].isFunction();

        
        return comp;
   }
    
    
    void establishInheritance (luabridge::LuaRef & instance_table, luabridge::LuaRef & parent_table)
    {
        luabridge::LuaRef new_metatable = luabridge::newTable(lua_state);
        new_metatable["__index"] = parent_table;
        
        instance_table.push(lua_state);
        new_metatable.push(lua_state);
        lua_setmetatable(lua_state, -2);
        lua_pop(lua_state,1);
    }
    
    static void CppLog(std::string message) {
        cout << message << std::endl;
    }

    static void CppLogError(std::string message) {
        cout << message << std::endl;
    }
    
    
};




#endif
