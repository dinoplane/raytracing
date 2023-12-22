#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "material.h"
#include "hittable.h"
#include <vector>
#include <memory>
#include <thread>
#include <iostream>

class camera {
  public:

    // const unsigned int NUM_THREADS = 16;

    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width  = 100;  // Rendered image width in pixel count
    int    samples_per_pixel = 10;   // Count of random samples for each pixel
    int    max_depth         = 10;   // Maximum number of ray bounces into scene

    double vfov = 90;  // Vertical view angle (field of view)
    point3 lookfrom = point3(0,0,-1);  // Point camera is looking from
    point3 lookat   = point3(0,0,0);   // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    shared_ptr<std::vector<color>> image_buffer;

    // void compute_pixel(int image_width, int start_j, int end_j, const hittable& world){
    //     for (int j = start_j; j < end_j; ++j) {
    //         std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
    //         for (int i = 0; i < image_width; ++i) {
    //             color pixel_color(0,0,0);
    //             for (int sample = 0; sample < samples_per_pixel; ++sample) {
    //                 ray r = get_ray(i, j);
    //                 pixel_color += ray_color(r, max_depth, world);
    //             }
    //             (*image_buffer)[j*image_width+i] = pixel_color;
    //         }
    //     }
    // }

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }
    }

    void render_multithread(const hittable& world, unsigned int NUM_THREADS) {
        initialize();
        const int CHUNK_SIZE = std::max((image_height + (NUM_THREADS - 1)) / NUM_THREADS, (unsigned int) 1);

        std::thread *thread_array = new std::thread[NUM_THREADS];
        for (unsigned int tid = 0; tid < NUM_THREADS; ++tid) {
            int start = CHUNK_SIZE * tid;
            int end = std::min(start + CHUNK_SIZE, image_height);
            thread_array[tid] =
                    std::thread(&camera::render_part, this, start, end, std::ref(world));
        }

        for (unsigned int tid = 0; tid < NUM_THREADS; ++tid) {
            thread_array[tid].join();
        }
    }

    void render_part(int j_start, int j_end, const hittable& world) {
        for (int j = j_start; j < j_end; ++j) {
            // std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                (*image_buffer)[j*image_width+i] = pixel_color;
            }
        }
    }

    void print_output(){
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height*image_width; ++j) {
            // std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            write_color(std::cout, (*image_buffer)[j], samples_per_pixel);
        }
        std::clog << "\rDone.                 \n";
    }



  private:
    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    vec3   u, v, w;        // Camera frame basis vectors

    vec3   defocus_disk_u;  // Defocus disk horizontal radius
    vec3   defocus_disk_v;  // Defocus disk vertical radius

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        image_buffer = make_shared<std::vector<color>>();
        (*image_buffer).reserve(image_width*image_height);


        // Determine viewport dimensions.
        // auto focal_length = glm::length(lookfrom - lookat);
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(cross(vup, w));
        v = glm::cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge


        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / (double)image_width;
        pixel_delta_v = viewport_v / (double)image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2.0 - viewport_v/2.0;
        // auto viewport_upper_left = center - (focal_length * w) - viewport_u/2.0 - viewport_v/2.0;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }


    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);

        if (world.hit(r, interval(0.0001, infinity), rec)) {
            // vec3 direction = random_on_hemisphere(rec.normal); // uniform distribution
            // vec3 direction = rec.normal + random_unit_vector();  // lambertian distribution
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
            // return 0.5 * ray_color(ray(rec.p, direction), depth-1, world);
        }

        vec3 unit_direction = glm::normalize(r.direction());
        auto a = 0.5*(unit_direction.y + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5, 0.8, 1.0);
    }

    ray get_ray(int i, int j) const {
        // Get a randomly-sampled camera ray for the pixel at location i,j, originating from
        // the camera defocus disk.
        auto pixel_center = pixel00_loc + ((double) i * pixel_delta_u) + ((double) j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
};

#endif