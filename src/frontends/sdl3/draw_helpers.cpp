#include "draw_helpers.hpp"

uint32_t create_texture() {
  uint32_t tex_id{};
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  return tex_id;
}

void update_texture(uint32_t tex_id, int32_t width, int32_t height,
                    void *data) {
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t color_to_surface_rgb(SDL_Surface *surface, nes_emu::Color color) {
  return SDL_MapSurfaceRGB(surface, color.red, color.green, color.blue);
}

void update_surface(SDL_Surface *surface, nes_emu::Sprite &data) {
  for (int x = 0; x < data.width; x++) {
    for (int y = 0; y < data.height; y++) {
      SDL_Rect rect = {x, y, 1, 1};
      SDL_FillSurfaceRect(surface, &rect,
                          color_to_surface_rgb(surface, data.get_pixel(x, y)));
    }
  }
}