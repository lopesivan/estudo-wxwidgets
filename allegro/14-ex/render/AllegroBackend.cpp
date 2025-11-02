// render/AllegroBackend.cpp
#include "render/AllegroBackend.hpp"

#include "core/RenderModel.hpp"

// Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

// C++
#include <cstdio>  // <-- adicione esta linha
#include <cstring>

namespace render
{

AllegroBackend::AllegroBackend() = default;

AllegroBackend::~AllegroBackend()
{
    shutdown();
}

bool AllegroBackend::init(void* /*native_handle*/,
                          int width,
                          int height)
{
    if (!al_is_system_installed())
    {
        if (!al_init())
            return false;
    }
    al_install_mouse();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    w_ = width;
    h_ = height;
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    backbuffer_ = al_create_bitmap(w_, h_);
    if (!backbuffer_)
        return false;

    font_ =
        al_load_font("SauceCodeProNerdFontMono-Regular.ttf", 20, 0);
    if (!font_)
        font_ = al_create_builtin_font();

    clearBlack();
    initialized_ = true;
    return true;
}

void AllegroBackend::resize(int width, int height)
{
    if (!initialized_)
        return;
    ensureBackbuffer(width, height);
    clearBlack();
}

void AllegroBackend::render(const core::RenderModel& model)
{
    if (!initialized_ || !backbuffer_)
        return;

    al_set_target_bitmap(backbuffer_);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // desenha pontos / textos conforme mouse_pos.c
    char buffer[128];
    for (int i = 0; i < model.points_stored; ++i)
    {
        float px, py;
        core::converter_km_para_pixel(model.pos_km_x[i],
                                      model.pos_km_y[i],
                                      model.width,
                                      model.height,
                                      &px,
                                      &py);
        al_draw_filled_circle(px, py, 10, al_map_rgb(255, 0, 0));
        std::snprintf(buffer,
                      sizeof(buffer),
                      "Posição %d: X=%.2f km, Y=%.2f km",
                      i + 1,
                      model.pos_km_x[i],
                      model.pos_km_y[i]);
        if (font_)
            al_draw_text(font_,
                         al_map_rgb(255, 255, 255),
                         20,
                         20 + i * 30,
                         0,
                         buffer);
    }

    if (model.points_stored == 2)
    {
        float x1, y1, x2, y2;
        core::converter_km_para_pixel(model.pos_km_x[0],
                                      model.pos_km_y[0],
                                      model.width,
                                      model.height,
                                      &x1,
                                      &y1);
        core::converter_km_para_pixel(model.pos_km_x[1],
                                      model.pos_km_y[1],
                                      model.width,
                                      model.height,
                                      &x2,
                                      &y2);
        al_draw_line(x1, y1, x2, y2, al_map_rgb(0, 255, 0), 2.0f);

        std::snprintf(buffer,
                      sizeof(buffer),
                      "Delta: dX=%.2f km, dY=%.2f km",
                      model.pos_km_x[1] - model.pos_km_x[0],
                      model.pos_km_y[1] - model.pos_km_y[0]);
        if (font_)
            al_draw_text(font_,
                         al_map_rgb(255, 255, 255),
                         20,
                         80,
                         0,
                         buffer);

        std::snprintf(
            buffer,
            sizeof(buffer),
            "Módulo: u=%.2f km",
            core::calcular_modulo_vetor(model.pos_km_x[0],
                                        model.pos_km_y[0],
                                        model.pos_km_x[1],
                                        model.pos_km_y[1]));
        if (font_)
            al_draw_text(font_,
                         al_map_rgb(255, 255, 255),
                         20,
                         110,
                         0,
                         buffer);
    }

    if (font_)
    {
        al_draw_text(font_,
                     al_map_rgb(200, 200, 200),
                     20,
                     model.height - 40,
                     0,
                     "[ESPAÇO] marca ponto | [ESC] sai");
    }
}

bool AllegroBackend::getFrameRGB(std::vector<unsigned char>& outRGB,
                                 int& width,
                                 int& height)
{
    if (!initialized_ || !backbuffer_)
        return false;

    width = w_;
    height = h_;
    outRGB.resize(w_ * h_ * 3);

    // Leitura neutra a formato (sempre correta): al_get_pixel +
    // al_unmap_rgba (Suficiente para 1280x720. Se quiser mais
    // desempenho, podemos trocar
    //  por lock + conversão conforme al_get_bitmap_format().)
    ALLEGRO_LOCKED_REGION* lr =
        al_lock_bitmap(backbuffer_, 0, ALLEGRO_LOCK_READONLY);
    (void)lr;  // lock opcional (ajuda performance)

    unsigned char* dst = outRGB.data();
    for (int y = 0; y < h_; ++y)
    {
        unsigned char* d = dst + y * w_ * 3;
        for (int x = 0; x < w_; ++x)
        {
            ALLEGRO_COLOR c = al_get_pixel(backbuffer_, x, y);
            unsigned char r, g, b, a;
            al_unmap_rgba(c, &r, &g, &b, &a);
            d[0] = r;
            d[1] = g;
            d[2] = b;
            d += 3;
        }
    }
    if (lr)
        al_unlock_bitmap(backbuffer_);

    return true;
}

void AllegroBackend::shutdown()
{
    if (font_)
    {
        al_destroy_font(font_);
        font_ = nullptr;
    }
    if (backbuffer_)
    {
        al_destroy_bitmap(backbuffer_);
        backbuffer_ = nullptr;
    }
    if (initialized_)
    {
        al_shutdown_primitives_addon();
        al_uninstall_system();
        initialized_ = false;
    }
}

void AllegroBackend::ensureBackbuffer(int w, int h)
{
    w_ = w;
    h_ = h;
    if (backbuffer_)
    {
        al_destroy_bitmap(backbuffer_);
        backbuffer_ = nullptr;
    }
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    backbuffer_ = al_create_bitmap(w_, h_);
}

void AllegroBackend::clearBlack()
{
    if (!backbuffer_)
        return;
    al_set_target_bitmap(backbuffer_);
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

}  // namespace render

// ----- Factory -----
#include <memory>

#include "core/BackendFactory.hpp"

namespace core
{
std::unique_ptr<IRenderBackend> makeBackend(RenderBackendType type)
{
    switch (type)
    {
        case RenderBackendType::Allegro:
        default:
            return std::make_unique<render::AllegroBackend>();
    }
}
}  // namespace core
