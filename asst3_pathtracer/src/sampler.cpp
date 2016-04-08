#include "sampler.h"

namespace CMU462 {

// Uniform Sampler2D Implementation //

Vector2D UniformGridSampler2D::get_sample() const {

  // TODO:
  // Implement uniform 2D grid sampler

  double x = (double)(std::rand()) / RAND_MAX;
  double y = (double)(std::rand()) / RAND_MAX;

  return Vector2D(x,y);
  // return Vector2D(0.5,0.5);

}

// Uniform Hemisphere Sampler3D Implementation //

Vector3D UniformHemisphereSampler3D::get_sample() const {

  double Xi1 = (double)(std::rand()) / RAND_MAX;
  double Xi2 = (double)(std::rand()) / RAND_MAX;

  double theta = acos(Xi1);
  double phi = 2.0 * PI * Xi2;

  double xs = sinf(theta) * cosf(phi);
  double ys = sinf(theta) * sinf(phi);
  double zs = cosf(theta);

  return Vector3D(xs, ys, zs);

}

Vector3D CosineWeightedHemisphereSampler3D::get_sample() const {
  float f;
  return get_sample(&f);
}

Vector3D CosineWeightedHemisphereSampler3D::get_sample(float *pdf) const {
  // You may implement this, but don't have to.
  double Xi1 = (double)(std::rand()) / RAND_MAX;
  double Xi2 = (double)(std::rand()) / RAND_MAX;

  double theta = acos(sqrt(1.0-Xi1));
  double phi = 2.0 * PI * Xi2;

  // the probability of randomly selecting the random
  *pdf = cos(theta) / PI;

  double xs = sinf(theta) * cosf(phi);
  double ys = cosf(theta);
  double zs = sinf(theta) * sinf(phi);

  Vector3D y = Vector3D(0.0, 0.0, 1.0);
  Vector3D h = y;
  if (fabs(h.x)<=fabs(h.y) && fabs(h.x)<=fabs(h.z))
      h.x= 1.0;
  else if (fabs(h.y)<=fabs(h.x) && fabs(h.y)<=fabs(h.z))
      h.y= 1.0;
  else
      h.z= 1.0;

  Vector3D x = cross(h,y).unit();
  Vector3D z = cross(x,y).unit();

  Vector3D direction = xs * x + ys * y + zs * z;
  return direction.unit();
  // return Vector3D(0, 0, 1);
}


} // namespace CMU462
