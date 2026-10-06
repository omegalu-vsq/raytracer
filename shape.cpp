#include "texture.hh"
#include "point.hh"
#include "vector.hh"
#include <cmath>
#include "shape.hh"

namespace ray {
    Shape::Shape(Point3 center, std::unique_ptr<TextureMaterial> material) : center_(center), material_(std::move(material))
    {}

    Sphere::Sphere(Point3 center, std::unique_ptr<TextureMaterial> material, double radius) : Shape(center, std::move(material)) {
        radius_ = radius;
    }

    double Sphere::intersection(Point3 origin, const Vector3 dir) {
        //std::cout << "vect(" << dir.x_ << ", " << dir.y_ << ", " << dir.z_ << ")\n";

        double a = dir * dir;
        double b = 2 * (dir * (origin - center_));
        double c = (origin - center_) * (origin - center_) - radius_*radius_;

        double disc = b*b - 4 * a * c;

        //std::cout << "disc:" << disc << std::endl;

        if (disc > 0) {
            //std::cout << "ouais" << std::endl;

            double rac1 = (-b + std::sqrt(disc)) / (2 * a);
            double rac2 = (-b - std::sqrt(disc)) / (2 * a);

            rac1 = rac1 >= 0 ? rac1 : -1;
            rac2 = rac2 >= 0 ? rac2 : -1;

            //std::cout << "rac1: " << rac1 << ", rac2: " << rac2 << std::endl;

            if (rac2 == -1)
                return rac1;
            if (rac1 == -1)
                return rac2;

            return rac1 > rac2 ? rac2 : rac1;
        }
        else
            return -1;
    }

    Vector3 Sphere::compute_normal(Point3 pt) {
        auto normal = pt - center_;
        normal.normalize();
        return normal;
    }

    surface_rapport Sphere::get_material(Point3 pt) {
        return material_->check_elements(pt);
    }

}
