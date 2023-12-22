#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include <cstdlib>

#include <chrono>

int main(int argc, char *argv[]) {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<dielectric>(1.5, true);
    auto material_center = make_shared<dielectric>(1.5);

    auto material_mcenter   = make_shared<metal>(color(0.8, 0.8, 0.1), 0.0);
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);


    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   -0.4, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 1024;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 100;

    cam.vfov     = 20;
    cam.lookfrom = point3(-2,2,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 10.0;
    cam.focus_dist    = 3.4;

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

    int nthread = 16;
    if (argc > 3){
        int val = strtol(argv[3], NULL, 10);
        if (val > 0)
            nthread = val;
    }
    // timing variables

    auto start = std::chrono::high_resolution_clock::now();
    auto chkpt = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

// if (){
//     cam.render(world);

//     stop = std::chrono::high_resolution_clock::now();
//     duration = std::chrono::duration_cast<
//                 std::chrono::microseconds>(stop - chkpt);

//     std::clog << "\nTime taken by render(s): "
//         << duration.count() << " microseconds" << std::endl;
// }

{
    std::clog << nthread << "\n";
    chkpt = std::chrono::high_resolution_clock::now();
    cam.render_multithread(world, nthread);

    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<
                std::chrono::microseconds>(stop - chkpt);

    std::clog << "Time taken by render(m): "
        << duration.count() << " microseconds" << std::endl;
}
    cam.print_output();
}