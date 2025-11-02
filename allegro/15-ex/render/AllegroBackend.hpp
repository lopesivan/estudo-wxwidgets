// render/AllegroBackend.hpp
#pragma once

#include <memory>
#include <vector>

// Precisamos dos headers do Allegro aqui porque o deleter
// chama as funções al_destroy_*
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "core/IRenderBackend.hpp"

struct ALLEGRO_LOCKED_REGION;  // fwd ok

namespace core
{
struct RenderModel;
}

namespace render
{

// Deleter único para vários tipos Allegro (extensível)
struct AllegroDeleter
{
    void operator()(ALLEGRO_BITMAP* p) const noexcept
    {
        if(p)
            al_destroy_bitmap(p);
    }
    void operator()(ALLEGRO_FONT* p) const noexcept
    {
        if(p)
            al_destroy_font(p);
    }
    // Extras, caso use em outros lugares do projeto
    void operator()(ALLEGRO_DISPLAY* p) const noexcept
    {
        if(p)
            al_destroy_display(p);
    }
    void operator()(ALLEGRO_EVENT_QUEUE* p) const noexcept
    {
        if(p)
            al_destroy_event_queue(p);
    }
    void operator()(ALLEGRO_TIMER* p) const noexcept
    {
        if(p)
            al_destroy_timer(p);
    }
};

// Aliases RAII
using BitmapUPtr = std::unique_ptr<ALLEGRO_BITMAP, AllegroDeleter>;
using FontUPtr   = std::unique_ptr<ALLEGRO_FONT, AllegroDeleter>;

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
                     int&                        width,
                     int&                        height) override;

  private:
    BitmapUPtr backbuffer_{nullptr};  // destrói automaticamente
    FontUPtr   font_{nullptr};
    int        w_ = 0, h_ = 0;
    bool       initialized_ = false;

    void ensureBackbuffer(int w, int h);
    void clearBlack();
};

}  // namespace render
