#include "scene.hpp"
#include <memory>

Scene::Scene() {
  directional_light = {0, 0, -1};
  viewport_dimension = {2, 3};
  scene_dimension = {SCREEN_WIDTH, SCREEN_HEIGHT};

  z_plane = int2fx(1);
}

void Scene::render() {
  auto this_ptr = std::make_shared<Scene>(this);
  for (auto renderable : renderables) {
    renderable.render(this_ptr);
  }
}

POINT Scene::project_2d(VECTOR vertex) {
  POINT pt = {0, 0};

  if (vertex.z != 0) {
    pt.x = fxdiv(z_plane, vertex.z);
    pt.y = fxmul(vertex.y, pt.x);
    pt.x = fxmul(vertex.x, pt.x);
  }

  return pt;
}
