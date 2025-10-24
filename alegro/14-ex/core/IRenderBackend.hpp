// core/IRenderBackend.hpp
#pragma once
#include <vector>

namespace core {

struct RenderModel;

// Interface Bridge — Implementor
class IRenderBackend {
public:
    virtual ~IRenderBackend() = default;

    // Pode ignorar native_handle se o backend for offscreen
    virtual bool init(void* native_handle, int width, int height) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void render(const RenderModel& model) = 0;
    virtual void shutdown() = 0;

    // Entrega os pixels como RGB contíguo (width*height*3)
    // Retorna true se preencheu 'outRGB' (w*h*3)
    virtual bool getFrameRGB(std::vector<unsigned char>& outRGB,
                             int& width,
                             int& height) = 0;
};

} // namespace core

