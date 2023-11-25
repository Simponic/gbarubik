#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <cstdint>
#include <tonc.h>

namespace palette {

constexpr std::uint8_t pal_len = 255;
constexpr std::uint16_t cube_colors[6] = {CLR_WHITE,  CLR_YELLOW, CLR_RED,
                                          CLR_ORANGE, CLR_BLUE,   CLR_GREEN};

constexpr void put_palette(std::uint16_t *palette_address) {
  toncset16(palette_address, CLR_BLACK, 1);
  toncset16(palette_address + 1, CLR_WHITE, 1);
}

}; // namespace palette

#endif // PALETTE_HPP
