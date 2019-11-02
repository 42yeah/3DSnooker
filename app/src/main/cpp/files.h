//
// Created by 周昊 on 2019/11/2.
//

#ifndef INC_3DSNOOKER_FILES_H
#define INC_3DSNOOKER_FILES_H

#include <android/asset_manager.h>
#include <string>
#include <fstream>
#include <sstream>


class Resources {
public:
    // === CONSTRUCTORS === //
    Resources(AAssetManager *manager) : assetManager(manager) {

    }

    // === METHODS === //
    std::stringstream readFileAsStringStream(std::string path);

    std::string readFileAsString(std::string path);

    // === VARIABLES === //
    AAssetManager *assetManager;
};

#endif //INC_3DSNOOKER_FILES_H
