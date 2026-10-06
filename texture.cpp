#include "texture.hh"

namespace ray {
    TextureMaterial::TextureMaterial(double kd, double ks, int ns, Color clr) : color(clr), kd(kd), ks(ks), ns(ns)
    {}

    UniformTexture::UniformTexture(double kd, double ks, int ns, Color clr) : TextureMaterial(kd, ks, ns, clr)
    {}

    surface_rapport UniformTexture::check_elements(Point3 pt) {
        (void)pt;
        surface_rapport rapport = {color, kd, ks, ns};
        return rapport;
    }
}

