#include "scene.hpp"

Scene::Scene() {
  directional_light = {0, 0, -1};
  viewport_dimension = {2, 3};
  scene_dimension = {SCREEN_WIDTH / 2, SCREEN_HEIGHT};

  z_plane = int2fx(1);
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

POINT Scene::viewport_to_scene(POINT p) {
  FIXED x = fxmul(
      p.x, (std::get<0>(scene_dimension) / std::get<0>(viewport_dimension))
               << FIX_SHIFT);
  FIXED y = fxmul(
      p.y, (std::get<1>(scene_dimension) / std::get<1>(viewport_dimension))
               << FIX_SHIFT);
  return {x, y};
}

void Scene::draw_line(POINT p0, POINT p1, std::uint8_t pal_idx) {
  POINT scene_p0 = viewport_to_scene(p0);
  POINT scene_p1 = viewport_to_scene(p1);

  std::uint16_t pixels = (pal_idx << 8) | pal_idx;
  bmp16_line(scene_p0.x, scene_p0.y, scene_p1.x, scene_p1.y, pixels, vid_page,
             M4_WIDTH);
}
