#include "Camera.hpp"
#include <tonc.h>

int main() {
  irq_init(NULL);
  irq_enable(II_VBLANK);

  REG_DISPCNT = DCNT_MODE4;

  while (1) {
    VBlankIntrWait();
  }
}
