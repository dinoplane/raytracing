#ifndef COLOR_H
#define COLOR_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


#include <iostream>

using color = glm::dvec3;

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    // Write the translated [0,255] value of each color component.
    auto scale = 1.0 / samples_per_pixel;
    pixel_color *= scale;

    static const interval intensity(0.000, 0.999);


    out << static_cast<int>(256 * intensity.clamp(pixel_color.x)) << ' '
        << static_cast<int>(256 * intensity.clamp(pixel_color.y)) << ' '
        << static_cast<int>(256 * intensity.clamp(pixel_color.z)) << '\n';
}

#endif