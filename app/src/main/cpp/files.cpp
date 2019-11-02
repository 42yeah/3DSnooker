//
// Created by 周昊 on 2019/11/2.
//

#include "files.h"
#include <iostream>
#include <string>
#include <string>
#include <android/asset_manager.h>


std::string Resources::readFileAsString(std::string path) {
    std::stringstream ss = readFileAsStringStream(path);
    std::string raw = ss.str();
    return raw;
}

std::stringstream Resources::readFileAsStringStream(std::string path) {
    AAsset *asset = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_STREAMING);
    std::stringstream ss;
    while (true) {
        char buf[1024];
        int len = AAsset_read(asset, buf, sizeof(buf));
        if (len <= 0) {
            break;
        }
        ss.write(buf, len);
    }
    return ss;
}
