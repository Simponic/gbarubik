#ifndef SCENE_HPP
#define SCENE_HPP

#include "model_instance.hpp"
#include "vector.hpp"
#include <cstdint>
#include <tonc.h>

class Scene {
public:
  usu::vector<Renderable> renderables;
  std::tuple<std::uint16_t, std::uint16_t>
      viewport_dimension; // <width, height>
  std::tuple<std::uint16_t, std::uint16_t> scene_dimension;
  VECTOR directional_light;
  FIXED z_plane;

  Scene();
  void render();
  POINT project_2d(VECTOR vertex);
};

#endif // SCENE_HPP
