#include "model_instance.hpp"
#include "scene.hpp"
#include <tuple>

ModelInstance::ModelInstance(std::shared_ptr<Mesh> mesh, FIXED scale,
                             VECTOR rotation, VECTOR position)
    : m_mesh(mesh), m_scale(scale), m_rot(rotation), m_pos(position) {}

VECTOR rotate(VECTOR v, VECTOR rot) {
  FIXED sin_theta_x, sin_theta_y, sin_theta_z;
  FIXED cos_theta_x, cos_theta_y, cos_theta_z;

  VECTOR res = {v.x, v.y, v.z};

  if (rot.x != 0) {
    sin_theta_x = float2fx(0.707); // lu_sin(rot.x) >> 4;
    cos_theta_x = float2fx(0.707); // lu_cos(rot.x) >> 4;
    res.y = fxmul(res.y, cos_theta_x) - fxmul(res.z, sin_theta_x);
    res.z = fxmul(res.z, cos_theta_x) + fxmul(res.y, sin_theta_x);
  }

  if (rot.y != 0) {
    sin_theta_y = lu_sin(rot.y) >> 4;
    cos_theta_y = lu_cos(rot.y) >> 4;
    res.x = fxmul(res.x, cos_theta_y) + fxmul(res.z, sin_theta_y);
    res.z = fxmul(res.z, cos_theta_y) - fxmul(res.x, sin_theta_y);
  }

  if (rot.z != 0) {
    sin_theta_z = lu_sin(rot.z) >> 4;
    cos_theta_z = lu_cos(rot.z) >> 4;
    res.x = fxmul(res.x, cos_theta_z) - fxmul(res.y, sin_theta_z);
    res.y = fxmul(res.z, cos_theta_z) + fxmul(res.x, sin_theta_z);
  }

  return res;
}

void ModelInstance::render(std::shared_ptr<Scene> scene_context) {
  usu::vector<POINT> projected(m_mesh->vertices.size());

  for (std::uint32_t i = 0; i < projected.size(); i++) {
    VECTOR transformed =
        m_mesh->vertices[i]; // rotate(m_mesh->vertices[i], m_rot);
    //    vec_add(&transformed, &transformed, &m_pos);

    projected[i] = scene_context->project_2d(transformed);
  }

  for (const TRIANGLE triangle : m_mesh->triangles) {
    POINT v0 = projected[std::get<0>(triangle.vertex_indices)];
    POINT v1 = projected[std::get<1>(triangle.vertex_indices)];
    POINT v2 = projected[std::get<2>(triangle.vertex_indices)];

    scene_context->draw_line(v0, v1, triangle.color_idx);
    scene_context->draw_line(v1, v2, triangle.color_idx);
    scene_context->draw_line(v2, v0, triangle.color_idx);
  }
}
