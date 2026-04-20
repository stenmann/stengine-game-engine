#ifndef ENGINEUTILS_H
#define ENGINEUTILS_H

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <filesystem>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace std;

class engineUtils {
public:
    static void ReadJsonFile(const std::string& path, rapidjson::Document & out_document)
    {
        FILE* file_pointer = nullptr;
    #ifdef _WIN32
        fopen_s(&file_pointer, path.c_str(), "rb");
    #else
        file_pointer = fopen(path.c_str(), "rb");
    #endif
        char buffer[65536];
        rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
        out_document.ParseStream(stream);
        std::fclose(file_pointer);

        if (out_document.HasParseError()) {
            rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
            std::cout << "error parsing json at [" << path << "]" << std::endl;
            exit(0);
        }
    }
    
   bool checkForDirectory(const string& direct)
    {
       filesystem::path p(direct);
       
       if(!filesystem::exists(p) && direct == "resources")
           {
               cout << "error: resources/ missing";
               exit(0);
           }
       if(!filesystem::exists(p) && direct == "resources/game.config")
           {
               cout << "error: " << direct << " missing";
               exit(0);
           }
       if(!filesystem::exists(p))
           return false;
       else
           return true;
       
       
   }
};
#endif
