#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include <cstdlib>

#include <chrono>

int main(int argc, char *argv[]) {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec3() * random_vec3();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec3(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;


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