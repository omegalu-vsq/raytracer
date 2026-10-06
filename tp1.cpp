#include "camera.hh"
#include "image.hh"
#include "vector.hh"

using namespace ray;

int main(void) {
    /*
    Image img = Image(2, 2);
    img.pixels[0].r = 255;
    img.pixels[1].g = 255;
    img.pixels[2].b = 255;
    img.pixels[3].r = 255;
    img.pixels[3].g = 255;
    img.pixels[3].b = 255;

    img.save("ray.ppm");
    */

    auto camera = Camera({0, 0, 1}, {1, 0, 1}, 5, 1080);
    auto engine = Engine(std::move(camera));

    auto material = std::make_unique<UniformTexture>(0.7, 0.3, 7, Colors::JADE);
    auto sphere = std::make_unique<Sphere>(Point3{12, -10, 5}, std::move(material), 3);

    auto material2 = std::make_unique<UniformTexture>(0., 1., 3, Colors::DARK_BLUE);
    auto sphere2 = std::make_unique<Sphere>(Point3{20, 20, 10}, std::move(material2), 10);

    auto material3 = std::make_unique<UniformTexture>(0.7, 0.3, 7, Colors::RED);
    auto ground = std::make_unique<Sphere>(Point3{0, 0, -410}, std::move(material3), 400);

    engine.getScene().addObject(std::move(sphere));
    engine.getScene().addObject(std::move(sphere2));
    engine.getScene().addObject(std::move(ground));

    engine.getScene().addPointLight({-30, 30, 20}, Colors::WHITE, 1);
    engine.getScene().setAmbilight(Colors::WHITE, 0.2);

    engine.run();

    return 0;
}
