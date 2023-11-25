#include "mesh.hpp"
#include "scene.hpp"
#include <tuple>

void Mesh::render(std::shared_ptr<Scene> scene_context) {
  for (const TRIANGLE triangle : m_triangles) {
    VECTOR v0 = m_vertices[std::get<0>(triangle.vertex_indices)];
    VECTOR v1 = m_vertices[std::get<1>(triangle.vertex_indices)];
    VECTOR v2 = m_vertices[std::get<2>(triangle.vertex_indices)];
  }
}
