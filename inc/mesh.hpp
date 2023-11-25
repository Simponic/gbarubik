#ifndef MESH_HPP
#define MESH_HPP

#include "vector.hpp"
#include <memory>
#include <tonc.h>
#include <tuple>

class Scene;

typedef struct TRIANGLE {
  std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> vertex_indices;
  std::uint8_t color_idx;
} TRIANGLE;

class Mesh {
protected:
  usu::vector<VECTOR> m_vertices;
  usu::vector<TRIANGLE> m_triangles;
};

#endif // MESH_HPP
