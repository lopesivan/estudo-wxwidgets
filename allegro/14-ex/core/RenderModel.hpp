// core/RenderModel.hpp
#pragma once
#include <cmath>

namespace core {

constexpr float X_KM_MAX = 1920.0f;
constexpr float Y_KM_MAX = 1080.0f;

struct RenderModel {
  // estado de interação
  float mouse_x_px = 0.0f;
  float mouse_y_px = 0.0f;
  int width = 1280;
  int height = 720;

  // dois pontos em "km"
  float pos_km_x[2] = {0.0f, 0.0f};
  float pos_km_y[2] = {0.0f, 0.0f};
  int points_stored = 0;

  void resetar_posicoes() {
    pos_km_x[0] = pos_km_y[0] = 0;
    pos_km_x[1] = pos_km_y[1] = 0;
  }
};

// ---- Funções do mouse_pos.c (portadas p/ C++) ----
inline void armazenar_posicao_mouse(float mouse_x, float mouse_y,
                                    int screen_width, int screen_height,
                                    float *pos_km_x, float *pos_km_y,
                                    int index) {
  float km_x = (mouse_x / screen_width) * X_KM_MAX;
  float km_y = Y_KM_MAX - ((mouse_y / screen_height) * Y_KM_MAX);
  pos_km_x[index] = km_x;
  pos_km_y[index] = km_y;
}

inline void converter_km_para_pixel(float km_x, float km_y, int screen_width,
                                    int screen_height, float *px, float *py) {
  *px = (km_x / X_KM_MAX) * screen_width;
  *py = screen_height - (km_y / Y_KM_MAX) * screen_height;
}

inline float calcular_modulo_vetor(float x0, float y0, float x1, float y1) {
  float dx = x1 - x0;
  float dy = y1 - y0;
  return std::sqrt(dx * dx + dy * dy);
}

} // namespace core
