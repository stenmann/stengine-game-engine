#ifndef AUDIODB_HPP
#define AUDIODB_HPP

#include <string>
#include <vector>
#include <iostream>
#include "rapidjson/document.h"
#include "engineUtils.hpp"
#include "AudioHelper.h"

using namespace std;

class musicDB {
    
private:
    engineUtils eu;
    
public:
    const string gameConfig = "resources/game.config";
    Mix_Chunk* bgmChunk = nullptr;
    Mix_Chunk* musicChunk = nullptr;
    
    bool hasBgm = false;
    bool hasMusic = false;
    
    musicDB() {
        AudioHelper::Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        AudioHelper::Mix_AllocateChannels(50);
        rapidjson::Document doc;
        eu.ReadJsonFile(gameConfig, doc);
        
        if(doc.HasMember("intro_bgm")) {
            hasBgm = true;
            bgmChunk = LoadAudio(doc["intro_bgm"].GetString());
        }
        
        if(doc.HasMember("gameplay_audio")) {
            hasMusic = true;
            musicChunk = LoadAudio(doc["gameplay_audio"].GetString());
        }

        
    }
    
    static inline unordered_map<string, Mix_Chunk*> audio_cache;
        
        static Mix_Chunk* LoadAudio(const string& clipName)
        {
            if (audio_cache.count(clipName) > 0)
                return audio_cache[clipName];
            
            string wavPath = "resources/audio/" + clipName + ".wav";
            string oggPath = "resources/audio/" + clipName + ".ogg";
            
            string finalPath = "";
            if (filesystem::exists(wavPath)) finalPath = wavPath;
            else if (filesystem::exists(oggPath)) finalPath = oggPath;
            else {
                cout << "error: failed to play audio clip " << clipName;
                exit(0);
            }
            
            Mix_Chunk* chunk = AudioHelper::Mix_LoadWAV(finalPath.c_str());
            audio_cache[clipName] = chunk;
            return chunk;
        }
        
        static void Play(int channel, string clip_name, bool does_loop)
        {
            Mix_Chunk* chunk = LoadAudio(clip_name);
            int loops = does_loop ? -1 : 0;
            AudioHelper::Mix_PlayChannel(channel, chunk, loops);
        }
        
        static void Halt(int channel)
        {
            AudioHelper::Mix_HaltChannel(channel);
        }
        
        static void SetVolume(int channel, float volume)
        {
            AudioHelper::Mix_Volume(channel, (int)volume);
        }
};

#endif
