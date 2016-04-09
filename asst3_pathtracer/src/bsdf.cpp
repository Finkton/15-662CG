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
  *pdf = 1;
  return this->reflectance;
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
  if (!refract(wo, wi, ior)) {
      reflect(wo, wi);
      *pdf = 1;

      return reflectance * (1 / fabs(wi->z));;
    }

    double ni, nt;

    if (wo.z > 0) {
      ni = 1;
      nt = ior;
    } else {
      ni = ior;
      nt = 1;
    }

    double fresnel;
    double r_par, r_per;

    double cosi = fabs(wo.z);
    double cost = fabs(wi->z);

    r_par = ((nt*cosi) - (ni*cost)) / ((nt*cosi) + (ni*cost));
    r_per = ((ni*cosi) - (nt*cost)) / ((ni*cosi) + (nt*cost));

    fresnel = 0.5 * (r_par*r_par + r_per*r_per);

    float rand = (float)(std::rand()) / RAND_MAX;
    bool refract = rand > ((float)fresnel);

    *pdf = refract ? 1-fresnel : fresnel;

    if (!refract) {
      reflect(wo, wi);
      return fresnel * reflectance * (1 / fabs(wi->z));
    }

    double trans_coeff = (1 - fresnel) * (nt / ni) * (nt / ni) / fabs(wi->z);

    return transmittance * trans_coeff;
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

  float cosi = wo.z;
  float sini = sqrt(1 - cosi*cosi);
  float nitRate; // ni/nt

  if(wo.z>0){
    nitRate = 1.f / ior; // wi(nt) material(ior) -> wo(ni) vacuum
  }
  else{
    nitRate = ior; // wi(nt) vacuum -> wo(ni) material (ior)
  }

  if(1 - (nitRate * nitRate) * (1 - cosi * cosi) < 0 ){ // total internal reflection
    return false;
  }

  float sint = nitRate * sini;
  float cost = sqrt(1 - sint*sint);

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
