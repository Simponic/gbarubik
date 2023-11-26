#ifndef MODEL_INSTANCE_HPP
#define MODEL_INSTANCE_HPP

#include "mesh.hpp"
#include "renderable.hpp"
#include <tonc.h>

class ModelInstance : Renderable {
private:
  FIXED m_scale;
  VECTOR m_rotation; // though technically "FIXED"'s, these are simply s32's
                     // where [0, 2pi] -> [0, 0xFFFF] in the x,y,z axes
  VECTOR m_pos;

  std::shared_ptr<Mesh> m_mesh;

public:
  void render(std::shared_ptr<Scene> scene_context);
};

#endif // MODEL_INSTANCE_HPP
