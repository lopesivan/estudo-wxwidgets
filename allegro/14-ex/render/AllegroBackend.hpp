// render/AllegroBackend.hpp
#pragma once
#include <vector>

#include "core/IRenderBackend.hpp"

struct ALLEGRO_BITMAP;
struct ALLEGRO_FONT;

namespace core
{
struct RenderModel;
}

namespace render
{

class AllegroBackend final : public core::IRenderBackend
{
public:
    AllegroBackend();
    ~AllegroBackend() override;

    bool init(void* native_handle, int width, int height) override;
    void resize(int width, int height) override;
    void render(const core::RenderModel& model) override;
    void shutdown() override;

    bool getFrameRGB(std::vector<unsigned char>& outRGB,
                     int& width,
                     int& height) override;

private:
    ALLEGRO_BITMAP* backbuffer_ = nullptr;
    ALLEGRO_FONT* font_ = nullptr;
    int w_ = 0, h_ = 0;
    bool initialized_ = false;

    void ensureBackbuffer(int w, int h);
    void clearBlack();
};

}  // namespace render
