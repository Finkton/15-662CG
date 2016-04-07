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
  // std::cout<<"my bbox";
  double tminx, tmaxx, tminy, tmaxy, tminz, tmaxz;
  double tmin, tmax;

  tminx = (min.x - r.o.x) / r.d.x;
  tmaxx = (max.x - r.o.x) / r.d.x;
  if(tminx > tmaxx) swap(tminx,tmaxx);

  tminy = (min.y - r.o.y) / r.d.y;
  tmaxy = (max.y - r.o.y) / r.d.y;
  if(tminy > tmaxy) swap(tminy,tmaxy);

  tminz = (min.z - r.o.z) / r.d.z;
  tmaxz = (max.z - r.o.z) / r.d.z;
  if(tminz > tmaxz) swap(tminz,tmaxz);

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
