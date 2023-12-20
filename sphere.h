#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>



class sphere : public hittable {
  public:
    sphere(point3 _center, float _radius) : center(_center), radius(_radius) {}

    bool hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = glm::length2(r.direction());
        auto half_b = dot(oc, r.direction());
        auto c = glm::length2(oc) - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (-half_b + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

  private:
    point3 center;
    float radius;
};

#endif