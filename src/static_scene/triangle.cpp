#include "triangle.h"

#include "CMU462/CMU462.h"
#include "GL/glew.h"

namespace CMU462 { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  // TODO:
  // compute the bounding box of the triangle

  return BBox();
}

bool Triangle::intersect(const Ray& r) const {

  // TODO: implement ray-triangle intersection
  std::cout<<"Start intersecting!"<<std::endl;
  Vector3D p0 = mesh->positions[v1];
  Vector3D p1 = mesh->positions[v2];
  Vector3D p2 = mesh->positions[v3];
  Vector3D e1 = p1 - p0;
  Vector3D e2 = p2 - p0;
  Vector3D s = r.o - p0;
  Vector3D d = r.d;

  Vector3D e1xd = cross(e1,d);
  Vector3D sxe2 = cross(s,e2);

  double den = dot(e1xd,e2);
  if(den == 0.0) {
    return false;
  }

  double u = dot(sxe2,d) / den;
  double v = dot(e1xd,s) / den;
  double t = dot(sxe2,e1) / den;
  if(u>0 && v>0 && u+v<1 && t>=r.min_t && t <= r.max_t){
    std::cout<<"true!";
    return true;
  }
  else{
    return false;
  }
}

bool Triangle::intersect(const Ray& r, Intersection *isect) const {

  // TODO:
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly


  return false;
}

void Triangle::draw(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CMU462
