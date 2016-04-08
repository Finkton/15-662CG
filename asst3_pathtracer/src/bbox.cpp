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

  template <typename T>
  void swap(T &a, T &b){
    T temp;
    temp = a;
    a = b;
    b = temp;
  }

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {
  // TODO:
  // Implement ray - bounding box intersection test
  // If the ray intersected the bouding box within the range given by
  // t0, t1, update t0 and t1 with the new intersection times.
  double xt0, xt1;
  xt0 = (min.x - r.o.x) / r.d.x;
  xt1 = (max.x - r.o.x) / r.d.x;
  if(xt0 > xt1) swap(xt0,xt1);

  double yt0, yt1;
  yt0 = (min.y - r.o.y) / r.d.y;
  yt1 = (max.y - r.o.y) / r.d.y;
  if(yt0 > yt1) swap(yt0,yt1);

  double zt0, zt1;
  zt0 = (min.z - r.o.z) / r.d.z;
  zt1 = (max.z - r.o.z) / r.d.z;
  if(zt0 > zt1) swap(zt0,zt1);

  double tmin, tmax;
  tmin = max3(xt0,yt0,zt0);
  tmax = min3(xt1,yt1,zt1);

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
