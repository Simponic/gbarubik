#ifndef SCENE_HPP
#define SCENE_HPP

#include "model_instance.hpp"
#include "renderable.hpp"
#include "vector.hpp"
#include <cstdint>
#include <tonc.h>

class Scene {
public:
  usu::vector<std::shared_ptr<Renderable>> renderables;
  std::tuple<std::uint16_t, std::uint16_t>
      viewport_dimension; // <width, height>
  std::tuple<std::uint16_t, std::uint16_t> scene_dimension;
  VECTOR directional_light;
  FIXED z_plane;

  Scene();
  POINT project_2d(VECTOR vertex);
  POINT viewport_to_scene(POINT p);
  void draw_line(POINT p0, POINT p1, std::uint8_t pal_idx);
  static inline void render(std::shared_ptr<Scene> scene_ctx) {
    for (std::shared_ptr<Renderable> renderable : scene_ctx->renderables)
      renderable->render(scene_ctx);
  }
};

#endif // SCENE_HPP
