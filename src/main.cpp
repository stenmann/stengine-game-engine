//
//  main.cpp
//  game_engine
//
//  Created by Nickolas Stenman on 1/22/26.
//
#ifdef _WIN32
    #define SDL_MAIN_HANDLED
#endif

#include "main.hpp"
#include <iostream>
#include <algorithm>
#include "rapidjson/document.h"
#include "engineUtils.hpp"
#include "glm/glm.hpp"
#include "ComponentDB.hpp"
#include "Actor.hpp"
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "Helper.h"
#include "AudioHelper.h"
#include "Render.hpp"
#include "ImageDB.hpp"
#include "TextDB.hpp"
#include "AudioDB.hpp"
#include "Input.hpp"
#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "physics.hpp"

using namespace std;

gameinfo* assetsPtr = nullptr;
musicDB* musicPtr = nullptr;

void renderGameplay(Render& renderState, imagesDB& imageDB, TextDB& textDB, gameinfo& assets);

int main(int argc, char* argv[]) {
    //dummy
    
    physics phys;
    Render renderState;
    
    assetsPtr = new gameinfo();
    
    gameinfo& assets = *assetsPtr;
    bool gameOn = true;
    bool quitRequested = false;
    bool introComplete = false;
    int curImg = 0;
    int curText = 0;
    
    imagesDB imageDB;
    imageDB.loadIntroImages(renderState.renderer);
    TextDB introText(renderState.renderer, renderState.y_resolution);
    bool endingHandled = false;
    Input::Init();

    

    
    if(imageDB.images.size() == 0)
    {
        introComplete = true;
    }
    
    while(gameOn && !quitRequested)
    {
        SDL_SetRenderDrawColor(renderState.renderer, renderState.clear_color_r,
                               renderState.clear_color_g, renderState.clear_color_b, 255);
        SDL_RenderClear(renderState.renderer);
        
        SDL_Event event;
        while(Helper::SDL_PollEvent(&event)) {
            if(!introComplete){
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE ||
                        event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                        curImg++;
                        curText++;
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        curImg++;
                        curText++;
                    }
                }
                else if(event.type == SDL_QUIT) {
                    quitRequested = true;
                }
            }
            if(introComplete)
            {
                Input::ProcessEvent(event);
                if(event.type == SDL_QUIT) {
                    quitRequested = true;
                }
            }
        }
        
        if(!introComplete)
        {
            bool imagesExhausted = (curImg >= imageDB.images.size());
            bool textsExhausted = (curText >= introText.textContents.size());
            
            if (imagesExhausted && textsExhausted) {
                introComplete = true;
                if (quitRequested)
                {
                    Helper::SDL_RenderPresent(renderState.renderer);
                }
                break;
            }
            
            if(imageDB.images.size() > 0) {
                int imgIndex = min(curImg, (int)imageDB.images.size() - 1);
                Helper::SDL_RenderCopy(renderState.renderer, imageDB.images[imgIndex], nullptr, nullptr);
            }
            
            if(introText.textContents.size() > 0) {
            }
            
            Helper::SDL_RenderPresent(renderState.renderer);
            
            if(quitRequested) {
                exit(0);
            }
        }
        if(introComplete)
        {
            assets.processPendingSceneLoad();
            assets.callOnStart();
            assets.callOnUpdate();
            assets.callOnLateUpdate();
            assets.processEventBus();
            physics::Step();
            assets.processPendingAdditions();
            assets.processPendingRemovals();
            assets.processPendingActors();
            assets.processPendingDestroys();

            SDL_SetRenderDrawColor(renderState.renderer, renderState.clear_color_r,
                                   renderState.clear_color_g, renderState.clear_color_b, 255);
            SDL_RenderClear(renderState.renderer);

            imagesDB::RenderSceneImages(renderState.renderer,imagesDB::camX, imagesDB::camY,
                                         imagesDB::zoom, renderState.x_resolution,
                                         renderState.y_resolution);
            imagesDB::RenderUIImages(renderState.renderer);
            TextDB::RenderLuaText(renderState.renderer);
            imagesDB::RenderPixels(renderState.renderer);

            Helper::SDL_RenderPresent(renderState.renderer);
            Input::LateUpdate();
        }
        
    }
}
        


