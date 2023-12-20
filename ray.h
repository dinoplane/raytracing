#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

using point3 = glm::dvec3;
using vec3 = glm::dvec3;

class ray {
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        point3 origin() const { return orig; }
        vec3 direction() const { return dir; }

        point3 at(double t) const {
            return orig + dir * t;
        }

        private:
            point3 orig;
            vec3 dir;
    };
#endif