#include "cube.hpp"

Cube::Cube() {
  for (std::uint8_t i = 0; i < 8; ++i)
    vertices.add({(i >> 2) & 1, (i >> 1) & 1, i & 1});

  triangles.add({{0, 4, 5}, 1});
  triangles.add({{0, 1, 5}, 1});
  triangles.add({{4, 6, 7}, 1});
  triangles.add({{4, 5, 7}, 1});
  triangles.add({{1, 5, 7}, 1});
  triangles.add({{1, 3, 7}, 1});
  triangles.add({{0, 4, 6}, 1});
  triangles.add({{0, 2, 6}, 1});
  triangles.add({{0, 2, 1}, 1});
  triangles.add({{1, 2, 3}, 1});
  triangles.add({{2, 7, 3}, 1});
  triangles.add({{2, 5, 7}, 1});
}
