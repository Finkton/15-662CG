/*
 * Student solution for CMU 15-462 Project 2 (MeshEdit)
 *
 * Implemented by Xiaoshan Lu on Feburary to March, 2016.
 *
 */

#include "student_code.h"
#include "mutablePriorityQueue.h"

namespace CMU462
{
  static MutablePriorityQueue<EdgeRecord> erQueue;

   VertexIter HalfedgeMesh::splitEdge( EdgeIter e0 )
   {
      // TODO This method should split the given edge and return an iterator to the newly inserted vertex.
      // TODO The halfedge of this vertex should point along the edge that was split, rather than the new edges.
      // split boundary
      if(e0->halfedge()->face()->isBoundary()||e0->halfedge()->twin()->face()->isBoundary()){
        std::cerr << "Split a boundary." << std::endl;
        HalfedgeIter h[10];
        if(!e0->halfedge()->isBoundary()){
          h[0] = e0->halfedge();
        }
        else{
          h[0] = e0->halfedge()->twin();
        }

        h[1] = h[0]->next();
        h[2] = h[1]->next();
        h[3] = h[0]->twin();
        h[4] = h[1]->twin();
        h[5] = h[2]->twin();
        for(int i=6;i<10;i++){
          h[i] = newHalfedge();
        }

        VertexIter v[4];
        v[0] = h[0]->vertex();
        v[1] = h[1]->vertex();
        v[2] = h[2]->vertex();
        // new
        v[3] = newVertex();
        v[3]->halfedge() = h[7];
        v[3]->position = (v[0]->position + v[1]->position)/2;

        EdgeIter e[3];
        e[0] = e0;
        for(int i=1;i<3;i++){
          e[i] = newEdge();
        }
        e[0]->halfedge() = h[0];
        e[1]->halfedge() = h[6];
        e[2]->halfedge() = h[8];

        FaceIter f[3];
        f[1] = h[0]->face();
        f[2] = newFace();
        f[0] = newBoundary();
        f[1]->halfedge() = h[0];
        f[2]->halfedge() = h[6];
        f[0]->halfedge() = h[7];

        //                    next        ,twin         ,vertex         ,edge         ,face
        h[0 ]-> setNeighbors(h[8]             ,h[7]         ,h[0]->vertex() ,h[0]->edge() ,f[1]         );
        h[1 ]-> setNeighbors(h[9]             ,h[1]->twin() ,h[1]->vertex() ,h[1]->edge() ,f[2]         );
        h[3 ]-> setNeighbors(h[7]             ,h[6]         ,h[3]->vertex() ,e[1]         ,h[3]->face() );
        h[7 ]-> setNeighbors(v[0]->halfedge() ,h[0]         ,v[3]           ,e[0]         ,f[0]         );
        h[6 ]-> setNeighbors(h[1]             ,h[3]         ,v[3]           ,e[1]         ,f[2]         );
        h[8 ]-> setNeighbors(h[2]             ,h[9]         ,v[3]           ,e[2]         ,f[1]         );
        h[9 ]-> setNeighbors(h[6]             ,h[8]         ,v[2]           ,e[2]         ,f[2]         );

        return v[3];
      }

      // split non-boundary
      {
        HalfedgeIter h[12];

        h[0] = e0->halfedge();
        h[1] = h[0]->next();
        h[2] = h[1]->next();
        h[3] = h[0]->twin();
        h[4] = h[3]->next();
        h[5] = h[4]->next();
        // new
        for(int i=6;i<12;i++){
          h[i] = newHalfedge();
        }

        VertexIter v[5];
        v[0] = h[0]->vertex();
        v[1] = h[1]->vertex();
        v[2] = h[2]->vertex();
        v[3] = h[5]->vertex();
        // new
        v[4] = newVertex();
        v[4]->isNew = true;
        // assign
        v[4]->halfedge() = h[0];
        v[4]->position = (v[0]->position + v[1]->position)/2;

        FaceIter f[4];
        f[0] = h[0]->face();
        f[1] = h[3]->face();
        // new
        for(int i = 2;i<4;i++){
          f[i] = newFace();
        }
        // assign
        f[2]->halfedge() = h[2];
        f[3]->halfedge() = h[5];

        EdgeIter e[4];
        e[0] = e0;                e[0]->isNew = false;
        for(int i=1;i<4;i++){
          e[i] = newEdge();
        }
        // assign
        e[1]->halfedge() = h[3];  e[1]->isNew = false;
        e[2]->halfedge() = h[6];  e[2]->isNew = true;
        e[3]->halfedge() = h[11]; e[3]->isNew = true;

        //                    next        ,twin         ,vertex         ,edge         ,face
        h[0 ]-> setNeighbors(h[0]->next() ,h[9]         ,v[4]           ,h[0]->edge() ,f[0]   );
        h[1 ]-> setNeighbors(h[6]         ,h[1]->twin() ,h[1]->vertex() ,h[1]->edge() ,f[0]   );
        h[2 ]-> setNeighbors(h[8]         ,h[2]->twin() ,h[2]->vertex() ,h[2]->edge() ,f[2]   );
        h[3 ]-> setNeighbors(h[3]->next() ,h[8]         ,v[4]           ,e[1]         ,f[1]   );
        h[4 ]-> setNeighbors(h[11]        ,h[4]->twin() ,h[4]->vertex() ,h[4]->edge() ,f[1]   );
        h[5 ]-> setNeighbors(h[9]         ,h[5]->twin() ,h[5]->vertex() ,h[5]->edge() ,f[3]   );
        h[6 ]-> setNeighbors(h[0]         ,h[7]         ,v[2]           ,e[2]         ,f[0]   );
        h[7 ]-> setNeighbors(h[2]         ,h[6]         ,v[4]           ,e[2]         ,f[2]   );
        h[8 ]-> setNeighbors(h[7]         ,h[3]         ,v[0]           ,e[1]         ,f[2]   );
        h[9 ]-> setNeighbors(h[10]        ,h[0]         ,v[1]           ,e[0]         ,f[3]   );
        h[10]-> setNeighbors(h[5]         ,h[11]        ,v[4]           ,e[3]         ,f[3]   );
        h[11]-> setNeighbors(h[3]         ,h[10]        ,v[3]           ,e[3]         ,f[1]   );

        f[0]->halfedge() = h[1];
        f[1]->halfedge() = h[4];
        e0->halfedge() = h[0];

        if(!v[0]->isBoundary()) v[0]->halfedge() = h[4];
        if(!v[1]->isBoundary()) v[1]->halfedge() = h[1];
        if(!v[2]->isBoundary()) v[2]->halfedge() = h[2];
        if(!v[3]->isBoundary()) v[3]->halfedge() = h[5];

  			return v[4];
      }
	 }

   VertexIter HalfedgeMesh::collapseEdge( EdgeIter e0 ){
     HalfedgeMesh::collapseEdge(e0,false);
   }

   VertexIter HalfedgeMesh::collapseEdge( EdgeIter e0, bool ds )
   {
     // TODO This method should collapse the given edge and return an iterator to the new vertex created by the collapse.
     // check
     if(e0->halfedge()->face()->isBoundary()||e0->halfedge()->twin()->face()->isBoundary()
     ||e0->halfedge()->next()->twin()->face()->isBoundary()||e0->halfedge()->next()->next()->twin()->face()->isBoundary()
     ||e0->halfedge()->twin()->next()->twin()->face()->isBoundary()||e0->halfedge()->twin()->next()->next()->twin()->face()->isBoundary()){
        std::cerr << "Cannot collapse a boundary." << std::endl;
        return VertexIter();
     }

     HalfedgeIter h[10];

     h[0] = e0->halfedge();
     h[1] = h[0]->next();
     h[2] = h[1]->next();
     h[3] = h[0]->twin();
     h[4] = h[3]->next();
     h[5] = h[4]->next();
     h[6] = h[1]->twin();
     h[7] = h[2]->twin();
     h[8] = h[4]->twin();
     h[9] = h[5]->twin();
     // check
     if(h[6]->next()==h[9]&&h[8]->next()==h[7]&&h[7]->next()==h[9]->next()->twin()){
       std::cerr << "Cannot collapse an edge of a tetrahedral." << std::endl;
       return VertexIter();
     }

     VertexIter v[4];
     v[0] = h[0]->vertex();
     v[1] = h[1]->vertex();
     v[2] = h[2]->vertex();
     v[3] = h[5]->vertex();
     // check
     if(v[2]->degree()<=3||v[3]->degree()<=3){
        std::cerr << "Cannot collapse this edge. (points degree <= 3)" << std::endl;
        return VertexIter();
     }

     {
       HalfedgeIter h_pointer = v[0]->halfedge();
       do{
         if(h_pointer->twin()->vertex()!=v[1]&&h_pointer->twin()->vertex()!=v[2]&&h_pointer->twin()->vertex()!=v[3]){
           HalfedgeIter hp = h_pointer->twin();
           do{
             if(hp->twin()->vertex()==v[1]){
               std::cerr << "Cannot collapse this edge. (duplicate neighbor)" << std::endl;
               return VertexIter();
             }
             hp = hp->twin()->next();
           }while(hp != h_pointer->twin());
         }
         h_pointer = h_pointer->twin()->next();
       }while(h_pointer!=v[0]->halfedge());
     }


     v[0]->position = (v[0]->position + v[1]->position)/2;

     EdgeIter e[5];
     e[0] = e0;
     e[1] = h[1]->edge();
     e[2] = h[2]->edge();
     e[3] = h[4]->edge();
     e[4] = h[5]->edge();

    //  erQueue.remove(e[0]);
     if(ds){
       for(int i=0;i<5;i++){
         erQueue.remove(e[i]);
       }
     }

     FaceIter f[2];
     f[0] = h[0]->face();
     f[1] = h[3]->face();

     // setNeighbors
     h[6]->setNeighbors(h[6]->next() ,h[7]     ,h[6]->vertex() ,e[1]      ,h[6]->face());
     h[7]->setNeighbors(h[7]->next() ,h[6]     ,v[0]           ,e[1]      ,h[7]->face());
     h[8]->setNeighbors(h[8]->next() ,h[9]     ,h[8]->vertex() ,e[4]      ,h[8]->face());
     h[9]->setNeighbors(h[9]->next() ,h[8]     ,v[0]           ,e[4]      ,h[9]->face());
     v[0]->halfedge() = h[9];
     v[2]->halfedge() = h[6];
     v[3]->halfedge() = h[8];
     e[1]->halfedge() = h[6]; e[1]->isNew = true;
     e[4]->halfedge() = h[9]; e[4]->isNew = true;

     // reassign every neighbors
     HalfedgeIter h_pointer;

     h_pointer = h[6]->next();
     while(h_pointer!=h[7]){
       if(ds&&h_pointer->edge()!=e[1]&&h_pointer->edge()!=e[2]&&h_pointer->edge()!=e[3]&&h_pointer->edge()!=e[4]){
         erQueue.remove(h_pointer->edge()->record);
       }
       h_pointer->setNeighbors(h_pointer->next(),h_pointer->twin(),v[0],h_pointer->edge(),h_pointer->face());
       h_pointer->edge()->isNew = false;
        h_pointer = h_pointer->twin()->next();
     }

     // delete
     deleteEdge(e[0]);
     deleteEdge(e[2]);
     deleteEdge(e[3]);
     for(int i = 1;i<2;i++){
       deleteVertex(v[i]);
     }
     for(int i = 0;i<2;i++){
       deleteFace(f[i]);
     }
     for(int i = 0;i<6;i++){
       deleteHalfedge(h[i]);
     }
     return v[0];
   }

   EdgeIter HalfedgeMesh::flipEdge( EdgeIter e0 )
   {
      // TODO This method should flip the given edge and return an iterator to the flipped edge.
      // std::cout<<"flipEdge"<<std::endl;
      // check
      if(e0->halfedge()->face()->isBoundary()||e0->halfedge()->twin()->face()->isBoundary()){
        std::cerr << "Cannot flip a boundary." << std::endl;
        return e0;
      }

      HalfedgeIter h[6];

      h[0] = e0->halfedge();
      h[1] = h[0]->next();
      h[2] = h[1]->next();
      h[3] = h[0]->twin();
      h[4] = h[3]->next();
      h[5] = h[4]->next();

      VertexIter v[4];
      v[0] = h[0]->vertex();
      v[1] = h[1]->vertex();
      v[2] = h[2]->vertex();
      v[3] = h[5]->vertex();
      // check
      for(int i=0;i<2;i++){
        if(v[i]->degree() <= 3){
            std::cerr << "Cannot flip this edge. (endpoints degree <= 3)" << std::endl;
            return e0;
        }
      }

      // check
      HalfedgeIter h_pointer = v[3]->halfedge();
      do{
        if(h_pointer->twin()->vertex()==v[2]){
          std::cerr << "Cannot flip this edge. (object edge already existed)" << std::endl;
         //  throw EdgeEditException(30);
          return e0;
        }
        h_pointer = h_pointer->twin()->next();
      }while(h_pointer!=v[3]->halfedge());

      FaceIter f[2];
      f[0] = h[0]->face();
      f[1] = h[3]->face();

      h[0]->setNeighbors(h[5],h[0]->twin(),v[2],h[0]->edge(),f[0]);
      h[3]->setNeighbors(h[2],h[3]->twin(),v[3],h[3]->edge(),f[1]);

      h[5]->setNeighbors(h[1],h[5]->twin(),h[5]->vertex(),h[5]->edge(),f[0]);
      h[2]->setNeighbors(h[4],h[2]->twin(),h[2]->vertex(),h[2]->edge(),f[1]);

      h[1]->setNeighbors(h[0],h[1]->twin(),h[1]->vertex(),h[1]->edge(),f[0]);
      h[4]->setNeighbors(h[3],h[4]->twin(),h[4]->vertex(),h[4]->edge(),f[1]);

      f[0]->halfedge() = h[1];
      f[1]->halfedge() = h[4];
      e0->halfedge() = h[0];

      v[0]->halfedge() = h[4];
      v[1]->halfedge() = h[1];
      v[2]->halfedge() = h[2];
      v[3]->halfedge() = h[5];

			return e0;
   }

   void MeshResampler::upsample( HalfedgeMesh& mesh )
   // This routine should increase the number of triangles in the mesh using Loop subdivision.
   {
      // Each vertex and edge of the original surface can be associated with a vertex in the new (subdivided) surface.
      // Therefore, our strategy for computing the subdivided vertex locations is to *first* compute the new positions
      // using the connectity of the original (coarse) mesh; navigating this mesh will be much easier than navigating
      // the new subdivided (fine) mesh, which has more elements to traverse.  We will then assign vertex positions in
      // the new mesh based on the values we computed for the original mesh.


      // TODO Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule,
      // TODO and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being
      // TODO a vertex of the original mesh.
      int countv = 0;
      for(VertexIter v = mesh.verticesBegin();v!=mesh.verticesEnd();v++){
        countv++;
        v->isNew=false;

        double myWeight,uWeight;
        if(v->degree()==3){
          uWeight = 3.0/16.0;
        }
        else{
          uWeight = 3.0/(8.0 * double(v->degree()));
        }

        myWeight = 1.0 - uWeight * double(v->degree());

        HalfedgeIter starth = v->halfedge();
        HalfedgeIter h = starth;

        Vector3D sumv = Vector3D(0,0,0);

        do{
          sumv += h->twin()->vertex()->position;
          h = h->next()->next()->twin();
        }while(h!=starth);

        v->newPosition = uWeight * sumv  + myWeight * v->position;

      } // end for

      // TODO Next, compute the updated vertex positions associated with edges, and store it in Edge::newPosition.
      list<EdgeIter> oeList;
      int counte = 0;
      for(EdgeIter e = mesh.edgesBegin();e!=mesh.edgesEnd();e++){
        e->isNew = false;
        oeList.push_back(e);
        double uWeight = 3.0 / 8.0;
        e->newPosition = ( e->halfedge()->vertex()->position + e->halfedge()->twin()->vertex()->position ) * uWeight;
        uWeight = 1.0/8.0;
        if(!e->halfedge()->face()->isBoundary()){
          e->newPosition += e->halfedge()->next()->twin()->vertex()->position * uWeight;
        }
        if(!e->halfedge()->twin()->face()->isBoundary()){
          e->newPosition += e->halfedge()->twin()->next()->twin()->vertex()->position * uWeight;
        }
        counte++;
      }

      // TODO Next, we're going to split every edge in the mesh, in any order.  For future
      // TODO reference, we're also going to store some information about which subdivided
      // TODO edges come from splitting an edge in the original mesh, and which edges are new,
      // TODO by setting the flat Edge::isNew.  Note that in this loop, we only want to iterate
      // TODO over edges of the original mesh---otherwise, we'll end up splitting edges that we
      // TODO just split (and the loop will never end!)
      while(!oeList.empty()){
        EdgeIter e = oeList.back();
        VertexIter nv;
        nv = mesh.splitEdge(e);
        nv->newPosition = e->newPosition;
        oeList.pop_back();
      }

      // TODO Now flip any new edge that connects an old and new vertex.
      counte = 0;
      for(EdgeIter e = mesh.edgesBegin();e!=mesh.edgesEnd();e++){
        if(e->isNew && (e->halfedge()->vertex()->isNew ^ e->halfedge()->twin()->vertex()->isNew)){
            mesh.flipEdge(e);
        }
        counte++;
      }

      countv = 0;
      // TODO Finally, copy the new vertex positions into final Vertex::position.
      for(VertexIter v = mesh.verticesBegin();v!=mesh.verticesEnd();v++){
        countv++;
        v->position = v->newPosition;
      }
   }

   // Given an edge, the constructor for EdgeRecord finds the
   // optimal point associated with the edge's current quadric,
   // and assigns this edge a cost based on how much quadric
   // error is observed at this optimal point.
   EdgeRecord::EdgeRecord( EdgeIter& _edge )
   : edge( _edge )
   {
      // TODO Compute the combined quadric from the edge endpoints.
      Matrix4x4 combQ = _edge->halfedge()->vertex()->quadric + _edge->halfedge()->twin()->vertex()->quadric;

      // TODO Build the 3x3 linear system whose solution minimizes
      // the quadric error associated with these two endpoints.
      Matrix3x3 A;
      for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
          A(i,j) = combQ(i,j);
        }
      }

      Vector3D b = Vector3D(-combQ(0,3),-combQ(1,3),-combQ(2,3));

      // TODO Use this system to solve for the optimal position, and
      // TODO store it in EdgeRecord::optimalPoint.
      Vector3D x = A.inv() * b;
      optimalPoint = x;

      // TODO Also store the cost associated with collapsing this edge
      // TODO in EdgeRecord::Cost.
      VertexIter v[4];
      v[0] = _edge->halfedge()->vertex();
      v[1] = _edge->halfedge()->twin()->vertex();
      v[2] = _edge->halfedge()->next()->twin()->vertex();
      v[3] = _edge->halfedge()->twin()->next()->twin()->vertex();
      for(int i=2;i<4;i++){
        if(v[i]->degree()<=3){
            score = numeric_limits<double>::max();
        }
      }

      {
        HalfedgeIter h_pointer = v[0]->halfedge();
        do{
          if(h_pointer->twin()->vertex()!=v[1]&&h_pointer->twin()->vertex()!=v[2]&&h_pointer->twin()->vertex()!=v[3]){
            HalfedgeIter hp = h_pointer->twin();
            do{
              if(hp->twin()->vertex()==v[1]){
                score = numeric_limits<double>::max();
              }
              hp = hp->twin()->next();
            }while(hp != h_pointer->twin());
          }
          h_pointer = h_pointer->twin()->next();
        }while(h_pointer!=v[0]->halfedge());
      }

      Vector4D xh = Vector4D(x.x,x.y,x.z,1);
      score = dot(combQ*xh, xh);

      edge = _edge;
   }


   void MeshResampler::downsample( HalfedgeMesh& mesh )
   {
     int _nFaces = mesh.nFaces();
     int _nVertices = mesh.nVertices();
     int restFaces = _nFaces/2;

     while (_nFaces>restFaces){
       // TODO Compute initial quadrics for each face by simply writing the plane
       // equation for the face in homogeneous coordinates.  These quadrics should
       // be stored in Face::quadric
       for(FaceIter f = mesh.facesBegin();f!=mesh.facesEnd();f++){
         Vector3D p = f->halfedge()->vertex()->position;
         Vector3D n = f->normal();
         double d = dot(-n,p);
         Vector4D v = Vector4D(n.x,n.y,n.z,d);
         f->quadric = outer(v,v);
       }

       // TODO Compute an initial quadric for each vertex as the sum of the quadrics
       // associated with the incident faces, storing it in Vertex::quadric
       for(VertexIter v = mesh.verticesBegin();v!=mesh.verticesEnd();v++){
         HalfedgeIter h = v->halfedge();
         v->quadric.zero();
         do{
           v->quadric += h->face()->quadric;
           h = h->next()->next()->twin();
         }while(h!=v->halfedge()&&!h->isBoundary());

         if(h->isBoundary()){
           h = v->halfedge()->twin();
           do{
             v->quadric += h->face()->quadric;
             h = h->next()->twin();
           }while(h!=v->halfedge()->twin()&&!h->isBoundary());
         }
       }

       // TODO Build a priority queue of edges according to their quadric error cost,
       // TODO i.e., by building an EdgeRecord for each edge and sticking it in the queue.
       erQueue.clear();
       for(EdgeIter e = mesh.edgesBegin();e!=mesh.edgesEnd();e++){
         EdgeRecord er;
         er = EdgeRecord(e);
         e->record = er;
         erQueue.insert(er);
       }

       // TODO Until we reach the target edge budget, collapse the best edge.  Remember
       // TODO to remove from the queue any edge that touches the collapsing edge BEFORE
       // TODO it gets collapsed, and add back into the queue any edge touching the collapsed
       // TODO vertex AFTER it's been collapsed.  Also remember to assign a quadric to the
       // TODO collapsed vertex, and to pop the collapsed edge off the top of the queue.
       _nFaces = mesh.nFaces();
       _nVertices = mesh.nVertices();

       int reduceFaces = 0;
       if(_nFaces<=4){
         std::cerr<<"Cannot downsample anymore."<<std::endl;
         return;
       }
       else if (_nFaces<1000){
         reduceFaces = _nFaces/10+1;
       }
       else{
         reduceFaces = 10;
       }

       while(reduceFaces>0){
         reduceFaces--;
         // Get the cheapest edge from the queue.
         EdgeRecord er= erQueue.top();

         EdgeIter e = er.edge;
         VertexIter vo = e->halfedge()->vertex();

         // Remove the cheapest edge from the queue by calling pop().
         erQueue.pop();

         // Compute the new quadric by summing the quadrics at its two endpoints.
         Matrix4x4 newQ = e->halfedge()->vertex()->quadric + e->halfedge()->twin()->vertex()->quadric;

         // Remove any edge touching either of its endpoints from the queue. (Has been done in collapse)
         // Collapse the edge.
         VertexIter vn = mesh.collapseEdge(e,true);
         if(vn!=vo){
           std::cerr<<"Cannot downsample anymore."<<std::endl;
           return;
         }

         // Set the quadric of the new vertex to the quadric computed in Step 2.
         vn->quadric = newQ;

         // Insert any edge touching the new vertex into the queue, creating new edge records for each of them.
         HalfedgeIter h = vn->halfedge();
         do{
           EdgeRecord inse;
           inse = EdgeRecord(h->edge());
           erQueue.insert(inse);
           h = h->next()->next()->twin();
         }while(h!=vn->halfedge()&&!h->face()->isBoundary());

         if(h->face()->isBoundary()){
           h = vn->halfedge()->twin()->next()->twin();
           do{
             EdgeRecord inse;
             inse = EdgeRecord(h->edge());
             erQueue.insert(inse);
             h = h->next()->twin();
           }while(h!=vn->halfedge()&&!h->face()->isBoundary());
         }
       }
     }
}

   void Vertex::computeCentroid( void )
   {
      // TODO Compute the average position of all neighbors of this vertex, and
      // TODO store it in Vertex::centroid.  This value will be used for resampling.
      HalfedgeIter h = this->halfedge();
      Vector3D sum = Vector3D(0,0,0);
      int count = 0;

      do{
        ++count;
        sum += h->twin()->vertex()->position;
        h = h->next()->next()->twin();
      }while(h!=this->halfedge()&&!h->face()->isBoundary());

      if(h->face()->isBoundary()){
        h = this->halfedge()->twin()->next();
        do{
          ++count;
          sum += h->twin()->vertex()->position;
          h = h->twin()->next();
        }while(h!=this->halfedge()&&!h->face()->isBoundary());
      }
      this->centroid = sum / double(count);
   }

   Vector3D Vertex::normal( void ) const
   // TODO Returns an approximate unit normal at this vertex, computed by
   // TODO taking the area-weighted average of the normals of neighboring
   // TODO triangles, then normalizing.
   {
      HalfedgeCIter h = this->halfedge();
      Vector3D sum = Vector3D(0,0,0);

      // TODO Compute and return the area-weighted unit normal.
      vector<Vector3D> normal;
      vector<double> area;
      do{
        Vector3D triVV[3];
        triVV[0] = h->vertex()->position;
        triVV[1] = h->next()->vertex()->position;
        triVV[2] = h->next()->next()->vertex()->position;

        // normal
        Vector3D triEV[2];
        triEV[0] = triVV[1]-triVV[0];
        triVV[1] = triVV[2]-triVV[1];

        normal.push_back(cross(triEV[0], triVV[1]));

        // area-weighted
        double triEL[3];
        triEL[0] = h->edge()->length();
        triEL[1] = h->next()->edge()->length();
        triEL[2] = h->next()->next()->edge()->length();
        // std::cout<<"length: "<<triEL[0]<<" "<<triEL[1]<<" "<<triEL[2]<<std::endl;
        double s = (triEL[0] + triEL[1] + triEL[2])/2.0;
        area.push_back(sqrt(s * (s*triEL[0]) * (s*triEL[1]) * (s*triEL[2])));

        h = h->next()->next()->twin();

      }while(h!=this->halfedge());

      int count;
      double areaSum = 0.0;
      for(count = 0;count<area.size();count++){
        areaSum += area[count];
      }

      while(!normal.empty()){
        sum += normal.back() * (area.back() / areaSum);
        normal.pop_back();
        area.pop_back();
      }
      sum.normalize();

      return sum;
	 }
   //
   void MeshResampler::resample( HalfedgeMesh& mesh )
   {
      // TODO Repeat the four main steps for 5 or 6 iterations
      list<EdgeIter> edgeList;
      int repeatTimes = 6;
      int smoothTimes = 10;
      if(mesh.nFaces()>10000){
        repeatTimes = 5;
        smoothTimes = 10;
      }
      for(int rmain = 0; rmain<repeatTimes;rmain++){
        // TODO Compute the mean edge length.
        double L = 0.0;
        for(EdgeIter e = mesh.edgesBegin();e!=mesh.edgesEnd();e++){
          L += e->length();
          e->isNew = false;
        }
        L /= mesh.nEdges();

        double splitL = 4.0 * L / 3.0;
        double collapseL = 4.0 * L / 5.0;

        // TODO Split edges much longer than the target length (being careful about how the loop is written!)
        edgeList.clear();
        for(EdgeIter e = mesh.edgesBegin();e!=mesh.edgesEnd();e++){
          if(e->length()>splitL){
            edgeList.push_back(e);
          }
        }
        while(!edgeList.empty()){
          EdgeIter e = edgeList.back();
          edgeList.pop_back();
          mesh.splitEdge(e);
        }

        // // TODO Collapse edges much shorter than the target length.  Here we need to be EXTRA careful about
        // // TODO advancing the loop, because many edges may have been destroyed by a collapse (which ones?)
        edgeList.clear();
        for(EdgeIter e = mesh.edgesBegin();e!=mesh.edgesEnd();e++){
          edgeList.push_back(e);
        }

        while(!edgeList.empty()){
          EdgeIter e = edgeList.back();
          edgeList.pop_back();
          if(e->length()<collapseL){
              VertexIter vo = e->halfedge()->vertex();
              EdgeIter e2 = e->halfedge()->next()->next()->edge();
              EdgeIter e3 = e->halfedge()->twin()->next()->edge();
              VertexIter vn = mesh.collapseEdge(e);
              edgeList.remove(e2);
              edgeList.remove(e3);
          } // if(e->length()<collapseL)
        } // while(!edgeList.empty())

        // TODO Now flip each edge if it improves vertex degree
        edgeList.clear();
        for(EdgeIter e = mesh.edgesBegin();e!=mesh.edgesEnd();e++){
          edgeList.push_back(e);
        }

        while(!edgeList.empty()){

          EdgeIter e = edgeList.back();
          edgeList.pop_back();

          int a1 = e->halfedge()->vertex()->degree();
          int a2 = e->halfedge()->twin()->vertex()->degree();
          int b1 = e->halfedge()->next()->twin()->vertex()->degree();
          int b2 = e->halfedge()->twin()->next()->twin()->vertex()->degree();

          int oldD, newD;
          oldD = abs(a1-6) + abs(a2-6) + abs(b1-6) + abs(b2-6);
          newD = abs(a1-1-6) + abs(a2-1-6) + abs(b1+1-6) + abs(b2+1-6);

          if(newD<oldD){
            mesh.flipEdge(e);
          }
        } // while(!edgeList.empty())
        edgeList.clear();

        // TODO Finally, apply some tangential smoothing to the vertex positions
        double w = 0.2;
        for(int rsmooth = 0; rsmooth<smoothTimes; rsmooth++){
          for(VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++){
             v->computeCentroid();
          }
          for(VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++){
            Vector3D n = v->normal();
            Vector3D dv = v->centroid - v->position;

            Vector3D temp = dot(n,dv) * n;

            v->position += w * (dv - temp);
          }
        } // end 20 time's loop
      } // for: Repeat the four main steps for 5 or 6 iterations
   }
}
