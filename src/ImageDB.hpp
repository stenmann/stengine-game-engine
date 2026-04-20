#ifndef IMAGEDB_H
#define IMAGEDB_H

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <queue>
#include <algorithm>

#include "engineUtils.hpp"
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "Helper.h"
#include "glm/glm.hpp"

using namespace std;

class imagesDB {
private:
    engineUtils eu;

public:
   static inline float camX = 0.0f;
   static inline float camY = 0.0f;
   static inline float zoom = 1.0f;
   static inline SDL_Renderer* renderer = nullptr;

    struct ImageDrawRequest {
        string image_name;
        float x = 0;
        float y = 0;
        int rotation_degrees = 0;
        float scale_x = 1.0f;
        float scale_y = 1.0f;
        float pivot_x = 0.5f;
        float pivot_y = 0.5f;
        int r = 255;
        int g = 255;
        int b = 255;
        int a = 255;
        int sorting_order = 0;
    };

    struct PixelDrawRequest {
        int x, y, r, g, b, a;
    };

    const string gameConfig = "resources/game.config";
    vector<string> imageNames;
    vector<SDL_Texture*> images;

    static inline unordered_map<string, SDL_Texture*> texture_cache;
    static inline vector<ImageDrawRequest> scene_draw_queue;
    //dummy
    static inline vector<ImageDrawRequest> UI_queue;
    static inline vector<PixelDrawRequest> pixel_queue;


    static SDL_Texture* GetTexture(const string& name, SDL_Renderer* renderer)
    {
        auto it = texture_cache.find(name);
        if (it != texture_cache.end()) return it->second;

        string imagePath = "resources/images/" + name + ".png";
        if (!filesystem::exists(imagePath)) {
            cout << "error: missing image " << name;
            exit(0);
        }
        SDL_Texture* tex = IMG_LoadTexture(renderer, imagePath.c_str());
        texture_cache[name] = tex;
        return tex;
    }

    // Lua-facing functions
    static void Draw(string image_name, float x, float y)
    {
        ImageDrawRequest req;
        req.image_name = image_name;
        req.x = x;
        req.y = y;
        scene_draw_queue.push_back(req);
    }

    static void DrawEx(string image_name, float x, float y,
                       float rotation_degrees, float scale_x, float scale_y,
                       float pivot_x, float pivot_y,
                       float r, float g, float b, float a, float sorting_order)
    {
        ImageDrawRequest req;
        req.image_name = image_name;
        req.x = x;
        req.y = y;
        req.rotation_degrees = (int)rotation_degrees;
        req.scale_x = scale_x;
        req.scale_y = scale_y;
        req.pivot_x = pivot_x;
        req.pivot_y = pivot_y;
        req.r = (int)r;
        req.g = (int)g;
        req.b = (int)b;
        req.a = (int)a;
        req.sorting_order = (int)sorting_order;
        scene_draw_queue.push_back(req);
    }

    static void DrawUI(string image_name, float x, float y)
    {
        ImageDrawRequest req;
        req.image_name = image_name;
        req.x = (int)x;
        req.y = (int)y;
        UI_queue.push_back(req);
    }

    static void DrawUIEx(string image_name, float x, float y,
                         float r, float g, float b, float a, float sorting_order)
    {
        ImageDrawRequest req;
        req.image_name = image_name;
        req.x = (int)x;
        req.y = (int)y;
        req.r = (int)r;
        req.g = (int)g;
        req.b = (int)b;
        req.a = (int)a;
        req.sorting_order = (int)sorting_order;
        UI_queue.push_back(req);
    }

    static void DrawPixel(float x, float y, float r, float g, float b, float a)
    {
        PixelDrawRequest req;
        req.x = (int)x;
        req.y = (int)y;
        req.r = (int)r;
        req.g = (int)g;
        req.b = (int)b;
        req.a = (int)a;
        pixel_queue.push_back(req);
    }

    static void RenderSceneImages(SDL_Renderer* renderer, float cam_x, float cam_y,
                                   float cam_zoom, int screen_w, int screen_h)
    {
        stable_sort(scene_draw_queue.begin(), scene_draw_queue.end(),
            [](const ImageDrawRequest& a, const ImageDrawRequest& b) {
                return a.sorting_order < b.sorting_order;
            });

        SDL_RenderSetScale(renderer, cam_zoom, cam_zoom);

        for (auto& request : scene_draw_queue)
        {
            const int pixels_per_meter = 100;
            glm::vec2 final_pos = glm::vec2(request.x, request.y) - glm::vec2(cam_x, cam_y);

            SDL_Texture* tex = GetTexture(request.image_name, renderer);
            float w, h;
            Helper::SDL_QueryTexture(tex, &w, &h);

            int flip_mode = SDL_FLIP_NONE;
            if (request.scale_x < 0) flip_mode |= SDL_FLIP_HORIZONTAL;
            if (request.scale_y < 0) flip_mode |= SDL_FLIP_VERTICAL;

            float x_scale = glm::abs(request.scale_x);
            float y_scale = glm::abs(request.scale_y);

            SDL_FRect tex_rect;
            tex_rect.w = w * x_scale;
            tex_rect.h = h * y_scale;

            SDL_FPoint pivot_point = {
                static_cast<float>(request.pivot_x * tex_rect.w),
                static_cast<float>(request.pivot_y * tex_rect.h)
            };

            tex_rect.x = (final_pos.x * pixels_per_meter + screen_w * 0.5f * (1.0f / cam_zoom) - pivot_point.x);
            tex_rect.y = (final_pos.y * pixels_per_meter + screen_h * 0.5f * (1.0f / cam_zoom) - pivot_point.y);

            SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
            SDL_SetTextureAlphaMod(tex, request.a);

            Helper::SDL_RenderCopyEx(0, "", renderer, tex, NULL, &tex_rect,
                                      request.rotation_degrees, &pivot_point,
                                      static_cast<SDL_RendererFlip>(flip_mode));

            SDL_RenderSetScale(renderer, cam_zoom, cam_zoom);

            SDL_SetTextureColorMod(tex, 255, 255, 255);
            SDL_SetTextureAlphaMod(tex, 255);
        }

        SDL_RenderSetScale(renderer, 1, 1);
        scene_draw_queue.clear();
    }

    static void RenderUIImages(SDL_Renderer* renderer)
    {
        stable_sort(UI_queue.begin(), UI_queue.end(),
            [](const ImageDrawRequest& a, const ImageDrawRequest& b) {
                return a.sorting_order < b.sorting_order;
            });

        for (auto& request : UI_queue)
        {
            SDL_Texture* tex = GetTexture(request.image_name, renderer);
            float w, h;
            Helper::SDL_QueryTexture(tex, &w, &h);

            SDL_FRect tex_rect;
            tex_rect.x = (float)request.x;
            tex_rect.y = (float)request.y;
            tex_rect.w = (float)w;
            tex_rect.h = (float)h;

            SDL_FPoint pivot_point = {0.0f, 0.0f};

            SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
            SDL_SetTextureAlphaMod(tex, request.a);

            Helper::SDL_RenderCopyEx(0, "", renderer, tex, NULL, &tex_rect,
                                      0, &pivot_point, SDL_FLIP_NONE);

            SDL_SetTextureColorMod(tex, 255, 255, 255);
            SDL_SetTextureAlphaMod(tex, 255);
        }
        UI_queue.clear();
    }

    static void RenderPixels(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        for (auto& req : pixel_queue)
        {
            SDL_SetRenderDrawColor(renderer, req.r, req.g, req.b, req.a);
            SDL_RenderDrawPoint(renderer, req.x, req.y);
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        pixel_queue.clear();
    }

    void loadIntroImages(SDL_Renderer* renderer) {
        if (!eu.checkForDirectory(gameConfig)) return;

        rapidjson::Document doc;
        eu.ReadJsonFile(gameConfig, doc);

        if (!doc.HasMember("intro_image")) return;

        const auto& arrayAct = doc["intro_image"];
        for (int i = 0; i < arrayAct.Size(); i++) {
            string image = arrayAct[i].GetString();
            string imagePath = "resources/images/" + image + ".png";
            if (!filesystem::exists(imagePath)) {
                cout << "error: missing image " << image;
                exit(0);
            }
            SDL_Texture* texture = IMG_LoadTexture(renderer, imagePath.c_str());
            imageNames.push_back(image);
            images.push_back(texture);
        }
    }
    
    static void SetPosition(float x, float y)
    {
        camX = x;
        camY = y;
    }
    static float GetPositionX()
    {
        return camX;
    }
    
    static float GetPositionY()
    {
        return camY;
    }
    
    static void SetZoom(float zoom_factor)
    {
        zoom = zoom_factor;
    }
    
    static float GetZoom()
    {
        return zoom;
    }
    
    
    static void CreateDefaultParticleTextureWithName(const string& name)
    {
        if (texture_cache.find(name) != texture_cache.end())
            return;

        // RGBA8888 instead of RGB888 — alpha channel required for alpha mod to work
        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 8, 8, 32, SDL_PIXELFORMAT_RGBA8888);

        Uint32 whiteColor = SDL_MapRGBA(surface->format, 255, 255, 255, 255);
        SDL_FillRect(surface, NULL, whiteColor);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        // Enable alpha blending on the texture
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        
        texture_cache[name] = texture;
    }
    

    ~imagesDB() {
        for (SDL_Texture* tex : images) {
            if (tex) SDL_DestroyTexture(tex);
        }
    }
};

#endif


