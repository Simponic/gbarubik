#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <memory>

class Scene;

class Renderable {
public:
  virtual void render(std::shared_ptr<Scene> scene_context) = 0;
};

#endif
