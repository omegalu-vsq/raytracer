#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "image.hh"
#include "light.hh"
#include "shape.hh"

namespace ray {
    class Scene {
    public:
        std::vector<std::unique_ptr<Shape>> objects_;
        std::vector<std::unique_ptr<Light>> sources_;
        Color ambilight_;

        Scene() : ambilight_({0, 0, 0}) {}
        ~Scene() = default;

        void addPointLight(Point3 pos, Color color, double power) {
            auto light = std::make_unique<PointLight>(color, pos, power);
            sources_.push_back(std::move(light));
        }

        void addObject(std::unique_ptr<Shape> shape) {
            objects_.push_back(std::move(shape));
        }

        void setAmbilight(Color color, double power) {
            if (power > 1.) {
                std::cerr << "/!\\ Ambilight power value has to be between 0 and 1, the calculation had been canceled.";
                power = 0.;
            }

			double r = static_cast<double>(color.r) / 255.;
			double g = static_cast<double>(color.g) / 255.;
			double b = static_cast<double>(color.b) / 255.;

            r *= power * 255.;
            g *= power * 255.;
            b *= power * 255.;
            
            //std::cout << "Ambilight(" << r << ", " << g << ", " << b << ")\n";
            std::cout << "Ambilight(" << std::round(r) << ", " << std::round(g) << ", " << std::round(b) << ")\n";

			ambilight_.r = static_cast<uint8_t>(std::round(r));
			ambilight_.g = static_cast<uint8_t>(std::round(g));
		    ambilight_.b = static_cast<uint8_t>(std::round(b));

            //std::cout << "Ambilight(" << int(ambilight_.r) << ", " << int(ambilight_.g) << ", " << int(ambilight_.b) << ")\n";
        }
        
    };
}
