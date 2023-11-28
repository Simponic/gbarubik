#include "cube.hpp"
#include "palette.hpp"
#include "renderable.hpp"
#include "scene.hpp"
#include "vector.hpp"
#include <tonc.h>

int main() {
  // interrupt & mode 4 foo
  irq_init(NULL);
  irq_enable(II_VBLANK);
  REG_DISPCNT = DCNT_MODE4 | DCNT_BG2;
  palette::put_palette((std::uint16_t *)MEM_PAL);

  auto scene = std::make_shared<Scene>();

  auto cube = std::shared_ptr<Mesh>((Mesh *)new Cube);

  ModelInstance model_instance(cube, float2fx(0.25), {0, 0x0C00, 0},
                               {int2fx(3), int2fx(3), int2fx(3)});

  auto model_instance_ptr = std::shared_ptr<Renderable>(&model_instance);
  scene->renderables.add(model_instance_ptr);

  std::uint8_t frame = 0;
  while (1) {
    if (frame == 0) {
      model_instance.add_pos({0, 0, float2fx(0.2)});
      M4_CLEAR();
      Scene::render(scene);
      vid_flip();
    }
    frame = (frame + 1) % 10;

    VBlankIntrWait();
  }
}
