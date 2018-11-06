//
// Created by Javier Garcia Blas on 25/9/18.
//

#ifndef TONATIUH_PLUGINS_H
#define TONATIUH_PLUGINS_H

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <vector>
#include <experimental/filesystem>


class Plugins {

public:
  std::vector<std::string> GetFactoriesAvailable(const std::string &pluginsPath, const std::string &factory);

  std::vector<std::string> GetMaterialFactoriesAvailable(const std::string &pluginsPath);

  std::vector<std::string> GetShapeFactoriesAvailable(const std::string &pluginsPath);

  std::vector<std::string> GetTrackerFactoriesAvailable(const std::string &pluginsPath);

  std::vector<std::string> GetTransmissivityFactoriesAvailable(const std::string &pluginsPath);
};


#endif //TONATIUH_PLUGINS_H
