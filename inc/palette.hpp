#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <cstdint>
#include <tonc.h>

namespace palette {

constexpr std::uint8_t pal_len = 255;
constexpr std::uint16_t cube_colors[6] = {CLR_WHITE,  CLR_YELLOW, CLR_RED,
                                          CLR_ORANGE, CLR_BLUE,   CLR_GREEN};

void put_palette(std::uint16_t *palette_address) {
  toncset16(palette_address, CLR_BLACK, 1);
  for (std::uint32_t i = 0; i < 6; ++i)
    toncset16(palette_address + i + 1, cube_colors[i], 1);
  // TODO: PALETTE GRADIENT IN ALPHA DIMENSION (BRIGHTNESS)
}
}; // namespace palette

#endif // PALETTE_HPP
