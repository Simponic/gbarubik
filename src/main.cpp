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

  ModelInstance modelInstance(cube, int2fx(2), {0x0C7F, 0x0000, 0},
                              {int2fx(0), 0, int2fx(0)});

  auto modelInstancePtr = std::shared_ptr<Renderable>(&modelInstance);
  scene->renderables.add(modelInstancePtr);

  while (1) {

    Scene::render(scene);
    vid_flip();

    VBlankIntrWait();
  }
}
