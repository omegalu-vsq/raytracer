#pragma once
#include "image.hh"
#include "point.hh"

namespace ray {
    class Light {
        public:
            Color color_;
            Point3 pos_;
            double power_;

            Light(Color color, Point3 pos, double power);
            virtual ~Light() = default;
    };

    class PointLight : public Light {
        public:
            PointLight(Color color, Point3 pos, double power);
    };
}
