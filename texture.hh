#pragma once
#include "image.hh"
#include "vector.hh"
#include "point.hh"

namespace ray {
    typedef struct surfaceRapport {
        Color color;
        double kd, ks;
        int ns;
    } surface_rapport;

    class TextureMaterial {
        protected:
            Color color;
            double kd, ks;
            int ns;
        public:
            TextureMaterial(double kd, double ks, int ns, Color clr);
            virtual ~TextureMaterial() = default;

            virtual surface_rapport check_elements(Point3 pt) = 0;
    };

    class UniformTexture : public TextureMaterial {
        public:
            UniformTexture(double kd, double ks, int ns, Color clr);
            surface_rapport check_elements(Point3 pt) override;
    };
}
