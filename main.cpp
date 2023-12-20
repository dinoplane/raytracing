#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "color.h"
#include "ray.h"

float hit_sphere(const point3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
        return -1.0f;
    } else {
        return (-half_b - sqrt(discriminant) ) / (a);
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5f, r);
    if (t > 0.0f) {
        vec3 N = glm::normalize(r.at(t) - vec3(0,0,-1));
        return 0.5f*color(N.x+1, N.y+1.0f, N.z+1.0f);
    }


    vec3 unit_direction = glm::normalize(r.direction());
    float a = 0.5f*(unit_direction.y + 1.0f);
    return (1.0f - a) * color(1.0f) + a * color(0.5f, 0.7f, 1.0f);
}

int main() {
    // Image

    // Render

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 1000;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    auto camera_center = point3(0.0f, 0.0f, 0.0f);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = vec3(viewport_width, 0.0f, 0.0f);
    vec3 viewport_v = vec3(0, -viewport_height, 0.0f);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / float(image_width);
    auto pixel_delta_v = viewport_v / float(image_height);

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - vec3(0.0f, 0.0f, focal_length) - viewport_u * 0.5f - viewport_v * 0.5f;
    auto pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);



    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (float j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;

        for (float i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << std::endl;
    glm::vec3 position = glm::vec3(0.0);
    std::clog << "Position: " << glm::to_string(glm::vec4(position, 1.0)) << std::endl;

}