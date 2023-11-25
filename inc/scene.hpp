#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "mesh.hpp"
#include "vector.hpp"
#include <cstdint>

class Scene {
private:
  usu::vector<Mesh> meshes;
  std::uint32_t width;
  std::uint32_t height;

public:
  Scene();

  void render();
};

#endif // SCENE_HPP
