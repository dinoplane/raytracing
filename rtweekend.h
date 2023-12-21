#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>



#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;
using vec3 = glm::dvec3;


// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}


inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline vec3 random_vec3() {
    return vec3(random_double(), random_double(), random_double());
}

inline vec3 random_vec3(double min, double max) {
    return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}


inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random_vec3(-1,1);
        if (glm::length2(p) < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return glm::normalize(random_in_unit_sphere());
}


inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

bool near_zero(const vec3& v) {
    // Return true if the vector is close to zero in all dimensions.

    return glm::all(glm::epsilonEqual(v, glm::dvec3(0.0), 1e-8));
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "hittable.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#endif