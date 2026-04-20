#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <optional>
#include "glm/glm.hpp"
#include <unordered_set>
#include <map>
#include "ImageDB.hpp"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

class ParticleSystem
{
public:
    bool enabled = true;
    bool playing = true;
    std::string key = "";
    string type = "ParticleSystem";
    RandomEngine emit_angle_distribution;
    RandomEngine emit_radius_distribution;
    RandomEngine start_scale_distribution;
    RandomEngine rotation_distribution;
    RandomEngine speed_distribution;
    RandomEngine rotation_speed_distribution;
    float x = 0.0f;
    float y = 0.0f;
    int frameNumber = 0;
    int frames_between_bursts = 1;
    int burst_quantity = 1;
    float start_scale_min = 1.0f;
    float start_scale_max = 1.0f;
    float rotation_min = 0.0f;
    float rotation_max = 0.0f;
    int start_color_r = 255;
    int start_color_g = 255;
    int start_color_b = 255;
    int start_color_a = 255;
    float emit_angle_min = 0.0f;
    float emit_angle_max = 360.0f;
    float emit_radius_min = 0.0f;
    float emit_radius_max = 0.5f;
    string image = "";
    string textureName = "default";
    int sorting_order = 9999;
    int duration_frames = 300;
    float start_speed_min = 0.0f;
    float start_speed_max = 0.0f;
    float rotation_speed_min = 0.0f;
    float rotation_speed_max = 0.0f;
    float gravity_scale_x = 0.0f;
    float gravity_scale_y = 0.0f;
    float drag_factor = 1.0f;
    float angular_drag_factor = 1.0f;
    float end_scale = -1.0f;
    int end_color_r = -1;
    int end_color_g = -1;
    int end_color_b = -1;
    int end_color_a = -1;
    
    
    int numberOfParticleSlots = 0;
    int numberOfParticles = 0;
    struct ParticleData {
        std::vector<float> x;
        std::vector<float> y;
        std::vector<float> initialScale;
        std::vector<float> rotation;

        std::vector<int> r, g, b, a;
        std::vector<int> startFrame;
        std::vector<bool> isActive;
        vector<float> startingVelX;
        vector<float> startingVelY;
        vector<float> angularVel;
        vector<float> scale;
        std::vector<int> end_r, end_g, end_b, end_a;
    };

    ParticleData data;
    queue<int> free_list;
    
    ParticleSystem() = default;

    ParticleSystem(const ParticleSystem& other) {
        enabled = other.enabled;  key = other.key;
        x = other.x;  y = other.y;
        frames_between_bursts = other.frames_between_bursts;
        burst_quantity = other.burst_quantity;
        start_scale_min = other.start_scale_min;  start_scale_max = other.start_scale_max;
        rotation_min = other.rotation_min;  rotation_max = other.rotation_max;
        start_color_r = other.start_color_r;  start_color_g = other.start_color_g;
        start_color_b = other.start_color_b;  start_color_a = other.start_color_a;
        emit_angle_min = other.emit_angle_min;  emit_angle_max = other.emit_angle_max;
        emit_radius_min = other.emit_radius_min;  emit_radius_max = other.emit_radius_max;
        image = other.image;  textureName = other.textureName;
        sorting_order = other.sorting_order;
        duration_frames = other.duration_frames;
        start_speed_min = other.start_speed_min;  start_speed_max = other.start_speed_max;
        rotation_speed_min = other.rotation_speed_min;  rotation_speed_max = other.rotation_speed_max;
        gravity_scale_x = other.gravity_scale_x;  gravity_scale_y = other.gravity_scale_y;
        drag_factor = other.drag_factor;  angular_drag_factor = other.angular_drag_factor;
        end_scale = other.end_scale;
        end_color_r = other.end_color_r;  end_color_g = other.end_color_g;
        end_color_b = other.end_color_b;  end_color_a = other.end_color_a;
        // runtime state intentionally reset to zero/empty
        frameNumber = 0;  numberOfParticleSlots = 0;  numberOfParticles = 0;
    }

    void OnStart()
    {
        if (image != "")
            textureName = image;
        else
            imagesDB::CreateDefaultParticleTextureWithName(textureName);

        emit_angle_distribution  = RandomEngine(emit_angle_min,  emit_angle_max,  298);
        emit_radius_distribution = RandomEngine(emit_radius_min, emit_radius_max, 404);
        start_scale_distribution = RandomEngine(start_scale_min, start_scale_max, 494);
        rotation_distribution    = RandomEngine(rotation_min,    rotation_max,    440);
        speed_distribution = RandomEngine(start_speed_min, start_speed_max, 498);
        rotation_speed_distribution = RandomEngine(rotation_speed_min, rotation_speed_max, 305);
    }

    void EmitBurst()
    {
        for (int n = 0; n < burst_quantity; n++)
        {
            int i;

            if (!free_list.empty()) {
                i = free_list.front();
                free_list.pop();
            } else {
                i = numberOfParticleSlots;

                data.x.push_back(0);
                data.y.push_back(0);
                data.initialScale.push_back(0);
                data.scale.push_back(0);
                data.rotation.push_back(0);
                data.r.push_back(0);
                data.g.push_back(0);
                data.b.push_back(0);
                data.a.push_back(0);
                data.startFrame.push_back(0);
                data.isActive.push_back(false);
                data.startingVelX.push_back(0);
                data.startingVelY.push_back(0);
                data.angularVel.push_back(0);

                data.end_r.push_back(0);
                data.end_g.push_back(0);
                data.end_b.push_back(0);
                data.end_a.push_back(0);

                numberOfParticleSlots++;
            }

            float angleRadians = glm::radians(emit_angle_distribution.Sample());
            float radius       = emit_radius_distribution.Sample();

            data.x[i] = x + glm::cos(angleRadians) * radius;
            data.y[i] = y + glm::sin(angleRadians) * radius;
            
            float speed = speed_distribution.Sample();
            data.startingVelX[i] = glm::cos(angleRadians) * speed;
            data.startingVelY[i] = glm::sin(angleRadians) * speed;
            data.angularVel[i] = rotation_speed_distribution.Sample();
            
            
            data.initialScale[i] = start_scale_distribution.Sample();
            data.scale[i] = data.initialScale[i];
            data.rotation[i] = rotation_distribution.Sample();

            data.r[i] = start_color_r;
            data.g[i] = start_color_g;
            data.b[i] = start_color_b;
            data.a[i] = start_color_a;
            data.end_r[i] = data.r[i];
            data.end_g[i] = data.g[i];
            data.end_b[i] = data.b[i];
            data.end_a[i] = data.a[i];

            data.startFrame[i] = frameNumber;
            data.isActive[i] = true;

            numberOfParticles++;
        }
    }

    void OnUpdate()
    {
        frames_between_bursts = glm::max(1, frames_between_bursts);
        burst_quantity        = glm::max(1, burst_quantity);
        if(duration_frames < 1)
        {
            duration_frames = 1;
        }

        if (playing && frameNumber % frames_between_bursts == 0)
            {EmitBurst();}

        for (int i = 0; i < numberOfParticleSlots;i++) {
            if(data.isActive[i] == false)
            {
                continue;
            }
            
            int frames_particle_has_been_alive = frameNumber - data.startFrame[i];
            if(frames_particle_has_been_alive >= duration_frames)
            {
                data.isActive[i] = false;
                free_list.push(i);
                numberOfParticles--;
                continue;
            }
            data.startingVelX[i] += gravity_scale_x;
            data.startingVelY[i] += gravity_scale_y;
            data.startingVelX[i] *= drag_factor;
            data.startingVelY[i] *= drag_factor;
            data.angularVel[i] *= angular_drag_factor;
            data.x[i] += data.startingVelX[i];
            data.y[i] += data.startingVelY[i];
            data.rotation[i] += data.angularVel[i];
            
            float lifetime_progress = static_cast<float>(frames_particle_has_been_alive)/duration_frames;
            
            if (end_scale >= 0.0f)
            {
                data.scale[i] = glm::mix(data.initialScale[i],end_scale,lifetime_progress);
            }
            
            if (end_color_r != -1) data.end_r[i] = (int)glm::mix((float)data.r[i], (float)end_color_r, lifetime_progress);
            if (end_color_g != -1) data.end_g[i] = (int)glm::mix((float)data.g[i], (float)end_color_g, lifetime_progress);
            if (end_color_b != -1) data.end_b[i] = (int)glm::mix((float)data.b[i], (float)end_color_b, lifetime_progress);
            if (end_color_a != -1) data.end_a[i] = (int)glm::mix((float)data.a[i], (float)end_color_a, lifetime_progress);
            imagesDB::DrawEx(textureName, data.x[i], data.y[i],
                data.rotation[i], data.scale[i], data.scale[i], 0.5f, 0.5f,
            data.end_r[i], data.end_g[i], data.end_b[i], data.end_a[i], sorting_order);
            
        }

        frameNumber++;
        //dummy
    }
    
    void Stop()  { playing = false; }
    void Play()  { playing = true;  }
    void Burst() { EmitBurst();     }
};

#endif
