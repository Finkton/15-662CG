#include "bsdf.h"

#include <iostream>
#include <algorithm>
#include <utility>

using std::min;
using std::max;
using std::swap;

namespace CMU462 {

void make_coord_space(Matrix3x3& o2w, const Vector3D& n) {

    Vector3D z = Vector3D(n.x, n.y, n.z);
    Vector3D h = z;
    if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) h.x = 1.0;
    else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) h.y = 1.0;
    else h.z = 1.0;

    z.normalize();
    Vector3D y = cross(h, z);
    y.normalize();
    Vector3D x = cross(z, y);
    x.normalize();

    o2w[0] = x;
    o2w[1] = y;
    o2w[2] = z;
}

// Diffuse BSDF //

Spectrum DiffuseBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return albedo * (1.0 / PI);
}

Spectrum DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *wi = this->sampler.get_sample(pdf);
  // *pdf = 1.f / (2.f * PI);
  return this->f(wo,*wi);
  // return Spectrum();
}

// Mirror BSDF //

Spectrum MirrorBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum MirrorBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO:
  // Implement MirrorBSDF
  reflect(wo,wi);
  *pdf = 1.f;
  return this->reflectance * (1.f / fabs(wo.z));
}

// Glossy BSDF //

/*
Spectrum GlossyBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}
Spectrum GlossyBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *pdf = 1.0f;
  return reflect(wo, wi, reflectance);
}
*/

// Refraction BSDF //

Spectrum RefractionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum RefractionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO:
  // Implement RefractionBSDF

  refract(wo,wi,this->ior);
  *pdf = 1;
  return this->transmittance;

  // return Spectrum();
}

// Glass BSDF //

Spectrum GlassBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlassBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO:
  // Compute Fresnel coefficient and either reflect or refract based on it.
  *pdf = 1.f;
  if (!refract(wo, wi, ior)) {
      reflect(wo, wi);
      return this->reflectance * (1.f / fabs(wo.z));
    }

    double ni, nt;

    if (wo.z > 0) {
      ni = 1;
      nt = ior;
    } else {
      ni = ior;
      nt = 1;
    }

    double cosi = fabs(wo.z);
    double cost = fabs(wi->z);

    double rpar = ((nt*cosi) - (ni*cost)) / ((nt*cosi) + (ni*cost));
    double rver = ((ni*cosi) - (nt*cost)) / ((ni*cosi) + (nt*cost));

    double fresnel = (rpar*rpar + rver*rver) / 2.f;

    if((double)(std::rand()) / RAND_MAX <= fresnel){
      reflect(wo, wi);
      return this->reflectance * (1.f / fabs(wo.z));
    }
    else{
      return transmittance * double((nt*nt)/(ni*ni) * (1-fresnel) / fabs(cosi));
    }


  // return Spectrum();
}

void BSDF::reflect(const Vector3D& wo, Vector3D* wi) {

  // TODO:
  // Implement reflection of wo about normal (0,0,1) and store result in wi.

  // wo = -wi + 2(dot(wi,Vector3D(0,0,1)))*(Vector3D(0,0,1))
  wi->x = -wo.x;
  wi->y = -wo.y;
  wi->z = wo.z;

}

bool BSDF::refract(const Vector3D& wo, Vector3D* wi, float ior) {

  // TODO:
  // Use Snell's Law to refract wo surface and store result ray in wi.
  // Return false if refraction does not occur due to total internal reflection
  // and true otherwise. When dot(wo,n) is positive, then wo corresponds to a
  // ray entering the surface through vacuum.

  // ior: the index of refraction (nt)

  double cosi = fabs(wo.z);
  double sini = sqrt(1 - cosi*cosi);
  double nitRate; // ni/nt

  if(wo.z>0){
    nitRate = 1.f / ior; // wi(nt) material(ior) -> wo(ni) vacuum
  }
  else{
    nitRate = ior; // wi(nt) vacuum -> wo(ni) material (ior)
  }

  if(1 - (nitRate * nitRate) * (1 - cosi * cosi) < 0 ){ // total internal reflection
    return false;
  }

  double sint = nitRate * sini;
  double cost = sqrt(1 - sint*sint);

  wi->x = - wo.x * sint / sini;
  wi->y = - wo.y * sint / sini;
  if(wo.z>0){
    wi->z = -cost;
  }
  else{
    wi->z = cost;
  }

  return true;

}

// Emission BSDF //

Spectrum EmissionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum EmissionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *wi  = sampler.get_sample(pdf);
  return Spectrum();
}

} // namespace CMU462
