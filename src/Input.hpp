#ifndef INPUT_H
#define INPUT_H

#include "SDL2/SDL.h"
#include <unordered_map>
#include <vector>
#include "glm/glm.hpp"

enum INPUT_STATE { INPUT_STATE_UP, INPUT_STATE_JUST_BECAME_DOWN, INPUT_STATE_DOWN, INPUT_STATE_JUST_BECAME_UP };

class Input
{
private:
    static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyboard_states;
    static inline std::vector<SDL_Scancode> just_became_down_scancodes;
    static inline std::vector<SDL_Scancode> just_became_up_scancodes;
    
    static inline glm::vec2 mouse_position;
    static inline unordered_map<int, INPUT_STATE> mouse_button_states;
    static inline vector<int> just_became_down_buttons;
    static inline vector <int> just_became_up_buttonss;
    static inline std::unordered_map<std::string, SDL_Scancode> key_to_scancode;
    
    static inline float mouse_scroll_this_frame = 0;
    
    static SDL_Scancode GetScancodeFromString(std::string keycode)
    {
        auto it = key_to_scancode.find(keycode);
        if (it != key_to_scancode.end())
            return it->second;
        return SDL_GetScancodeFromName(keycode.c_str());
    }

    
public:
    static void Init()
    {
        // Directional
        keyboard_states[SDL_SCANCODE_UP] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_DOWN] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_RIGHT] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_LEFT] = INPUT_STATE_UP;

        // Misc
        keyboard_states[SDL_SCANCODE_ESCAPE] = INPUT_STATE_UP;

        // Modifier
        keyboard_states[SDL_SCANCODE_LSHIFT] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_RSHIFT] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_LCTRL] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_RCTRL] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_LALT] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_RALT] = INPUT_STATE_UP;

        // Editing
        keyboard_states[SDL_SCANCODE_TAB] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_RETURN] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_BACKSPACE] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_DELETE] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_INSERT] = INPUT_STATE_UP;

        // Space
        keyboard_states[SDL_SCANCODE_SPACE] = INPUT_STATE_UP;

        // Letters
        keyboard_states[SDL_SCANCODE_A] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_B] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_C] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_D] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_E] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_F] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_G] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_H] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_I] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_J] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_K] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_L] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_M] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_N] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_O] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_P] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_Q] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_R] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_S] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_T] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_U] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_V] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_W] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_X] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_Y] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_Z] = INPUT_STATE_UP;

        // Numbers
        keyboard_states[SDL_SCANCODE_0] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_1] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_2] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_3] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_4] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_5] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_6] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_7] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_8] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_9] = INPUT_STATE_UP;

        // Special characters
        keyboard_states[SDL_SCANCODE_SLASH] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_SEMICOLON] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_EQUALS] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_MINUS] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_PERIOD] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_COMMA] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_LEFTBRACKET] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_RIGHTBRACKET] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_BACKSLASH] = INPUT_STATE_UP;
        keyboard_states[SDL_SCANCODE_APOSTROPHE] = INPUT_STATE_UP;
        
        key_to_scancode["lshift"] = SDL_SCANCODE_LSHIFT;
        key_to_scancode["rshift"] = SDL_SCANCODE_RSHIFT;
        key_to_scancode["lctrl"] = SDL_SCANCODE_LCTRL;
        key_to_scancode["rctrl"] = SDL_SCANCODE_RCTRL;
        key_to_scancode["lalt"] = SDL_SCANCODE_LALT;
        key_to_scancode["ralt"] = SDL_SCANCODE_RALT;
        key_to_scancode["return"] = SDL_SCANCODE_RETURN;
        key_to_scancode["enter"] = SDL_SCANCODE_RETURN;
    }
    
    // Call every frame at start of event loop.
    static void ProcessEvent(const SDL_Event& e)
    {
        if (e.type == SDL_KEYDOWN) {
            keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_DOWN;
            just_became_down_scancodes.push_back(e.key.keysym.scancode);
        }
        if (e.type == SDL_KEYUP) {
            keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_UP;
            just_became_up_scancodes.push_back(e.key.keysym.scancode);
        }
        if (e.type == SDL_MOUSEMOTION) {
            mouse_position.x = e.motion.x;
            mouse_position.y = e.motion.y;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_DOWN;
            just_became_down_buttons.push_back(e.button.button);
        }
        if (e.type == SDL_MOUSEBUTTONUP) {
            mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_UP;
            just_became_up_buttonss.push_back(e.button.button);
        }
        if (e.type == SDL_MOUSEWHEEL) {
            mouse_scroll_this_frame = e.wheel.preciseY;
        }
    }
    
    static void LateUpdate()
    {
        for (SDL_Scancode code : just_became_down_scancodes)
            keyboard_states[code] = INPUT_STATE_DOWN;
        just_became_down_scancodes.clear();

        for (SDL_Scancode code : just_became_up_scancodes)
            keyboard_states[code] = INPUT_STATE_UP;
        just_became_up_scancodes.clear();

        for (int button : just_became_down_buttons)
            mouse_button_states[button] = INPUT_STATE_DOWN;
        just_became_down_buttons.clear();

        for (int button : just_became_up_buttonss)
            mouse_button_states[button] = INPUT_STATE_UP;
        just_became_up_buttonss.clear();

        mouse_scroll_this_frame = 0.0f;  // reset each frame
    }
    
    static bool GetKey(std::string keycode)
    {
        SDL_Scancode scancode = GetScancodeFromString(keycode);
        if (scancode == SDL_SCANCODE_UNKNOWN) return false;
        auto it = keyboard_states.find(scancode);
        if (it == keyboard_states.end()) return false;
        return it->second == INPUT_STATE_DOWN || it->second == INPUT_STATE_JUST_BECAME_DOWN;
    }

    static bool GetKeyDown(std::string keycode)
    {
        SDL_Scancode scancode = GetScancodeFromString(keycode);
        if (scancode == SDL_SCANCODE_UNKNOWN) return false;
        auto it = keyboard_states.find(scancode);
        if (it == keyboard_states.end()) return false;
        return it->second == INPUT_STATE_JUST_BECAME_DOWN;
    }

    static bool GetKeyUp(std::string keycode)
    {
        SDL_Scancode scancode = GetScancodeFromString(keycode);
        if (scancode == SDL_SCANCODE_UNKNOWN) return false;
        auto it = keyboard_states.find(scancode);
        if (it == keyboard_states.end()) return false;
        return it->second == INPUT_STATE_JUST_BECAME_UP;
    }
    
    static glm::vec2 GetMousePosition() {
        return mouse_position;
    }

    static bool GetMouseButton(int button) {
        if (mouse_button_states.count(button) == 0) return false;
        return mouse_button_states[button] == INPUT_STATE_DOWN ||
               mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
    }

    static bool GetMouseButtonDown(int button) {
        if (mouse_button_states.count(button) == 0) return false;
        return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
    }

    static bool GetMouseButtonUp(int button) {
        if (mouse_button_states.count(button) == 0) return false;
        return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_UP;
    }

    static float GetMouseScrollDelta() {
        return mouse_scroll_this_frame;
    }

    static void HideCursor() {
        SDL_ShowCursor(SDL_DISABLE);
    }

    static void ShowCursor() {
        SDL_ShowCursor(SDL_ENABLE);
    }


};

#endif

