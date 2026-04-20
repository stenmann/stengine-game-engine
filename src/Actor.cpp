#include "Actor.hpp"
#include "ComponentDB.hpp"
#include "Template.hpp"

luabridge::LuaRef Actor::AddComponent(std::string type)
{
    std::string key = "r" + std::to_string(n++);
    std::string typePath = "resources/component_types/" + type + ".lua";
    
    auto comp = componentDB::getInstance().createComponent(key, type, typePath);
    (*comp->componentRef)["enabled"] = true;
    InjectConvenienceReferences(comp->componentRef);
    pending_additions.push_back({this, comp});
    
    return *comp->componentRef;
}

void Actor::RemoveComponent(luabridge::LuaRef ref)
{
    ref["enabled"] = false;
    std::string key = ref["key"];
    pending_removal.push_back({this, key});
}

luabridge::LuaRef Actor::Instantiate(std::string templateName)
{
    static templates actorTemplate;
    Actor act = actorTemplate.lookUpTemplate(templateName);

    Actor* newActor = new Actor();
    newActor->actor_name = act.actor_name;
    newActor->id = static_cast<int>(Actor::all_actors->size());
    newActor->lua_state = lua_state_static;

    for (auto& [key, comp] : act.components) {
        if (comp->type == "ParticleSystem") {
            ParticleSystem* original = comp->componentRef->cast<ParticleSystem*>();
            ParticleSystem* ps = new ParticleSystem(*original); // deep copy
            auto newComp = std::make_shared<Component>();
            newComp->type = "ParticleSystem";
            newComp->hasStart = true;
            newComp->hasUpdate = true;
            newComp->componentRef = std::make_shared<luabridge::LuaRef>(lua_state_static, ps);
            newActor->components[key] = newComp;
        } else {
            newActor->components[key] = comp;
        }
    }

    for (auto& [key, comp] : newActor->components) {
        if (comp->type != "ParticleSystem" && comp->type != "Rigidbody")
            newActor->InjectConvenienceReferences(comp->componentRef);
    }

    pending_actors.push_back(newActor);
    return luabridge::LuaRef(lua_state_static, newActor);
}

void Actor::Destroy(Actor* actor)
{
    actor->destroyed = true;
    for (auto& [key, comp] : actor->components) {
        if (comp->type == "ParticleSystem") {
            comp->componentRef->cast<ParticleSystem*>()->enabled = false; // must NOT use []
        } else {
            (*comp->componentRef)["enabled"] = false;
        }
    }

    pending_actors.erase(
        remove(pending_actors.begin(), pending_actors.end(), actor),
        pending_actors.end()
    );

    pending_destroys.push_back(actor);
}
