#include "environment_light.h"

namespace CMU462 { namespace StaticScene {

EnvironmentLight::EnvironmentLight(const HDRImageBuffer* envMap)
    : envMap(envMap) {
  // TODO: initialize things here as needed

  flux.resize(envMap->h * envMap->w);
  ptheta.resize(envMap->h);
  pphitheta.resize(envMap->h * envMap->w);
  cdftheta.resize(envMap->h);
  cdfphitheta.resize(envMap->h * envMap->w);

  double dt = PI / double(envMap->h);

  double sum = 0.0;
  for(int y = 0; y<envMap->h; y++){
    double sinthetha = sin( y * dt );
    for(int x = 0; x<envMap->w; x++){
      flux[ x + y * envMap->w ] = envMap->data[ x + y * envMap->w ].illum() * sinthetha;
      sum += flux[ x + y * envMap->w ];
    }
  }

  // the flux through a pixel
  for(int y = 0; y<envMap->h; y++){
    for(int x = 0; x<envMap->w; x++){
      flux[ x + y * envMap->w ] *= 1.0 / sum;
    }
  }

  // P(theta) and P(phi|theta)
  for(int y = 0; y<envMap->h; y++){
    for(int x = 0; x<envMap->w; x++){
      ptheta[y] += flux[ x + y * envMap->w ];
    }
    for(int x = 0; x<envMap->w; x++){
      pphitheta[ x + y * envMap->w ] = flux[ x + y * envMap->w ] / ptheta[y];
    }
  }

  // CDF
  double sum_ptheta = 0;
  for (int y = 0; y < envMap->h; y++) {
    sum_ptheta += ptheta[y];
    cdftheta[y] = sum_ptheta;
    double sum_phi_theta = 0;
    for (int x = 0; x < envMap->w; x++) {
      sum_phi_theta += pphitheta[x + envMap->w * y];
      cdfphitheta[x + envMap->w * y] = sum_phi_theta;
    }
  }

}

Spectrum EnvironmentLight::sample_L(const Vector3D& p, Vector3D* wi,
                                    float* distToLight,
                                    float* pdf) const {
  // TODO: Implement
  *wi = UniformHemisphereSampler3D().get_sample();
  *pdf = 1.0 / (4.0 * PI);

  double Xi1 = (double)(std::rand()) / RAND_MAX;
  double Xi2 = (double)(std::rand()) / RAND_MAX;

  int x, y;
  for(int y = 0; y < envMap->h ; y++){
    if(Xi2 < cdftheta[y]){
      break;
    }
  }

  for(int x = 0; x < envMap->w ; x++){
    if(Xi1 < cdfphitheta[x + y * envMap->w]){
      break;
    }
  }

  double dt = PI / double(envMap->h);
  double dp = 2 * PI / double(envMap->w);

  double theta = double(y) * dt;
  double phi = double(x) * dp;

  double zs = sin(theta) * cos(phi);
  double xs = sin(theta) * sin(phi);
  double ys = cos(theta);

  *pdf = envMap->data[ x + y * envMap->w].illum() / (dp * dt * sin(theta));
  *wi = Vector3D(xs, ys, zs);

  *distToLight = std::numeric_limits<float>::infinity();
  Ray r = Ray(p,*wi);
  return this->sample_dir(r);

}

Spectrum EnvironmentLight::sample_dir(const Ray& r) const {
  // TODO: Implement
  double theta = acos(r.d.y);
  double phi = atan2( r.d.x , -r.d.z ) + PI;
  phi = (phi < 0) ? phi + 2 * M_PI : phi;

  double phi_ratio = phi / (2 * PI);
  double theta_ratio = theta / (PI);

  double x = double(envMap->w) * phi_ratio;
  double y = double(envMap->h) * theta_ratio;

  // Bilinear interpolation
  double xf = floor(x);
  double xc = ceil(x);
  double yf = floor(y);
  double yc = ceil(y);

  double x_ratio = x - xf;
  double y_ratio = y - yf;

  Spectrum temp1,temp2;
  Spectrum result;

  temp1 = envMap->data[xf + envMap->w * yf] * (1 - y_ratio) + envMap->data[xf + envMap->w * yc] * y_ratio;
  temp2 = envMap->data[xc + envMap->w * yf] * (1 - y_ratio) + envMap->data[xc + envMap->w * yc] * y_ratio;

  result = temp1 * (1 - x_ratio) + temp2 * (x_ratio);

  return result;
}

} // namespace StaticScene
} // namespace CMU462
