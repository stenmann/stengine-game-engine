#ifndef TEXTDB_HPP 
#define TEXTDB_HPP

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
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "SDL2_image/SDL_image.h"

using namespace std;

class TextDB {
    
private:
    engineUtils eu;
    TTF_Font* loadedFont;
    SDL_Renderer* renderer;
    
    struct LuaTextDrawRequest {
        std::string content;
        int x, y;
        std::string font_name;
        int font_size;
        int r, g, b, a;
    };

    static inline std::queue<LuaTextDrawRequest> lua_text_queue;
    
public:
    const string gameConfig = "resources/game.config";
    static inline std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> font_cache;
    
    struct textInfo {
        string content = "";
        int font_size = 16;
        SDL_Color color = {255, 255, 255, 255};
        int posx = 25;
        int posy = 0;
    };
    
    vector<textInfo> textContents;
    
    TextDB(SDL_Renderer* rend, int windowHeight) : renderer(rend), loadedFont(nullptr) {
        TTF_Init();
            
            rapidjson::Document doc;
            eu.ReadJsonFile(gameConfig, doc);
            
            // 1. Validation: If we have text but no font, that's an error.
            if (doc.HasMember("intro_text") && !doc.HasMember("font")) {
                cout << "error: text render failed. No font configured";
                exit(0);
            }
            
            if(doc.HasMember("font")) {
                string fontName = doc["font"].GetString();
                string fontPath = "resources/fonts/" + fontName + ".ttf";
                
                if(!eu.checkForDirectory(fontPath)) {
                    cout << "error: font " << fontName << " missing";
                    exit(0);
                }
                
                loadedFont = TTF_OpenFont(fontPath.c_str(), 16);
                if (!loadedFont) {
                    cout << "error: Failed to open font";
                    exit(0);
                }
            }
            
            if(doc.HasMember("intro_text")) {
                const auto& arrayAct = doc["intro_text"];
                
                for(int i = 0; i < arrayAct.Size(); i++) {
                    textInfo text;
                    text.content = arrayAct[i].GetString();
                    text.posy = windowHeight - 50;
                    textContents.push_back(text);
                }
            }
    }
    
    ~TextDB() {
        if (loadedFont) {
            TTF_CloseFont(loadedFont);
        }
        TTF_Quit();
    }

    bool checkIntroText() {
        rapidjson::Document doc;
        eu.ReadJsonFile(gameConfig, doc);
        if(doc.HasMember("intro_image")) {
            return true;
        }
        else {
            return false;
        }
    }
    
    static void Draw(std::string content, float x, float y,
                     std::string font_name, float font_size,
                     float r, float g, float b, float a)
    {
        LuaTextDrawRequest req;
        req.content = content;
        req.x = (int)x;
        req.y = (int)y;
        req.font_name = font_name;
        req.font_size = (int)font_size;
        req.r = (int)r;
        req.g = (int)g;
        req.b = (int)b;
        req.a = (int)a;
        lua_text_queue.push(req);
    }
    
    static void RenderLuaText(SDL_Renderer* renderer)
    {
        while (!lua_text_queue.empty()) {
            LuaTextDrawRequest& req = lua_text_queue.front();
            
            // get or load font
            if (font_cache[req.font_name].count(req.font_size) == 0) {
                std::string fontPath = "resources/fonts/" + req.font_name + ".ttf";
                TTF_Font* font = TTF_OpenFont(fontPath.c_str(), req.font_size);
                if (!font) {
                    lua_text_queue.pop();
                    continue;
                }
                font_cache[req.font_name][req.font_size] = font;
            }
            
            TTF_Font* font = font_cache[req.font_name][req.font_size];
            SDL_Color color = {(Uint8)req.r, (Uint8)req.g, (Uint8)req.b, (Uint8)req.a};
            
            SDL_Surface* surface = TTF_RenderText_Solid(font, req.content.c_str(), color);
            if (!surface) { lua_text_queue.pop(); continue; }
            
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                SDL_FRect destRect = {
                    (float)req.x, (float)req.y,
                    (float)surface->w, (float)surface->h
                };
                Helper::SDL_RenderCopy(renderer, texture, nullptr, &destRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
            lua_text_queue.pop();
        }
    }
};

#endif



