#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int main(int, char**) {
    if (!al_init()) return 1;
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    if (!disp) return 2;

    al_clear_to_color(al_map_rgb(30, 30, 40));
    al_draw_line(50, 50, 590, 430, al_map_rgb(255, 255, 0), 3.0f);
    al_flip_display();

    // Espera 2s ou ESC
    ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(q, al_get_keyboard_event_source());
    al_register_event_source(q, al_get_timer_event_source(timer));
    al_start_timer(timer);

    double start = al_get_time();
    bool done = false;
    while (!done) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(q, &ev)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                done = true;
        }
        if (al_get_time() - start > 2.0)
            done = true;
    }

    al_destroy_event_queue(q);
    al_destroy_timer(timer);
    al_destroy_display(disp);
    return 0;
}

