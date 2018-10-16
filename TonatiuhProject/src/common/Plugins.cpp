//
// Created by Javier Garcia Blas on 25/9/18.
//

#include "Plugins.h"


/*std::vector<std::string> Plugins::GetFactoriesAvailable (const std::string & pluginsPath, const std::string & factory) {
    namespace fs = std::experimental::filesystem;

    std::vector<std::string> plugins;
    fs::path p {pluginsPath.c_str()};

    for(auto & p: fs::recursive_directory_iterator(p)) {
        std::string filename = p.path().filename().string();

        if (filename.find(factory))
            plugins.push_back(filename);
    }

    if (!plugins.size()) {
        throw std::runtime_error("Factory " + factory + " not found");
    }

    return plugins;
}


std::vector<std::string> Plugins::GetMaterialFactoriesAvailable (const std::string & pluginsPath) {
    return GetFactoriesAvailable(pluginsPath, "Material");
}

std::vector<std::string> Plugins::GetShapeFactoriesAvailable (const std::string & pluginsPath) {
    return GetFactoriesAvailable(pluginsPath, "Shape");
}

std::vector<std::string> Plugins::GetTrackerFactoriesAvailable (const std::string & pluginsPath) {
    return GetFactoriesAvailable(pluginsPath, "Tracker");
}

std::vector<std::string> Plugins::GetTransmissivityFactoriesAvailable (const std::string & pluginsPath) {
    return GetFactoriesAvailable(pluginsPath, "Transmissivity");
}*/
