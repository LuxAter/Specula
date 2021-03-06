#ifndef SPECULA_CORE_SHAPE_HPP
#define SPECULA_CORE_SHAPE_HPP

#include "core/geometry/bounds.hpp"
#include "specula/global.hpp"

namespace specula {
class Shape {
public:
  Shape(const Transform *object_to_world, const Transform *world_to_object,
        bool reverse_orientation);
  virtual ~Shape();
  virtual Bounds3f object_bounds() const = 0;
  virtual Bounds3f world_bounds() const;
  virtual bool intersect(const Ray &ray, Float *thit, SurfaceInteraction *isect,
                         bool test_alpha_texture = true) const = 0;
  virtual bool intersect(const Ray &ray, bool test_alpha_texture = true) const {
    return intersect(ray, nullptr, nullptr, test_alpha_texture);
  }
  virtual Float area() const = 0;
  virtual Interaction sample(const Point2f &u, Float *pdf) const = 0;
  virtual Float pdf(const Interaction &) const { return 1.0f / area(); }
  virtual Interaction sample(const Interaction &ref, const Point2f &u,
                             Float *pdf) const;
  virtual Float pdf(const Interaction &ref, const Vector3f &wi) const;
  virtual Float solid_angle(const Point3f &p, int nsamples = 512) const;

  const Transform *object_to_world, *world_to_object;
  const bool reverse_orientation;
  const bool transform_swaps_handedness;
};
} // namespace specula

#endif // SPECULA_CORE_SHAPE_HPP
