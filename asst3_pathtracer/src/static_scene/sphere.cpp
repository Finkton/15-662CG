#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CMU462 { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  // TODO:
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.


    Vector3D oc = r.o - this->o;
    double a = r.d.norm2();
    double b = 2*dot(r.d,oc);
    double c = oc.norm2() - this->r * this->r;

    double delta = (b * b)/4.f - a * c;

    if(delta < 0) return false;

    t1 = (-(b/2.f) - sqrt(delta)) / a;
    t2 = (-(b/2.f) + sqrt(delta)) / a;

    return t2>0;

  // return false;

}

bool Sphere::intersect(const Ray& r) const {

  // TODO:
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.

  double t1, t2;
  if (!test(r, t1, t2) || t1 < r.min_t || t1 > r.max_t || t2 < r.min_t || t2 > r.max_t ){
    return false;
  }
  else{
    return true;
  }
  // return false;

}

bool Sphere::intersect(const Ray& r, Intersection *i) const {

  // TODO:
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.
  double t1, t2;
  bool hit = test(r, t1, t2);

  if (!hit){
    return false;
  }
  else{
    i->primitive = this;
    i->bsdf = this->get_bsdf();
    if(t1 > r.min_t){
      i->t = t1;
      i->n = normal(r.o + t1 * r.d);
    }
    else{
      i->t = t2;
      i->n = normal(r.o + t2 * r.d);
    }
    return true;
  }
}

void Sphere::draw(const Color& c) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CMU462
