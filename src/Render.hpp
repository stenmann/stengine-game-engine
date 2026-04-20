#ifndef RENDER_H
#define RENDER_H


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
#include "ImageDB.hpp"
#include "TextDB.hpp"
#include "assets.hpp"

using namespace std;

class Render {
private:
    gameinfo stats;
    engineUtils eu;
    
public:
    const string gameConfig = "resources/game.config";
    const string renderConfig = "resources/rendering.config";
    int x_resolution = 640;
    int y_resolution = 360;
    string title = "";
    int clear_color_r = 255;
    int clear_color_g = 255;
    int clear_color_b = 255;
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    
    Render () {
        
        if (!eu.checkForDirectory(gameConfig)) {
            return;
        }
        
        rapidjson::Document doc;
        eu.ReadJsonFile(gameConfig, doc);
        
        if (doc.HasMember("game_title")) {
            title = doc["game_title"].GetString();
        }
        
        if(eu.checkForDirectory(renderConfig))
        {
            rapidjson::Document renderDoc;
            eu.ReadJsonFile(renderConfig, renderDoc);
            
            if(renderDoc.HasMember("x_resolution"))
                x_resolution = renderDoc["x_resolution"].GetInt();
            if(renderDoc.HasMember("y_resolution"))   
                y_resolution = renderDoc["y_resolution"].GetInt();
            if(renderDoc.HasMember("clear_color_r"))
                clear_color_r = renderDoc["clear_color_r"].GetInt();
            if(renderDoc.HasMember("clear_color_g"))
                clear_color_g = renderDoc["clear_color_g"].GetInt();
            if(renderDoc.HasMember("clear_color_b"))
                clear_color_b = renderDoc["clear_color_b"].GetInt();
        }
        window = Helper::SDL_CreateWindow(title.c_str(),0, 0, x_resolution, y_resolution, SDL_WINDOW_SHOWN);
        
        renderer = Helper::SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        imagesDB::renderer = renderer;
        
    }
        
};


#endif
