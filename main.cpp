#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include <cstdlib>





color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = glm::normalize(r.direction());
    double a = 0.5*(unit_direction.y + 1.0);
    return (1.0 - a) * color(1.0) + a * color(0.5, 0.7, 1.0);
}

int main(int argc, char *argv[]) {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 1024;
    cam.samples_per_pixel = 100;

    if (argc > 1){
        int val = strtol(argv[1], NULL, 10);
        if (val > 1)
            cam.image_width = val;
    }

    cam.render(world);
}