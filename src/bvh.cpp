#include "bvh.h"

#include "CMU462/CMU462.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CMU462 { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  this->primitives = _primitives;

  // TODO:
  // Construct a BVH from the given vector of primitives and maximum leaf
  // size configuration. The starter code build a BVH aggregate with a
  // single leaf node (which is also the root) that encloses all the
  // primitives.

  BBox bb;
  for (size_t i = 0; i < primitives.size(); ++i) {
    bb.expand(primitives[i]->get_bbox());
  }

  root = new BVHNode(bb, 0, primitives.size());
  buildTree(root,0,primitives.size(),max_leaf_size);

}


void BVHAccel::buildTree(BVHNode *root, size_t start, size_t range, size_t max_leaf_size){
  if (range <= max_leaf_size) return;

  double minCost = std::numeric_limits<double>::infinity();
  enum axis { N, X, Y, Z } minAxis = N;
  double minValue = 0;
  BBox minLeft;
  BBox minRight;

  double sn = root->bb.surface_area();
   for (size_t i = start; i < start+range; i++) {
     double xvalue = primitives[i]->get_bbox().centroid().x;
     BBox xleft=BBox();
     BBox xright=BBox();

     double yvalue = primitives[i]->get_bbox().centroid().y;
     BBox yleft=BBox();
     BBox yright=BBox();

     double zvalue = primitives[i]->get_bbox().centroid().z;
     BBox zleft=BBox();
     BBox zright=BBox();

     for (size_t j = start; j < start+range; j++) {
       if (xvalue > primitives[j]->get_bbox().centroid().x) {
         xleft.expand(primitives[j]->get_bbox());
       }
       else {
         xright.expand(primitives[j]->get_bbox());
       }

       if (yvalue > primitives[j]->get_bbox().centroid().y) {
         yleft.expand(primitives[j]->get_bbox());
       }
       else {
         yright.expand(primitives[j]->get_bbox());
       }

       if (zvalue > primitives[j]->get_bbox().centroid().z) {
         zleft.expand(primitives[j]->get_bbox());
       }
       else {
         zright.expand(primitives[j]->get_bbox());
       }
     } // end for j

     double xcost = std::numeric_limits<double>::infinity();
     if (xleft.size!=0 && xright.size!=0) {
       xcost = xleft.size*(xleft.surface_area()/sn) + xright.size*(xright.surface_area()/sn);
     }

     double ycost = std::numeric_limits<double>::infinity();
     if (yleft.size!=0 && yright.size!=0) {
       ycost = yleft.size*(yleft.surface_area()/sn) + yright.size*(yright.surface_area()/sn);
     }

     double zcost = std::numeric_limits<double>::infinity();
     if (zleft.size!=0 && zright.size!=0) {
       zcost = zleft.size*(zleft.surface_area()/sn) + zright.size*(zright.surface_area()/sn);
     }

     if (xcost < minCost && xcost<ycost && xcost<zcost) {
       minCost = xcost;
       minAxis = X;
       minValue = xvalue;
       minLeft = xleft;
       minRight = xright;
     }
     else if (ycost < minCost && ycost<xcost && ycost<zcost) {
       minCost = ycost;
       minAxis = Y;
       minValue = yvalue;
       minLeft = yleft;
       minRight = yright;
     }
     else if (zcost < minCost && zcost<xcost && zcost<ycost) {
       minCost = zcost;
       minAxis = Z;
       minValue = zvalue;
       minLeft = zleft;
       minRight = zright;
     }
   } // end for i

 std::vector<Primitive*>::iterator bound;
 if(minAxis == X){
   bound = std::vector<Primitive*>::iterator(
     std::partition(&primitives[start], &primitives[start+range],
                    [minValue](Primitive *p) -> bool {
                      return minValue > p->get_bbox().centroid().x;
                    }));
 }
 else if(minAxis == Y){
   bound = std::vector<Primitive*>::iterator(
     std::partition(&primitives[start], &primitives[start+range],
                    [minValue](Primitive *p) -> bool {
                      return minValue > p->get_bbox().centroid().y;
                    }));
 }
 else if(minAxis == Z){
   bound = std::vector<Primitive*>::iterator(
     std::partition(&primitives[start], &primitives[start+range],
                    [minValue](Primitive *p) -> bool {
                      return minValue > p->get_bbox().centroid().z;
                    }));
 }
 else {
   return;
 }

 size_t mid = bound - primitives.begin();

 root->l = new BVHNode(minLeft, start, mid - start);
 root->r = new BVHNode(minRight, mid, range - (mid - start));

 buildTree(root->l, start, mid - start, max_leaf_size);
 buildTree(root->r, mid, range - (mid - start), max_leaf_size);
}

BVHAccel::~BVHAccel() {

  // TODO:
  // Implement a proper destructor for your BVH accelerator aggregate

}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

bool BVHAccel::intersect(const Ray &ray) const {

  // TODO:
  // Implement ray - bvh aggregate intersection test. A ray intersects
  // with a BVH aggregate if and only if it intersects a primitive in
  // the BVH that is not an aggregate.
  bool hit = false;
  for (size_t p = 0; p < primitives.size(); ++p) {
    if(primitives[p]->intersect(ray)) hit = true;
  }

  return hit;

}

bool BVHAccel::intersect(const Ray &ray, Intersection *i) const {

  // TODO:
  // Implement ray - bvh aggregate intersection test. A ray intersects
  // with a BVH aggregate if and only if it intersects a primitive in
  // the BVH that is not an aggregate. When an intersection does happen.
  // You should store the non-aggregate primitive in the intersection data
  // and not the BVH aggregate itself.

  bool hit = false;
  for (size_t p = 0; p < primitives.size(); ++p) {
    if(primitives[p]->intersect(ray, i)) hit = true;
  }

  return hit;

}

void find_closest_hit(Ray& r, BVHNode* node, Intersection* i){



}

}  // namespace StaticScene
}  // namespace CMU462
