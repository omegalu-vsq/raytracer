#pragma once
#include "texture.hh"
#include "point.hh"
#include "vector.hh"

#include <memory>

namespace ray {
    class Shape {
        public:
            Point3 center_;
            std::unique_ptr<TextureMaterial> material_;

            Shape(Point3 center, std::unique_ptr<TextureMaterial> material);

            virtual double intersection(Point3 origin, Vector3 dir) = 0;
            virtual Vector3 compute_normal(Point3 pt) = 0;
            virtual surface_rapport get_material(Point3 pt) = 0;
    };

    class Sphere : public Shape {
        public:
            double radius_;

            Sphere(Point3 center, std::unique_ptr<TextureMaterial> material, double radius);

            double intersection(Point3 origin, Vector3 dir) override;
            Vector3 compute_normal(Point3 pt) override;
            surface_rapport get_material(Point3 pt) override;

    };

    /*
    class Triangle : public Shape {
        public :
            Point3 A, B, C;

            Triangle(Point3 points[], std::unique_ptr<TextureMaterial> material);

            double intersection(Point3 origin, Vector3 dir) override;
            Vector3 compute_normal(Point3 pt) override;
            surface_rapport get_material(Point3 pt) override;
    };
    */
}
