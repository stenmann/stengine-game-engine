#ifndef SCENEDB_HPP
#define SCENEDB_HPP

#include <string>

// Forward declare gameinfo to avoid circular includes
class gameinfo;

class SceneDB {
public:
    static inline std::string current_scene = "";
    static inline std::string next_scene    = "";
    static inline bool        scene_pending = false;

    // Pointer back to gameinfo so SceneDB can trigger the actual load
    static inline gameinfo* game = nullptr;

    static void Load(std::string scene_name) {
        next_scene    = scene_name;
        scene_pending = true;
    }

    static std::string GetCurrent() {
        return current_scene;
    }

    static void DontDestroy(Actor* actor) {
        actor->doNotDestroy = true;
    }
};

#endif
