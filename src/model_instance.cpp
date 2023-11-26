#include "model_instance.hpp"
#include "scene.hpp"
#include <tuple>

/**
static inline POINT viewportToScreen (POINT *point) {
    // Convert a viewport coordinate to screen x, y
    return createPoint(fixed_multiply(point->x, fixed_divide(WINDOW_WIDTH <<
FIX_SHIFT, viewport_width << FIX_SHIFT)), fixed_multiply(point->y,
fixed_divide(WINDOW_HEIGHT << FIX_SHIFT, viewport_height << FIX_SHIFT))
    );
}

static inline POINT projectVertex(VERTEX *vertex) {
    // Project a vertex to a point
    POINT temp;
    if (vertex->z != 0) {
        // Make sure we don't divide by zero
        temp = createPoint(fixed_multiply(vertex->x, fixed_divide(ZPlane <<
FIX_SHIFT, vertex->z)), fixed_multiply(vertex->y, fixed_divide(ZPlane <<
FIX_SHIFT, vertex->z))
        );
    }
    else {
        temp = createPoint(0, 0);
    }
    temp = viewportToScreen(&temp);
    return temp;
}

void renderInstance(INSTANCE *instance, SDL_Renderer *renderer) {
    // Render an instance
        // Array for projected points
    POINT projected[instance->model->vertices_length];
        // Pointers for transformed vertices
    VERTEX *transformed = malloc(sizeof(VERTEX) *
instance->model->vertices_length);

    for (int i = 0; i < instance->model->vertices_length; i++) {
        // Apply translation and rotation
        *(transformed + i) = *(instance->model->vertices + i);
        applyXRotation((transformed + i), *instance->xRotation);
        applyYRotation((transformed + i), *instance->yRotation);
        applyZRotation((transformed + i), *instance->zRotation);
                *(transformed + i) = addVertices((transformed + i),
instance->position);
        // Project vertices
        projected[i] = projectVertex(transformed + i);
    }
    VERTEX n, copyV;
        // A directional light source
    VERTEX playerLight = createVertex(0, 0, -1 << FIX_SHIFT);
    normalizeVertex(&playerLight);
    TRIANGLE *addr;
    FIXED intensity;
    COLOR clr;
    for (int i = 0; i < instance->model->triangles_length; i++) {
        // Render the triangle
        addr = (instance->model->triangles + i);
        n = computeNormal(transformed, addr);
        normalizeVertex(&n);
                // Intensity of light on the triangle
        intensity = fixed_multiply(dotProduct(&n, &playerLight) + (1 <<
FIX_SHIFT), 127 << FIX_SHIFT); copyV = *(transformed + addr->v2);
        normalizeVertex(&copyV);

                // Grayscale color of the triangle from light intensity
        clr = createColor(intensity >> FIX_SHIFT, intensity >> FIX_SHIFT,
intensity >> FIX_SHIFT);

        if (dotProduct(&n, &copyV) < 0) {
                        // The triangle is viewable by the camera
            drawFilledTriangle(
                &(projected[addr->v0]),
                &(projected[addr->v1]),
                &(projected[addr->v2]),
                &clr,
                renderer
            );

        }
    }
        transformed = NULL;
    free(transformed);




    static inline void applyXRotation(VERTEX *vertex, FIXED xRotation) {
    // Apply rotation to vertex on x-axis
    FIXED sinTheta = float_to_fixed(sin(fixed_to_float(xRotation) * (3.14159 /
180))); FIXED cosTheta = float_to_fixed(cos(fixed_to_float(xRotation) * (3.14159
/ 180))); FIXED y = vertex->y; FIXED z = vertex->z; vertex->y =
fixed_multiply(y, cosTheta) - fixed_multiply(z, sinTheta); vertex->z =
fixed_multiply(z, cosTheta) + fixed_multiply(y, sinTheta);
}

static inline void applyYRotation(VERTEX *vertex, FIXED yRotation) {
    // Apply rotation to vertex on y-axis
    FIXED sinTheta = float_to_fixed(sin(fixed_to_float(yRotation) * (3.14159 /
180))); FIXED cosTheta = float_to_fixed(cos(fixed_to_float(yRotation) * (3.14159
/ 180))); FIXED x = vertex->x; FIXED z = vertex->z; vertex->x =
fixed_multiply(x, cosTheta) + fixed_multiply(z, sinTheta); vertex->z =
fixed_multiply(z, cosTheta) - fixed_multiply(x, sinTheta);
}

static inline void applyZRotation(VERTEX *vertex, FIXED zRotation) {
    // Apply rotation to vertex on z-axis
    FIXED sinTheta = float_to_fixed(sin(fixed_to_float(zRotation) * (3.14159 /
180))); FIXED cosTheta = float_to_fixed(cos(fixed_to_float(zRotation) * (3.14159
/ 180))); FIXED x = vertex->x; FIXED y = vertex->y; vertex->x =
fixed_multiply(x, cosTheta) - fixed_multiply(y, sinTheta); vertex->y =
fixed_multiply(y, cosTheta) + fixed_multiply(x, sinTheta);
}
}
 */

VECTOR rotate(VECTOR v, VECTOR rot) {
  FIXED sin_theta_x, sin_theta_y, sin_theta_z;
  FIXED cos_theta_x, cos_theta_y, cos_theta_z;
  VECTOR res = {v.x, v.y, v.z};

  if (rot.x != 0) {
    sin_theta_x = lu_sin(rot.x) >> 4;
    cos_theta_x = lu_cos(rot.x) >> 4;
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
};

void ModelInstance::render(std::shared_ptr<Scene> scene_context) {
  usu::vector<VECTOR> transformed(m_mesh->vertices.size());
  usu::vector<POINT> projected(transformed.size());

  for (std::uint32_t i = 0; i < transformed.size(); i++) {
    transformed[i] = rotate(m_mesh->vertices[i], m_rotation);
    projected[i] = scene_context->project_2d(m_mesh->vertices[i]);
  }

  for (const TRIANGLE triangle : m_mesh->triangles) {
    VECTOR v0 = transformed[std::get<0>(triangle.vertex_indices)];
    VECTOR v1 = transformed[std::get<1>(triangle.vertex_indices)];
    VECTOR v2 = transformed[std::get<2>(triangle.vertex_indices)];
  }
}
