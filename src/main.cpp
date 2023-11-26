#include "cube.hpp"
#include "palette.hpp"
#include "renderable.hpp"
#include "scene.hpp"
#include "vector.hpp"
#include <tonc.h>

class Box : public Renderable {
public:
  virtual void render(std::shared_ptr<Scene> scene) {
    scene->draw_line({0, 0}, {2 << FIX_SHIFT, 3 << FIX_SHIFT}, 1);
  }
};

int main() {
  // interrupt & mode 4 foo
  irq_init(NULL);
  irq_enable(II_VBLANK);
  REG_DISPCNT = DCNT_MODE4 | DCNT_BG2;
  palette::put_palette((std::uint16_t *)MEM_PAL);

  auto scene = std::make_shared<Scene>();
  //  auto cube = std::shared_ptr<Renderable>((Renderable *)new Cube());
  //  scene->renderables.add(cube);
  auto box = std::shared_ptr<Renderable>((Renderable *)new Box());

  scene->renderables.add(box);

  while (1) {
    Scene::render(scene);

    VBlankIntrWait();
  }
}
