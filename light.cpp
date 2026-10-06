#include "light.hh"

namespace ray {
    Light::Light(Color color, Point3 pos, double power) : color_(color), pos_(pos), power_(power)
    {}

    PointLight::PointLight(Color color, Point3 pos, double power) : Light(color, pos, power)
    {}

}
