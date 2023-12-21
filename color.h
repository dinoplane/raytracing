#ifndef COLOR_H
#define COLOR_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "interval.h"

#include <iostream>

using color = glm::dvec3;


inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}


void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    pixel_color *= scale;

    // Apply the linear to gamma transform.
    pixel_color.x = linear_to_gamma(pixel_color.x);
    pixel_color.y = linear_to_gamma(pixel_color.y);
    pixel_color.z = linear_to_gamma(pixel_color.z);

    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(pixel_color.x)) << ' '
        << static_cast<int>(256 * intensity.clamp(pixel_color.y)) << ' '
        << static_cast<int>(256 * intensity.clamp(pixel_color.z)) << '\n';
}

#endif