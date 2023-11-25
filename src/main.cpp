#include "palette.hpp"
#include "vector.hpp"
#include <tonc.h>

int main() {
  // interrupt & mode 4 foo
  irq_init(NULL);
  irq_enable(II_VBLANK);
  REG_DISPCNT = DCNT_MODE4 | DCNT_BG2;

  // initialize our palette
  palette::put_palette((std::uint16_t *)MEM_PAL);

  // begin
  bmp16_line(1, 3, 1 + SCREEN_WIDTH / 2 - 2, SCREEN_HEIGHT, 0x0101, vid_page,
             SCREEN_WIDTH);
  vid_flip();
  bmp16_line(2, 3, 2 + SCREEN_WIDTH / 2 - 2, SCREEN_HEIGHT, 0x0101, vid_page,
             SCREEN_WIDTH);

  std::uint32_t frame = 0;
  while (1) {
    frame = (frame + 1) % 60;
    if (frame == 0) {
      vid_flip();
    }

    VBlankIntrWait();
  }
}
