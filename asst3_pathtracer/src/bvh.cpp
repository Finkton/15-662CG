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


void BVHAccel::buildTree(BVHNode *root, size_t start, size_t end, size_t max_leaf_size){
  if ( end - start <= max_leaf_size) return;

  double minCost = INF_D;
  enum axis { X=0, Y=1, Z=2, N=3 } minAxis = N;
  double minCoor = 0;
  BBox minLeft;
  BBox minRight;

  double sn = root->bb.surface_area();
   for (size_t i = start; i < end; i++) {
     double cost[3] = {INF_D,INF_D,INF_D};
     double coor[3];
     BBox left[3] = {BBox(),BBox(),BBox()};
     BBox right[3] = {BBox(),BBox(),BBox()};
     bool update = false;


     coor[X] = primitives[i]->get_bbox().centroid().x;
     coor[Y] = primitives[i]->get_bbox().centroid().y;
     coor[Z] = primitives[i]->get_bbox().centroid().z;

     for (size_t j = start; j < end; j++) { // build XYZ left right tree
       if (coor[X] > primitives[j]->get_bbox().centroid().x) {
         left[X].expand(primitives[j]->get_bbox());
       }
       else {
         right[X].expand(primitives[j]->get_bbox());
       }

       if (coor[Y] > primitives[j]->get_bbox().centroid().y) {
         left[Y].expand(primitives[j]->get_bbox());
       }
       else {
         right[Y].expand(primitives[j]->get_bbox());
       }

       if (coor[Z] > primitives[j]->get_bbox().centroid().z) {
         left[Z].expand(primitives[j]->get_bbox());
       }
       else {
         right[Z].expand(primitives[j]->get_bbox());
       }
     } // end build XYZ left right tree

     if (left[X].size!=0 && right[X].size!=0) {
       cost[X] = left[X].size*(left[X].surface_area()/sn) + right[X].size*(right[X].surface_area()/sn);
     }

     if (left[Y].size!=0 && right[Y].size!=0) {
       cost[Y] = left[Y].size*(left[Y].surface_area()/sn) + right[Y].size*(right[Y].surface_area()/sn);
     }

     if (left[Z].size!=0 && right[Z].size!=0) {
       cost[Z] = left[Z].size*(left[Z].surface_area()/sn) + right[Z].size*(right[Z].surface_area()/sn);
     }

     if(cost[X] < minCost || cost[Y] < minCost || cost[Z] < minCost){
       update = true;
       if (cost[X]<=cost[Y] && cost[X]<=cost[Z]) {
         minAxis = X;
       }
       else if (cost[Y]<cost[X] && cost[Y]<=cost[Z]) {
         minAxis = Y;
       }
       else if (cost[Z]<cost[X] && cost[Z]<cost[Y]) {
         minAxis = Z;
       }
     }

     if(update){
       minCost = cost[minAxis];
       minCoor = coor[minAxis];
       minLeft = left[minAxis];
       minRight = right[minAxis];
     }
   } // end for i

   auto boundit = std::vector<Primitive*>::iterator(std::stable_partition(&primitives[start], &primitives[end],
           [minAxis,minCoor](Primitive *p) -> bool {
                      if(minAxis == X)  return minCoor > p->get_bbox().centroid().x;
                      if(minAxis == Y)  return minCoor > p->get_bbox().centroid().y;
                      if(minAxis == Z)  return minCoor > p->get_bbox().centroid().z;
                    }));

 size_t bound = boundit - primitives.begin();
 root->l = new BVHNode(minLeft, start, bound - start);
 root->r = new BVHNode(minRight, bound, end - bound);
 buildTree(root->l, start, bound, max_leaf_size);
 buildTree(root->r, bound, end, max_leaf_size);
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

  // std::cout<<"intersect!"<<std::endl;
  // bool hit = false;
  // for (size_t p = 0; p < primitives.size(); ++p) {
  //   if(primitives[p]->intersect(ray)) hit = true;
  // }
  //
  // return hit;

  Intersection i;
  return find_closest_hit(ray, root, &i);

}

bool BVHAccel::intersect(const Ray &ray, Intersection *i) const {

  // TODO:
  // Implement ray - bvh aggregate intersection test. A ray intersects
  // with a BVH aggregate if and only if it intersects a primitive in
  // the BVH that is not an aggregate. When an intersection does happen.
  // You should store the non-aggregate primitive in the intersection data
  // and not the BVH aggregate itself.

  // std::cout<<"intersect2!"<<std::endl;
  // bool hit = false;
  // for (size_t p = 0; p < primitives.size(); ++p) {
  //   if(primitives[p]->intersect(ray, i)) hit = true;
  // }
  //
  // return hit;

  return find_closest_hit(ray,root,i);

}

bool BVHAccel::find_closest_hit(const Ray& r, BVHNode* node, Intersection* i) const{
  // std::cout<< "find_closest_hit"<<std::endl;
  double t0 = r.min_t, t1 = r.max_t;
  if(!node->bb.intersect(r,t0,t1) || t0 > i->t){ // closest point farther than i point
    return false;
  }
  // std::cout<< "find_closest_hit"<<std::endl;
  if (node->isLeaf()) {
    bool hit = false;
    for (size_t p = node->start; p < node->start + node->range; p++) {
      Intersection currenti;
      if (primitives[p]->intersect(r, &currenti) && currenti.t < i->t) {
        hit = true;
        i->n = currenti.n;
        i->bsdf = currenti.bsdf;
        i->primitive = currenti.primitive;
        i->t = currenti.t;
      }
    }
    return hit;
  }
  else {
    // return find_closest_hit(r, node->r, i) || find_closest_hit(r, node->l, i);
    bool hit_r = find_closest_hit(r, node->r, i);
    bool hit_l = find_closest_hit(r, node->l, i);
    return hit_r || hit_l;
  }
}

}  // namespace StaticScene
}  // namespace CMU462
