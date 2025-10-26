// core/BackendFactory.hpp
#pragma once
#include <memory>
#include "IRenderBackend.hpp"

namespace core
{

enum class RenderBackendType
{
    Allegro
};

std::unique_ptr<IRenderBackend> makeBackend(RenderBackendType type);

}  // namespace core
