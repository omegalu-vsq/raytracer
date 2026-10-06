#pragma once
#include <cstdint>
#include <vector>

#include "point.hh"
#include "scene.hh"
#include "vector.hh"

namespace ray {
    class Camera {
        Point3 pos_;
        Point3 vision_;
        Point3 vp_center_;
        Vector3 up_, side_, front_;
        double alpha_, beta_;
        double fov_;
        double height_, width_;
        double pixal_;

    public:
        Camera(Point3 pos, Point3 vision, double alpha, double beta, double fov_, uint16_t width);
        Camera(Point3 pos, Point3 vision, double fov_, uint16_t height);

        Point3 getPos() {
            return pos_;
        }

        std::vector<Point3> getPixels();

        friend class Engine;

    };

    class Engine {
        Camera camera_;
        Scene scene_;
        Image image_;

    public:
        Engine(Camera camera);

        const Camera& getCamera() {
            return camera_;
        }

        Scene& getScene() {
            return scene_;
        }

        Color raycast(Point3 ray_point, Vector3 ray_director, int depth);

        void run();
    };
}
