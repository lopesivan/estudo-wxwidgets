// core/BackendFactory.cpp
#include "core/BackendFactory.hpp"
#include "render/AllegroBackend.hpp"
#include <memory>

namespace core
{
std::unique_ptr<IRenderBackend> makeBackend(RenderBackendType type)
{
    switch(type)
    {
    case RenderBackendType::Allegro:
    default:
        return std::make_unique<render::AllegroBackend>();
    }
}
}  // namespace core
