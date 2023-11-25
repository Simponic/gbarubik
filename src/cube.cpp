#include "cube.hpp"

Cube::Cube() {
  for (std::uint8_t i = 0; i < 8; ++i)
    m_vertices.add({(i >> 2) & 1, (i >> 1) & 1, i & 1});
}
