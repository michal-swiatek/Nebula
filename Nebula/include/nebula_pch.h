//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_NEBULA_PCH_H
#define NEBULAENGINE_NEBULA_PCH_H

//  std headers

#include <fstream>
#include <sstream>
#include <iostream>

#include <format>
#include <string>

#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//  3rd-party headers
#include <spdlog/spdlog.h>

//  Platform specific headers
#if defined(_WIN32) || defined(WIN32)
    #include <Windows.h>
#endif

#endif //NEBULAENGINE_NEBULA_PCH_H
