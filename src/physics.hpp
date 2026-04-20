
#ifndef PHYSICS_H
#define PHYSICS_H

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

struct collision {
    
    Actor* other = nullptr;
    b2Vec2 point;
    b2Vec2 relative_velocity;
    b2Vec2 normal;
};

static void ReportError(const string& actorName, const luabridge::LuaException& e) {
    string errorMessage = e.what();
    std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
    cout << "\033[31m" << actorName << " : " << errorMessage << "\033[0m" << endl;
}

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        b2WorldManifold world_manifold;
        contact->GetWorldManifold(&world_manifold);
        
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        
        Actor* actorA = reinterpret_cast<Actor*>(fixtureA->GetUserData().pointer);
        Actor* actorB = reinterpret_cast<Actor*>(fixtureB->GetUserData().pointer);
        
        if (actorA == nullptr || actorB == nullptr) return;

        bool sensorA = fixtureA->IsSensor();
        bool sensorB = fixtureB->IsSensor();

        if (sensorA && sensorB) {
            b2Vec2 relative_velocity = fixtureA->GetBody()->GetLinearVelocity()
                                         - fixtureB->GetBody()->GetLinearVelocity();
            collision collisionA;
            collision collisionB;
            collisionA.other = actorB;
            collisionB.other = actorA;
            // trigger path - sentinel values
            collisionA.point = b2Vec2(-999.0f, -999.0f);
            collisionA.normal = b2Vec2(-999.0f, -999.0f);
            collisionA.relative_velocity = relative_velocity;
            collisionB.point = b2Vec2(-999.0f, -999.0f);
            collisionB.normal = b2Vec2(-999.0f, -999.0f);
            collisionB.relative_velocity = relative_velocity;
            
            for (auto& [key, comp] : actorA->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onTriggerEnter = instance["OnTriggerEnter"];
                if (onTriggerEnter.isFunction()) {
                    try { onTriggerEnter(instance, collisionA); }
                    catch (const luabridge::LuaException& e) { ReportError(actorA->actor_name, e); }
                }
            }
            for (auto& [key, comp] : actorB->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onTriggerEnter = instance["OnTriggerEnter"];
                if (onTriggerEnter.isFunction()) {
                    try { onTriggerEnter(instance, collisionB); }
                    catch (const luabridge::LuaException& e) { ReportError(actorB->actor_name, e); }
                }
            }
        }
        
        else if (!sensorA && !sensorB) {
            
            b2Vec2 relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();
            
            collision collisionA;
            collisionA.other = actorB;
            collisionA.point = world_manifold.points[0];
            collisionA.relative_velocity = relative_velocity;
            collisionA.normal = world_manifold.normal;
            
            collision collisionB;
            collisionB.other = actorA;
            collisionB.point = world_manifold.points[0];
            collisionB.relative_velocity = relative_velocity;
            collisionB.normal = world_manifold.normal;
            
            for (auto& [key, comp] : actorA->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onCollisionEnter = instance["OnCollisionEnter"];
                if (onCollisionEnter.isFunction()) {
                    try {
                        onCollisionEnter(instance, collisionA);
                    }
                    catch (const luabridge::LuaException& e) {
                        ReportError(actorA->actor_name, e);
                    }
                }
            }
            
            for (auto& [key, comp] : actorB->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onCollisionEnter = instance["OnCollisionEnter"];
                if (onCollisionEnter.isFunction()) {
                    try {
                        onCollisionEnter(instance, collisionB);
                    }
                    catch (const luabridge::LuaException& e) {
                        ReportError(actorB->actor_name, e);
                    }
                }
            }
        }
    }
    
    void EndContact(b2Contact* contact)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        
        Actor* actorA = reinterpret_cast<Actor*>(fixtureA->GetUserData().pointer);
        Actor* actorB = reinterpret_cast<Actor*>(fixtureB->GetUserData().pointer);
        
        if (actorA == nullptr || actorB == nullptr) return;
        
        bool sensorA = fixtureA->IsSensor();
        bool sensorB = fixtureB->IsSensor();
        
        b2Vec2 relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();
        
        if (sensorA && sensorB) {
            collision collisionA;
            collision collisionB;
            collisionA.other = actorB;
            collisionA.point = b2Vec2(-999.0f, -999.0f);
            collisionA.normal = b2Vec2(-999.0f, -999.0f);
            collisionA.relative_velocity = relative_velocity;
            collisionB.other = actorA;
            collisionB.point = b2Vec2(-999.0f, -999.0f);
            collisionB.normal = b2Vec2(-999.0f, -999.0f);
            collisionB.relative_velocity = relative_velocity;
            
            for (auto& [key, comp] : actorA->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onTriggerExit = instance["OnTriggerExit"];
                if (onTriggerExit.isFunction()) {
                    try { onTriggerExit(instance, collisionA); }
                    catch (const luabridge::LuaException& e) { ReportError(actorA->actor_name, e); }
                }
            }
            for (auto& [key, comp] : actorB->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onTriggerExit = instance["OnTriggerExit"];
                if (onTriggerExit.isFunction()) {
                    try { onTriggerExit(instance, collisionB); }
                    catch (const luabridge::LuaException& e) { ReportError(actorB->actor_name, e); }
                }
            }
        }
        else if (!sensorA && !sensorB) {
            collision collisionA;
            collision collisionB;
            collisionA.other = actorB;
            collisionA.point = b2Vec2(-999.0f, -999.0f);
            collisionA.normal = b2Vec2(-999.0f, -999.0f);
            collisionA.relative_velocity = relative_velocity;
            collisionB.other = actorA;
            collisionB.point = b2Vec2(-999.0f, -999.0f);
            collisionB.normal = b2Vec2(-999.0f, -999.0f);
            collisionB.relative_velocity = relative_velocity;
            
            for (auto& [key, comp] : actorA->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onCollisionExit = instance["OnCollisionExit"];
                if (onCollisionExit.isFunction()) {
                    try { onCollisionExit(instance, collisionA); }
                    catch (const luabridge::LuaException& e) { ReportError(actorA->actor_name, e); }
                }
            }
            for (auto& [key, comp] : actorB->components) {
                luabridge::LuaRef& instance = *comp->componentRef;
                luabridge::LuaRef onCollisionExit = instance["OnCollisionExit"];
                if (onCollisionExit.isFunction()) {
                    try { onCollisionExit(instance, collisionB); }
                    catch (const luabridge::LuaException& e) { ReportError(actorB->actor_name, e); }
                }
            }
        }
    }

};

struct HitResult {
    Actor* actor;
    b2Vec2 point;
    b2Vec2 normal;
    bool is_trigger;
};

class RaycastCallback : public b2RayCastCallback {
public:
    std::vector<HitResult> hits;

    float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                        const b2Vec2& normal, float fraction) override {
        
        Actor* actor = reinterpret_cast<Actor*>(fixture->GetUserData().pointer);
        
        // Ignore phantom fixtures (no actor pointer)
        if (actor == nullptr) return -1;
        
        HitResult hit;
        hit.actor = actor;
        hit.point = point;
        hit.normal = normal;
        hit.is_trigger = fixture->IsSensor();
        hits.push_back(hit);
        
        return 1; // return 1 = don't clip the ray, keep going, collect all hits
    }
};

class physics{
public:
    static inline b2World* physics_world = nullptr;
    static inline ContactListener contact_listener;
    static inline lua_State* lua_state = nullptr; 
    physics()
    {
        b2Vec2 gravity(0.0f,9.8f);
        physics_world = new b2World(gravity);
        physics_world->SetContactListener(&contact_listener);
    }
    
    static void Step() {
        if (!physics_world) return;
        physics_world->Step(1.0f / 60.0f, 8, 3);
    }
    
    static luabridge::LuaRef Raycast(b2Vec2 pos, b2Vec2 dir, float dist) {
        // Edge cases: no world, or invalid distance
        if (!physics_world || dist <= 0)
            return luabridge::LuaRef(lua_state); // nil
        
        dir.Normalize();
        b2Vec2 end = pos + dist * dir;
        
        RaycastCallback callback;
        physics_world->RayCast(&callback, pos, end);
        
        if (callback.hits.empty())
            return luabridge::LuaRef(lua_state); // nil
        
        // Sort by distance (fraction isn't stored, so sort by distance from pos)
        std::sort(callback.hits.begin(), callback.hits.end(),
            [&pos](const HitResult& a, const HitResult& b) {
                return b2Distance(pos, a.point) < b2Distance(pos, b.point);
            });
        
        return luabridge::LuaRef(lua_state, callback.hits[0]); // closest
    }
    
    static luabridge::LuaRef RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist) {
        luabridge::LuaRef result_table = luabridge::newTable(lua_state);
        
        if (!physics_world || dist <= 0)
            return result_table; // empty table
        
        dir.Normalize();
        b2Vec2 end = pos + dist * dir;
        
        RaycastCallback callback;
        physics_world->RayCast(&callback, pos, end);
        
        // Sort nearest to furthest
        std::sort(callback.hits.begin(), callback.hits.end(),
            [&pos](const HitResult& a, const HitResult& b) {
                return b2Distance(pos, a.point) < b2Distance(pos, b.point);
            });
        
        for (int i = 0; i < callback.hits.size(); i++) {
            result_table[i + 1] = callback.hits[i];
        }
        
        return result_table;
    }
    
};
    
class Rigidbody {
public:
    std::string key = "";
    bool enabled = true;
    Actor* actor = nullptr;
    //dummy comment
    float x = 0.0f;
    float y = 0.0f;
    string body_type = "dynamic";
    bool precise = true;
    float gravity_scale = 1.0f;
    float density = 1.0f;
    float angular_friction = 0.3f;
    float rotation = 0.0f;
    bool has_collider = true;
    bool has_trigger = true;
    string collider_type = "box";
    float width = 1.0f;
    float height = 1.0f;
    float radius = 0.5f;
    float friction = 0.3f;
    float bounciness = 0.3f;
    
    // trigger
    string trigger_type = "box";
    float trigger_width = 1.0f;
    float trigger_height = 1.0f;
    float trigger_radius = 0.5f;
    
    b2Body* body = nullptr;
    
    void OnStart() {
        //dummy
        b2BodyDef body_def;
        if (body_type == "dynamic") body_def.type = b2_dynamicBody;
        else if (body_type == "kinematic") body_def.type = b2_kinematicBody;
        else if (body_type == "static") body_def.type = b2_staticBody;
        
        body_def.gravityScale = gravity_scale;
        body_def.position = b2Vec2(x, y);
        body_def.bullet = precise;
        body_def.angularDamping = angular_friction;
        body_def.angle = rotation * (b2_pi / 180.0f);
        
        body = physics::physics_world->CreateBody(&body_def);
        
        if(!has_collider && !has_trigger)
        {
            b2PolygonShape phantom_shape;
            phantom_shape.SetAsBox(width * 0.5f, height * 0.5f);
            
            b2FixtureDef phantom_fixture_def;
            phantom_fixture_def.shape = &phantom_shape;
            phantom_fixture_def.density = density;
            phantom_fixture_def.isSensor = true;
            phantom_fixture_def.userData.pointer = 0;
            phantom_fixture_def.filter.categoryBits = 0x0004;  // phantom category
            phantom_fixture_def.filter.maskBits = 0x0004;
            body->CreateFixture(&phantom_fixture_def);
        }
            if(has_collider)
            {
                if(collider_type == "box")
                {
                    b2PolygonShape box;
                    box.SetAsBox(width * 0.5f,height * 0.5f);
                    b2FixtureDef fixture_def;
                    fixture_def.isSensor = false;
                    fixture_def.shape = &box;
                    fixture_def.density = density;
                    fixture_def.friction = friction;
                    fixture_def.restitution = bounciness;
                    fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
                    fixture_def.filter.categoryBits = 0x0001;  // category: collider
                    fixture_def.filter.maskBits = 0x0001;
                    body->CreateFixture(&fixture_def);
                }
                if(collider_type == "circle")
                {
                    b2CircleShape circle;
                    circle.m_radius = radius;
                    b2FixtureDef fixture_def;
                    fixture_def.isSensor = false;
                    fixture_def.shape = &circle;
                    fixture_def.density = density;
                    fixture_def.friction = friction;
                    fixture_def.restitution = bounciness;
                    fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
                    fixture_def.filter.categoryBits = 0x0001;  // category: collider
                    fixture_def.filter.maskBits = 0x0001;
                    body->CreateFixture(&fixture_def);
                }
            }
            if(has_trigger)
            {
                if(trigger_type == "box")
                {
                    b2PolygonShape trigger_box;
                    trigger_box.SetAsBox(trigger_width * 0.5f, trigger_height * 0.5f);
                    b2FixtureDef trigger_def;
                    trigger_def.shape = &trigger_box;
                    trigger_def.isSensor = true;
                    trigger_def.density = density;
                    trigger_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
                    trigger_def.filter.categoryBits = 0x0002;  // category: trigger
                    trigger_def.filter.maskBits = 0x0002;
                    body->CreateFixture(&trigger_def);
                }
                if(trigger_type == "circle")
                {
                    b2CircleShape trigger_circle;
                    trigger_circle.m_radius = trigger_radius;
                    b2FixtureDef trigger_def;
                    trigger_def.shape = &trigger_circle;
                    trigger_def.isSensor = true;
                    trigger_def.density = density;
                    trigger_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
                    trigger_def.filter.categoryBits = 0x0002;  // category: trigger
                    trigger_def.filter.maskBits = 0x0002;
                    body->CreateFixture(&trigger_def);
                }
            }

    }
    
    void OnDestroy() {
        if (body && physics::physics_world) {
            physics::physics_world->DestroyBody(body);
            body = nullptr;
        }
    }
    
    b2Vec2 GetPosition() {
        if (!body) return b2Vec2(x, y);
        return body->GetPosition();
    }

    float GetRotation() {
        if (!body) return rotation;
        return body->GetAngle() * (180.0f / b2_pi);
    }

    void AddForce(b2Vec2 force) {
        if (!body) return;
        body->ApplyForceToCenter(force, true);
    }

    void SetVelocity(b2Vec2 vec) {
        if (!body) return;
        body->SetLinearVelocity(vec);
    }

    void SetPosition(b2Vec2 pos) {
        if (!body) { x = pos.x; y = pos.y; return; }
        body->SetTransform(pos, body->GetAngle());
    }

    void SetRotation(float degrees) {
        if (!body) { rotation = degrees; return; }
        float radians = degrees * (b2_pi / 180.0f);
        body->SetTransform(body->GetPosition(), radians);
    }

    void SetAngularVelocity(float degrees_clockwise) {
        if (!body) return;
        body->SetAngularVelocity(degrees_clockwise * (b2_pi / 180.0f));
    }

    void SetGravityScale(float scale) {
        if (!body) { gravity_scale = scale; return; }
        body->SetGravityScale(scale);
    }

    void SetUpDirection(b2Vec2 direction) {
        if (!body) return;
        direction.Normalize();
        float new_angle_radians = glm::atan(direction.x, -direction.y);
        body->SetTransform(body->GetPosition(), new_angle_radians);
    }

    void SetRightDirection(b2Vec2 direction) {
        if (!body) return;
        direction.Normalize();
        float new_angle_radians = glm::atan(direction.x, -direction.y) - (b2_pi / 2.0f);
        body->SetTransform(body->GetPosition(), new_angle_radians);
    }

    b2Vec2 GetVelocity() {
        if (!body) return b2Vec2(0, 0);
        return body->GetLinearVelocity();
    }

    float GetAngularVelocity() {
        if (!body) return 0.0f;
        return body->GetAngularVelocity() * (180.0f / b2_pi);
    }

    float GetGravityScale() {
        if (!body) return gravity_scale;
        return body->GetGravityScale();
    }

    b2Vec2 GetUpDirection() {
        if (!body) return b2Vec2(0, -1);
        float angle = body->GetAngle();
        b2Vec2 result = b2Vec2(glm::sin(angle), -glm::cos(angle));
        result.Normalize();
        return result;
    }

    b2Vec2 GetRightDirection() {
        if (!body) return b2Vec2(1, 0);
        float angle = body->GetAngle();
        b2Vec2 result = b2Vec2(glm::cos(angle), glm::sin(angle));
        result.Normalize();
        return result;
    }
    
    
        
    };


    
    


#endif
