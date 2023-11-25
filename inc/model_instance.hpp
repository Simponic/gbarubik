#ifndef MODEL_INSTANCE_HPP
#define MODEL_INSTANCE_HPP

#include "mesh.hpp"
#include "renderable.hpp"
#include <tonc.h>

class ModelInstance : Renderable {
private:
  FIXED m_scale;
  VECTOR m_rotation;
  VECTOR m_pos;

  std::shared_ptr<Mesh> m_mesh;
};

#endif // MODEL_INSTANCE_HPP
