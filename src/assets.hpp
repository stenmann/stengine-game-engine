#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <array>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "engineUtils.hpp"
#include "Actor.hpp"
#include "Helper.h"
#include "Template.hpp"
#include "AudioHelper.h"
#include "ComponentDB.hpp"

using namespace std;

class gameinfo {
private:
    engineUtils eu;
    templates actorTemplate;
    
public:

    
    const string baseDirect = "resources";
    const string gameConfig = "resources/game.config";
    const string cameraConfig = "resources/rendering.config";
    const string componentConfig = "resources/component_types";
    string sceneValue = "";
    string gameStart = "";

    
    vector<Actor*> actors;
    vector<Actor*> actorsToStart;
    
    gameinfo() {
        
        SceneDB::game = this;
        if (!eu.checkForDirectory(baseDirect)) return;
        if (!eu.checkForDirectory(gameConfig)) return;
        
        rapidjson::Document doc;
        eu.ReadJsonFile(gameConfig, doc);
        
        if (doc.HasMember("game_start_message")) gameStart = doc["game_start_message"].GetString();
        if (doc.HasMember("initial_scene")) sceneValue = doc["initial_scene"].GetString();
        
        
        if (!sceneValue.empty()) loadScene(sceneValue);
        
    }
    
    void loadScene(const string& sceneName) {
        vector<Actor*> survivors;
        for (Actor* actor : actors) {
            if (actor->doNotDestroy) {
                survivors.push_back(actor);
            } else {
                callOnDestroy(actor);
                delete actor;
            }
        }
        actors = survivors;
        actorsToStart.clear();

        SceneDB::current_scene = sceneName;
        
        string scenePath = "resources/scenes/" + sceneName + ".scene";
        if (!eu.checkForDirectory(scenePath)) {
            cout << "error: scene " << sceneName << " is missing";
            exit(0);
        }
        
        rapidjson::Document doc;
        eu.ReadJsonFile(scenePath, doc);
        
        if (!doc.HasMember("actors")) return;
        
        const auto& arrayAct = doc["actors"];
        actors.reserve(arrayAct.Size() + 100);
        
        for (int i = 0; i < arrayAct.Size(); i++) {
            Actor actor;
            
            if(arrayAct[i].HasMember("template"))
            {
                actor = actorTemplate.lookUpTemplate(arrayAct[i]["template"].GetString());
            }
            
            if (arrayAct[i].HasMember("name")) actor.actor_name = arrayAct[i]["name"].GetString();
            
            if(arrayAct[i].HasMember("components"))
            {
                const rapidjson::Value& componentsObj = arrayAct[i]["components"];
                
                for (auto it = componentsObj.MemberBegin(); it != componentsObj.MemberEnd(); ++it)
                {
                    std::string key = it->name.GetString();
                    if(actor.components.count(key) <= 0)
                    {
                        std::string type = it->value["type"].GetString();
                        
                        if (type == "ParticleSystem")
                        {
                            actor.components[key] = componentDB::getInstance().createComponent(key, type, "");
                            ParticleSystem* ps = (*actor.components[key]->componentRef).cast<ParticleSystem*>();
                            for (auto it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); it2++) {
                                string val = it2->name.GetString();
                                if (val == "type") continue;
                                if (val == "x" && it2->value.IsNumber()) ps->x = it2->value.GetFloat();
                                if (val == "y" && it2->value.IsNumber()) ps->y = it2->value.GetFloat();
                                if (val == "frames_between_bursts" && it2->value.IsNumber()) ps->frames_between_bursts = it2->value.GetInt();
                                if (val == "burst_quantity" && it2->value.IsNumber()) ps->burst_quantity = it2->value.GetInt();
                                if (val == "start_scale_min" && it2->value.IsNumber()) ps->start_scale_min = it2->value.GetFloat();
                                if (val == "start_scale_max" && it2->value.IsNumber()) ps->start_scale_max = it2->value.GetFloat();
                                if (val == "rotation_min" && it2->value.IsNumber()) ps->rotation_min = it2->value.GetFloat();
                                if (val == "rotation_max" && it2->value.IsNumber()) ps->rotation_max = it2->value.GetFloat();
                                if (val == "start_color_r" && it2->value.IsNumber()) ps->start_color_r = it2->value.GetInt();
                                if (val == "start_color_g" && it2->value.IsNumber()) ps->start_color_g = it2->value.GetInt();
                                if (val == "start_color_b" && it2->value.IsNumber()) ps->start_color_b = it2->value.GetInt();
                                if (val == "start_color_a" && it2->value.IsNumber()) ps->start_color_a = it2->value.GetInt();
                                if (val == "emit_angle_min" && it2->value.IsNumber()) ps->emit_angle_min = it2->value.GetFloat();
                                if (val == "emit_angle_max" && it2->value.IsNumber()) ps->emit_angle_max = it2->value.GetFloat();
                                if (val == "emit_radius_min" && it2->value.IsNumber()) ps->emit_radius_min = it2->value.GetFloat();
                                if (val == "emit_radius_max" && it2->value.IsNumber()) ps->emit_radius_max = it2->value.GetFloat();
                                if (val == "image" && it2->value.IsString()) ps->image = it2->value.GetString();
                                if (val == "sorting_order" && it2->value.IsNumber()) ps->sorting_order = it2->value.GetInt();
                                if(val == "duration_frames" && it2->value.IsNumber()) ps->duration_frames = it2->value.GetFloat();
                                if (val == "start_speed_min" && it2->value.IsNumber()) ps->start_speed_min = it2->value.GetFloat();
                                if (val == "start_speed_max" && it2->value.IsNumber()) ps->start_speed_max = it2->value.GetFloat();
                                if (val == "rotation_speed_min" && it2->value.IsNumber()) ps->rotation_speed_min = it2->value.GetFloat();
                                if (val == "rotation_speed_max" && it2->value.IsNumber()) ps->rotation_speed_max = it2->value.GetFloat();
                                if (val == "gravity_scale_x" && it2->value.IsNumber()) ps->gravity_scale_x = it2->value.GetFloat();
                                if (val == "gravity_scale_y" && it2->value.IsNumber()) ps->gravity_scale_y = it2->value.GetFloat();
                                if (val == "drag_factor" && it2->value.IsNumber()) ps->drag_factor = it2->value.GetFloat();
                                if (val == "angular_drag_factor" && it2->value.IsNumber()) ps->angular_drag_factor = it2->value.GetFloat();
                                if (val == "end_scale" && it2->value.IsNumber()) ps->end_scale = it2->value.GetFloat();
                                if (val == "end_color_r" && it2->value.IsNumber()) ps->end_color_r = it2->value.GetInt();
                                if (val == "end_color_g" && it2->value.IsNumber()) ps->end_color_g = it2->value.GetInt();
                                if (val == "end_color_b" && it2->value.IsNumber()) ps->end_color_b = it2->value.GetInt();
                                if (val == "end_color_a" && it2->value.IsNumber()) ps->end_color_a = it2->value.GetInt();
                                if (val == "enabled" && it2->value.IsBool()) ps->enabled = it2->value.GetBool();
                                
                            }
                            continue;
                        }
                        
                        if (type == "Rigidbody") {
                            actor.components[key] = componentDB::getInstance().createComponent(key, type, "");
                            Rigidbody* rb = (*actor.components[key]->componentRef).cast<Rigidbody*>();
                            for (auto it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); it2++) {
                                string val = it2->name.GetString();
                                if (val == "type") continue;
                                if (val == "x" && it2->value.IsNumber()) rb->x = it2->value.GetFloat();
                                if (val == "y" && it2->value.IsNumber()) rb->y = it2->value.GetFloat();
                                if (val == "body_type" && it2->value.IsString()) rb->body_type = it2->value.GetString();
                                if (val == "precise" && it2->value.IsBool()) rb->precise = it2->value.GetBool();
                                if (val == "gravity_scale" && it2->value.IsNumber()) rb->gravity_scale = it2->value.GetFloat();
                                if (val == "density" && it2->value.IsNumber()) rb->density = it2->value.GetFloat();
                                if (val == "angular_friction" && it2->value.IsNumber()) rb->angular_friction = it2->value.GetFloat();
                                if (val == "rotation" && it2->value.IsNumber()) rb->rotation = it2->value.GetFloat();
                                if (val == "has_collider" && it2->value.IsBool()) rb->has_collider = it2->value.GetBool();
                                if (val == "has_trigger" && it2->value.IsBool()) rb->has_trigger = it2->value.GetBool();
                                if (val == "collider_type" && it2->value.IsString()) rb->collider_type = it2->value.GetString();
                                if (val == "width" && it2->value.IsNumber()) rb->width = it2->value.GetFloat();
                                if (val == "height" && it2->value.IsNumber()) rb->height = it2->value.GetFloat();
                                if (val == "radius" && it2->value.IsNumber()) rb->radius = it2->value.GetFloat();
                                if (val == "friction" && it2->value.IsNumber()) rb->friction = it2->value.GetFloat();
                                if (val == "bounciness" && it2->value.IsNumber()) rb->bounciness = it2->value.GetFloat();
                                if (val == "trigger_type" && it2->value.IsString()) rb->trigger_type = it2->value.GetString();
                                if (val == "trigger_width" && it2->value.IsNumber()) rb->trigger_width = it2->value.GetFloat();
                                if (val == "trigger_height" && it2->value.IsNumber()) rb->trigger_height = it2->value.GetFloat();
                                if (val == "trigger_radius" && it2->value.IsNumber()) rb->trigger_radius = it2->value.GetFloat();
                            }
                            continue;
                        }
                        else {
                            string typePath = "resources/component_types/" + type + ".lua";
                            
                            if (!eu.checkForDirectory(typePath)) {
                                cout << "error: failed to locate component " << type;
                                exit(0);
                            }
                            actor.components[key] = componentDB::getInstance().createComponent(key, type,typePath);
                        }
                    }

                    luabridge::LuaRef& instance = *actor.components[key]->componentRef;
                    
                    for (auto it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd();it2++)
                    {
                        string val = it2->name.GetString();
                        
                        if(val == "type")
                            {continue;}
                        if(it2->value.IsString())
                        {
                            instance[val] = string(it2->value.GetString());
                        }
                        if(it2->value.IsInt())
                        {
                            instance[val] = it2->value.GetInt();
                        }
                        if(it2->value.IsBool())
                        {
                            instance[val] = it2->value.GetBool();
                        }
                        if(it2->value.IsDouble())
                        {
                            instance[val] = it2->value.GetDouble();
                        }
                        
                    }
                }
            }
            
            actor.id = i;
            
            Actor* actorPtr = new Actor(actor);
            actorPtr->lua_state = componentDB::getInstance().lua_state;
            
            for (auto& [key, comp] : actorPtr->components) {
                if (comp->type == "ParticleSystem") continue;
                actorPtr->InjectConvenienceReferences(comp->componentRef);
            }
            
            actors.push_back(actorPtr);
            actorsToStart.push_back(actorPtr);
        }
        Actor::all_actors = &actors;
        Actor::lua_state_static = componentDB::getInstance().lua_state;
    }
    
    void callOnStart() {
        vector<Actor*> toStart = actorsToStart;
        actorsToStart.clear();
        
        for (Actor* actor : toStart) {
            for (auto& [key, comp] : actor->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                if (comp->hasStart && !comp->hasStarted) {
                    comp->hasStarted = true;
                    
                    if (comp->type == "Rigidbody") {
                        Rigidbody* rb = instance.cast<Rigidbody*>();
                        rb->OnStart();
                    }
                    else if (comp->type == "ParticleSystem") {
                        ParticleSystem* ps = instance.cast<ParticleSystem*>();
                        ps->OnStart();
                    }
                    else {
                        if (instance["enabled"]) {
                            try {
                                instance["OnStart"](instance);
                            }
                            catch (const luabridge::LuaException& e) {
                                ReportError(actor->actor_name, e);
                            }
                        }
                    }
                }
            }
        }
    }
    
    void callOnUpdate() {
            for (Actor* actor : actors) {
                for (auto& [key, comp] : actor->components) {
                    if (comp->hasUpdate) {
                        luabridge::LuaRef& instance = *comp->componentRef;
                        if (comp->type == "ParticleSystem")
                            {
                            ParticleSystem* ps = instance.cast<ParticleSystem*>();
                            if (ps->enabled)
                                {ps->OnUpdate();}
                            }
                        else if (instance["enabled"]) {
                            try {
                                instance["OnUpdate"](instance);
                            }
                            catch (const luabridge::LuaException& e) {
                                ReportError(actor->actor_name, e);
                            }
                        }
                    }
                }
            }
        }
    
    void callOnLateUpdate() {
            for (Actor* actor : actors) {
                for (auto& [key, comp] : actor->components) {
                    if (comp->hasLateUpdate) {
                        luabridge::LuaRef& instance = *comp->componentRef;
                        if(instance["enabled"])
                        {
                            try {
                                instance["OnLateUpdate"](instance);
                            }
                            catch (const luabridge::LuaException& e)
                            {
                                ReportError(actor->actor_name, e);
                            }
                        }
                    }
                }
            }
        }
    
    void callOnDestroy(Actor* actor) {
        for (auto& [key, comp] : actor->components) {
            luabridge::LuaRef& instance = *comp->componentRef;
            if (comp->type == "Rigidbody") {
                instance.cast<Rigidbody*>()->OnDestroy();
            } else if (comp->type == "ParticleSystem") {
                // nothing to do, just skip safely
                continue;
            } else if (comp->hasDestroy) {
                try { instance["OnDestroy"](instance); }
                catch (const luabridge::LuaException& e) { ReportError(actor->actor_name, e); }
            }
        }
    }
    
    void processEventBus() {
        for (auto& unsub : EventBus::pending_unsubscribes) {
            std::string event_type = std::get<0>(unsub);
            luabridge::LuaRef component = std::get<1>(unsub);
            luabridge::LuaRef function = std::get<2>(unsub);
            
            auto& subs = EventBus::subscriptions[event_type];
            subs.erase(std::remove_if(subs.begin(), subs.end(),
                [component, function](const std::pair<luabridge::LuaRef, luabridge::LuaRef>& pair) {
                    lua_State* L = component.state();
                    pair.first.push(L);
                    component.push(L);
                    bool comp_eq = lua_rawequal(L, -1, -2);
                    lua_pop(L, 2);
                    pair.second.push(L);
                    function.push(L);
                    bool func_eq = lua_rawequal(L, -1, -2);
                    lua_pop(L, 2);
                    return comp_eq && func_eq;
                }), subs.end());
        }
        EventBus::pending_unsubscribes.clear();

        for (auto& sub : EventBus::pending_subscribes) {
            EventBus::subscriptions[std::get<0>(sub)].push_back({std::get<1>(sub), std::get<2>(sub)});
        }
        EventBus::pending_subscribes.clear();
    }
    
    
    
    static std::string obtain_word_after_phrase(const std::string& input, const std::string& phrase) {
        size_t pos = input.find(phrase);
        if (pos == std::string::npos) return "";
        pos += phrase.length();
        while (pos < input.size() && std::isspace(input[pos])) ++pos;
        if (pos == input.size()) return "";
        size_t endPos = pos;
        while (endPos < input.size() && !std::isspace(input[endPos])) ++endPos;
        return input.substr(pos, endPos - pos);
    }
    
    void ReportError(const string & actorName, const luabridge::LuaException & e)
    {
        string errorMessage = e.what();
        
        std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
        
        cout << "\033[31m" << actorName << " : " << errorMessage << "\033[0m" << endl;
    }
    
    void processPendingAdditions() {
        for (auto& [actor, comp] : Actor::pending_additions) {
            string key = (*comp->componentRef)["key"];
            actor->components[key] = comp;
            if (comp->type != "ParticleSystem" && comp->type != "Rigidbody")
                actor->InjectConvenienceReferences(comp->componentRef);
            actorsToStart.push_back(actor);
        }
        Actor::pending_additions.clear();
    }
    
    void processPendingRemovals() {
        for (auto& [actor, key] : Actor::pending_removal) {
            auto it = actor->components.find(key);
            if (it != actor->components.end()) {
                auto& comp = it->second;
                luabridge::LuaRef& instance = *comp->componentRef;
                if (comp->type == "Rigidbody")
                {
                    instance.cast<Rigidbody*>()->OnDestroy();
                }
                else if (comp->type == "ParticleSystem")
                {
                    continue;
                }
                else if (comp->hasDestroy) {
                    try { instance["OnDestroy"](instance); }
                    catch (...) {}
                }
                actor->components.erase(it);
            }
        }
        Actor::pending_removal.clear();
    }
    
    
    void processPendingDestroys() {
        for (Actor* actor : Actor::pending_destroys) {
            callOnDestroy(actor);
            // remove from all_actors
            Actor::all_actors->erase(
                remove(Actor::all_actors->begin(), Actor::all_actors->end(), actor),
                                     Actor::all_actors->end()
            );
            // remove from actors vector
            actors.erase(
                remove(actors.begin(), actors.end(), actor),
                actors.end()
            );
            delete actor;
        }
        Actor::pending_destroys.clear();
    }
    
    void processPendingActors() {
        for (Actor* actor : Actor::pending_actors) {
            actors.push_back(actor);
            actorsToStart.push_back(actor);
            for (auto& [key, comp] : actor->components) {
                if (comp->type == "ParticleSystem" || comp->type == "Rigidbody") continue;
                actor->InjectConvenienceReferences(comp->componentRef);
            }
        }
        Actor::pending_actors.clear();
    }
    
    void processPendingSceneLoad() {
        if (!SceneDB::scene_pending) return;
        SceneDB::scene_pending = false;
        loadScene(SceneDB::next_scene);
    }
    
};

#endif
