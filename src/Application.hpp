#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <optional>
#include "glm/glm.hpp"
#include <unordered_set>
#include <map>
#include <thread>
#include <chrono>
#include "Helper.h"

using namespace std;

class Application
{
    
public:
    static void Quit()
    {
        exit(0);
    }
    
    static void Sleep(int dur_ms)
    {
        this_thread::sleep_for(chrono::milliseconds(dur_ms));
    }
    
    static int GetFrame()
    {
        return Helper::GetFrameNumber();
    }
    
    static void OpenURL(string url) {
            #ifdef _WIN32
                system(("start " + url).c_str());
            #elif __APPLE__
                system(("open " + url).c_str());
            #else
                system(("xdg-open " + url).c_str());
            #endif
        }
    
    
};

#endif
