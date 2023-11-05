//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_TYPES_H
#define NEBULAENGINE_TYPES_H

#include <memory>

#include "Timestep.h"

namespace nebula {
    
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Reference = std::shared_ptr<T>;

    template <typename T>
    using View = T*;
    
}

#endif //NEBULAENGINE_TYPES_H
