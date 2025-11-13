#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LARGURA 1200
#define ALTURA 700
#define FPS 60

float escala = 30.0f;

typedef struct {
  float x, y;
  float raio;
} Circulo;

Circulo circulos[100];
int num_circulos = 0;

void mundo_para_tela(float mx, float my, int *px, int *py) {
  *px = LARGURA / 2 + (int)(mx * escala);
  *py = ALTURA / 2 - (int)(my * escala);
}

void tela_para_mundo(int px, int py, float *mx, float *my) {
  *mx = (px - LARGURA / 2) / escala;
  *my = (ALTURA / 2 - py) / escala;
}

void desenhar_grade(ALLEGRO_FONT *font) {
  // Calcular intervalo de grade baseado na escala
  float intervalo = 1.0f;
  if (escala < 15.0f)
    intervalo = 5.0f;
  else if (escala < 30.0f)
    intervalo = 2.0f;

  int max = (int)(LARGURA / (escala * intervalo)) + 2;

  // Linhas da grade
  for (int i = -max; i <= max; ++i) {
    float pos = i * intervalo;
    int x = LARGURA / 2 + (int)(pos * escala);
    int y = ALTURA / 2 - (int)(pos * escala);

    if (x >= 0 && x <= LARGURA)
      al_draw_line(x, 0, x, ALTURA, al_map_rgb(50, 50, 50), 1);
    if (y >= 0 && y <= ALTURA)
      al_draw_line(0, y, LARGURA, y, al_map_rgb(50, 50, 50), 1);
  }

  // Eixos principais
  al_draw_line(LARGURA / 2, 0, LARGURA / 2, ALTURA, al_map_rgb(200, 200, 200),
               2);
  al_draw_line(0, ALTURA / 2, LARGURA, ALTURA / 2, al_map_rgb(200, 200, 200),
               2);

  // Números (apenas se escala permitir legibilidade)
  if (escala > 20.0f) {
    for (int i = -max; i <= max; ++i) {
      if (i == 0)
        continue;

      float val = i * intervalo;
      char texto[10];
      int px, py;

      mundo_para_tela(val, 0, &px, &py);
      if (px >= 0 && px <= LARGURA) {
        snprintf(texto, sizeof(texto), "%.0f", val);
        al_draw_text(font, al_map_rgb(255, 255, 255), px, ALTURA / 2 + 5,
                     ALLEGRO_ALIGN_CENTER, texto);
      }

      mundo_para_tela(0, val, &px, &py);
      if (py >= 0 && py <= ALTURA) {
        snprintf(texto, sizeof(texto), "%.0f", val);
        al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA / 2 + 5, py,
                     ALLEGRO_ALIGN_LEFT, texto);
      }
    }
  }
}

int main() {
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  ALLEGRO_TIMER *timer = NULL;
  ALLEGRO_FONT *font = NULL;

  al_init();
  al_install_mouse();
  al_install_keyboard();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();

  display = al_create_display(LARGURA, ALTURA);
  fila_eventos = al_create_event_queue();
  timer = al_create_timer(1.0 / FPS);
  font = al_load_font("font.ttf", 18, 0);
  if (!font)
    font = al_create_builtin_font();

  al_register_event_source(fila_eventos, al_get_display_event_source(display));
  al_register_event_source(fila_eventos, al_get_mouse_event_source());
  al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

  al_start_timer(timer);

  bool sair = false;
  bool redesenhar = true;
  bool arrastando = false;
  int circulo_selecionado = -1;

  while (!sair) {
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);

    if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      sair = true;
    } else if (evento.type == ALLEGRO_EVENT_TIMER) {
      if (arrastando)
        redesenhar = true;
    } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
      if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        sair = true;
      } else if (evento.keyboard.keycode == ALLEGRO_KEY_Z) {
        escala -= 2.0f;
        if (escala < 5.0f)
          escala = 5.0f;
        redesenhar = true;
      } else if (evento.keyboard.keycode == ALLEGRO_KEY_X) {
        escala += 2.0f;
        if (escala > 200.0f)
          escala = 200.0f;
        redesenhar = true;
      }
    } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
      float mx, my;
      tela_para_mundo(evento.mouse.x, evento.mouse.y, &mx, &my);
      circulo_selecionado = -1;

      for (int i = 0; i < num_circulos; ++i) {
        float dx = mx - circulos[i].x;
        float dy = my - circulos[i].y;
        if (dx * dx + dy * dy <= circulos[i].raio * circulos[i].raio) {
          circulo_selecionado = i;
          arrastando = true;
          break;
        }
      }

      if (circulo_selecionado == -1 && num_circulos < 100) {
        circulos[num_circulos].x = mx;
        circulos[num_circulos].y = my;
        circulos[num_circulos].raio = 0.5f;
        num_circulos++;
      }
      redesenhar = true;
    } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
      arrastando = false;
      redesenhar = true;
    } else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES && arrastando &&
               circulo_selecionado != -1) {
      float mx, my;
      tela_para_mundo(evento.mouse.x, evento.mouse.y, &mx, &my);
      circulos[circulo_selecionado].x = mx;
      circulos[circulo_selecionado].y = my;
      redesenhar = true;
    }

    if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
      redesenhar = false;

      al_clear_to_color(al_map_rgb(20, 20, 20));
      desenhar_grade(font);

      for (int i = 0; i < num_circulos; ++i) {
        int px, py;
        mundo_para_tela(circulos[i].x, circulos[i].y, &px, &py);
        int raio = (int)(circulos[i].raio * escala);
        al_draw_circle(px, py, raio, al_map_rgb(0, 255, 0), 2);
      }

      al_flip_display();
    }
  }

  al_destroy_timer(timer);
  al_destroy_font(font);
  al_destroy_display(display);
  al_destroy_event_queue(fila_eventos);
  return 0;
}
