//
//  Template.hpp
//  game_engine
//
//  Created by Nickolas Stenman on 2/16/26.
//

#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <filesystem>
#include <iostream>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "engineUtils.hpp"
#include "Actor.hpp"
#include "ComponentDB.hpp"

using namespace std;

class templates {
private:
    
public:
    const string gameConfig = "resources/actor_templates/";
    engineUtils eu;
    static inline std::unordered_map<std::string, rapidjson::Document*> doc_cache;
    
    Actor lookUpTemplate(const string& templateName)
    {

        
        string scenePath = gameConfig + templateName + ".template";
        rapidjson::Document* doc;
            if (doc_cache.count(templateName) == 0) {
                doc = new rapidjson::Document();
                eu.ReadJsonFile(scenePath, *doc);
                doc_cache[templateName] = doc;
            } else {
                doc = doc_cache[templateName];
            }
        Actor templateActor;

        if (doc->HasMember("name")) templateActor.actor_name = (*doc)["name"].GetString();
        if (doc->HasMember("components"))
        {
            const rapidjson::Value& componentsObj = (*doc)["components"];
            
            for (auto it = componentsObj.MemberBegin(); it != componentsObj.MemberEnd(); ++it)
            {
                std::string key = it->name.GetString();
                std::string type = it->value["type"].GetString();
                if (type == "ParticleSystem") {
                    templateActor.components[key] = componentDB::getInstance().createComponent(key, type, "");
                    ParticleSystem* ps = (*templateActor.components[key]->componentRef).cast<ParticleSystem*>();
                    for (auto it2 = it->value.MemberBegin(); it2 != it->value.MemberEnd(); it2++) {
                        string val = it2->name.GetString();
                        if (val == "type") continue;
                        if (val == "enabled" && it2->value.IsBool()) ps->enabled = it2->value.GetBool();
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
                        if (val == "duration_frames" && it2->value.IsNumber()) ps->duration_frames = it2->value.GetInt();
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
                    }
                    continue;
                }

                if (type == "Rigidbody") {
                    templateActor.components[key] = componentDB::getInstance().createComponent(key, type, "");
                    Rigidbody* rb = (*templateActor.components[key]->componentRef).cast<Rigidbody*>();
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
                    continue;  // skip the rest of the loop body
                }
                
                string typePath = "resources/component_types/" + type + ".lua";
                
                if (!eu.checkForDirectory(typePath)) {
                    cout << "error: failed to locate component " << type;
                    exit(0);
                }

                templateActor.components[key] = componentDB::getInstance().createComponent(key, type,typePath);
                
                luabridge::LuaRef& instance = *templateActor.components[key]->componentRef;
                
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
        
        return templateActor;
    }
};

#endif
