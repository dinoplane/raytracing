#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include <cstdlib>



int main(int argc, char *argv[]) {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_mcenter   = make_shared<metal>(color(0.8, 0.8, 0.1), 0.0);
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);


    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_mcenter));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 1024;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 100;


    if (argc > 1){
        int val = strtol(argv[1], NULL, 10);
        if (val > 1)
            cam.image_width = val;
    }

    if (argc > 2){
        int val = strtol(argv[2], NULL, 10);
        if (val > 1)
            cam.samples_per_pixel = val;
    }

    cam.render(world);
}