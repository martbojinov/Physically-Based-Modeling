
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

// Modification to Sphere Shape to Implement RayMarching
// by Kevin M. Smith 3-2-2019
//


// shapes/sphere.cpp*
#include "shapes/raymarchedheightfield.h"
#include "sampling.h"
#include "paramset.h"
#include "efloat.h"
#include "stats.h"

namespace pbrt {

// Sphere Method Definitions
Bounds3f RayMarchedHeightField::ObjectBound() const {
    return Bounds3f(Point3f(INT_MIN, INT_MIN, INT_MIN),
                    Point3f(INT_MAX, INT_MAX, INT_MAX));
}


//  Note: create PBRT parameters for these
//

#define MAX_RAY_STEPS 1000
#define DIST_THRESHOLD .01
#define MAX_DISTANCE 100
#define NORMAL_EPS .001

static int counter = 0;

//  Template Method
//
bool RayMarchedHeightField::Intersect(const Ray &r, Float *tHit, SurfaceInteraction *isect, bool testAlphaTexture) const {
  /*
  // print count
  printf("%d\n", counter);
  counter++;
  */

  Vector3f dir = Normalize(r.d);  // ray direction vectors are not normalized in PBRT by default (KMS)
	bool hit = false;
  Point3f p = r.o;  // start point

  // ray marching algorithm from the slides
  for (int i=0; i < max_ray; i++) {
    float dist = this->sdf(p);
      if (dist < dist_thresh) {    // hit, return true
        hit = true;
        break;
      }
      else if (dist > max_dist) { // miss, return false
        break;
      }
      else {  // march
        p = p + (dir * dist);
      }
  }

	if (hit && tHit != nullptr && isect != nullptr) {
		// Thiis where you return your SurfaceInteraction structure and your tHit
		// Important Note: You must check for null pointer as Intersect is called
		// by IntersectP() with null values for these parameters.
    Vector3f default_norm = Vector3f(0, 0, 0);
    Vector3f norm = this->GetNormalRM(p, normal_eps, default_norm);

    Point3f pHit = p;
    Vector3f pError = Vector3f(10 * max_dist, 10 * max_dist, 10 * max_dist);
    Point2f uv = Point2f(0, 0);
    Normal3f dndu = Normal3f(0,0,0);
    Normal3f dndv = Normal3f(0,0,0);
    Vector3f dpdu;
    Vector3f dpdv;

    CoordinateSystem(norm, &dpdu, &dpdv);

    *isect = (*ObjectToWorld)(SurfaceInteraction(pHit, pError, uv, -r.d, dpdu, dpdv, dndu, dndv, r.time, this));

    // distance from ray start to hit point
    *tHit = (Float)Distance(r.o, p);
	}

  return hit;
}

// returns noise value in one-dimension
float OctaveNoise(Point3f p, int nOctaves, float freq, float amplitude ){
  float total = 0;

  for (int i = 1; i <= nOctaves; i++) {
    total += Noise(p * freq * i) * amplitude / i;
  }
  //total = total + Noise(p);

  return total;
}

//  Template Method
//
Float RayMarchedHeightField::sdf(const Point3f &pos) const {
  float y = OctaveNoise(pos, 4, 0.25, 0.3);
  // float d = Distance(pos, Point3f(pos.x, 0, pos.z));   // distance between plane and point (2d plane)
  float d1 = Distance(pos, Point3f(pos.x, y, pos.z));     // distance between plane and point (heigt field)
  float d2 = Distance(pos, Point3f(0, 0, 0)) - radius;    // sphere


  //return std::min(d1, d2);    // union
  //return std::max(d1, d2);      // intersection
  return std::max(d1, -d2);   // subtraction
  //return std::max(std::min(d1,d2),-std::max(d1,d2));  // xor

  // individual sdfs
  //return d1;  // height field
  //return d2;  // sphere
}

// Get Normal using Gradient (Finite Distances Methods )  - See class slides.
//  Note if the normal you calculate has zero length, return the defaultNormal
//
Vector3f RayMarchedHeightField::GetNormalRM( const Point3f &p, float eps, const Vector3f &defaultNormal) const {

  Point3f px = Point3f(p.x - eps, p.y, p.z);
  Point3f py = Point3f(p.x, p.y - eps, p.z);
  Point3f pz = Point3f(p.x, p.y, p.z - eps);
  float dp = this->sdf(p);
  float dx = this->sdf(px);
  float dy = this->sdf(py);
  float dz = this->sdf(pz);
  Vector3f norm = Vector3f(dp - dx, dp - dy, dp - dz);
  norm = Normalize(norm);

  if (norm.Length() == 0) {
    return defaultNormal;
  }
  else {
    return norm;
  }
}


Float RayMarchedHeightField::Area() const { return phiMax * radius * (zMax - zMin); }

// These functions are stubbed
//
Interaction RayMarchedHeightField::Sample(const Point2f &u, Float *pdf) const {
    LOG(FATAL) << "RayMarchedHeightField::Sample not implemented.";
    return Interaction();
}

Interaction RayMarchedHeightField::Sample(const Interaction &ref, const Point2f &u,
                           Float *pdf) const {
    LOG(FATAL) << "RayMarchedHeightField::Sample not implemented.";
    return Interaction();
}



std::shared_ptr<Shape> CreateRayMarchedHeightFieldShape(const Transform *o2w, const Transform *w2o, bool reverseOrientation, const ParamSet &params) {
    int   max_ray     = params.FindOneFloat("max ray", 1000);
    Float dist_thresh = params.FindOneFloat("distance threshold", 0.01f);
    int   max_dist    = params.FindOneFloat("max distance", 100);
    Float normal_eps  = params.FindOneFloat("normal eps", 0.0001);

    Float radius = params.FindOneFloat("radius", 1.f);
    Float zmin = params.FindOneFloat("zmin", -radius);
    Float zmax = params.FindOneFloat("zmax", radius);
    Float phimax = params.FindOneFloat("phimax", 360.f);
    return std::make_shared<RayMarchedHeightField>(o2w, w2o, reverseOrientation, radius, zmin, zmax, phimax, max_ray, dist_thresh, max_dist, normal_eps);
}

}  // namespace pbrt
