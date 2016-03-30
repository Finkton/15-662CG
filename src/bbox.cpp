#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CMU462 {

  template <typename T>
  const T min3(const T a, const T b, const T c)
  {
    return std::min(a, std::min(b, c));
  }

  template <typename T>
  const T max3(const T a, const T b, const T c)
  {
    return std::max(a, std::max(b, c));
  }

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {
  // TODO:
  // Implement ray - bounding box intersection test
  // If the ray intersected the bouding box within the range given by
  // t0, t1, update t0 and t1 with the new intersection times.
  double tminx, tmaxx, tminy, tmaxy, tminz, tmaxz;
  double tmin, tmax;

  tmin = (min.x - r.o.x) / r.d.x;
  tmax = (max.x - r.o.x) / r.d.x;
  tminx = std::min(tmin, tmax);
  tmaxx = std::max(tmin, tmax);

  tmin = (min.y - r.o.y) / r.d.y;
  tmax = (max.y - r.o.y) / r.d.y;
  tminy = std::min(tmin, tmax);
  tmaxy = std::max(tmin, tmax);

  tmin = (min.z - r.o.z) / r.d.z;
  tmax = (max.z - r.o.z) / r.d.z;
  tminz = std::min(tmin, tmax);
  tmaxz = std::max(tmin, tmax);

  tmin = max3(tminx,tminy,tminz);
  tmax = min3(tmaxx,tmaxy,tmaxz);

  if(tmin >= tmax){
    return false;
  }
  else{
    t1 = tmax;
    if(tmin >= t0) t0 = tmin;
    return true;
  }
}

void BBox::draw(Color c) const {

  glColor4f(c.r, c.g, c.b, c.a);

	// top
	glBegin(GL_LINE_STRIP);
	glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(max.x, max.y, max.z);
	glEnd();

	// bottom
	glBegin(GL_LINE_STRIP);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, min.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, min.y, min.z);
	glEnd();

	// side
	glBegin(GL_LINES);
	glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, min.y, max.z);
	glVertex3d(max.x, max.y, min.z);
  glVertex3d(max.x, min.y, min.z);
	glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, min.y, min.z);
	glVertex3d(min.x, max.y, max.z);
  glVertex3d(min.x, min.y, max.z);
	glEnd();

}

std::ostream& operator<<(std::ostream& os, const BBox& b) {
  return os << "BBOX(" << b.min << ", " << b.max << ")";
}

} // namespace CMU462
