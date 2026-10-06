#include "camera.hh"

#include <cmath>
#include <cstdint>
#include <utility>

#include "point.hh"
#include "vector.hh"

namespace ray {
    // CAMERA
    Camera::Camera(Point3 pos, Point3 vision, double alpha, double beta, double fov, uint16_t height) : pos_(pos), vision_(vision) {
        alpha_ = alpha;
        beta_ = beta;
        fov_ = fov;

        front_ = vision - pos;
        front_.normalize();

        side_ = Vector3(0, 0, 1) ^ front_;
        side_.normalize();

        up_ = side_ ^ front_;
        up_.normalize();

        vp_center_ = pos + front_ * fov;

        width_ = std::tan(alpha/2) * fov * 2;
        height_ = std::tan(beta/2) * fov * 2;

        pixal_ = height_ / height;
    }

    Camera::Camera(Point3 pos, Point3 vision, double fov, uint16_t height) : pos_(pos), vision_(vision) {
        alpha_ = atan(8 / fov) * 2;
        beta_ = atan(4.5 / fov) * 2;
        //double ratio = alpha_/beta_;

        fov_ = fov;

        front_ = vision - pos;
        std::cout << "front(" << front_.x_ << ", " << front_.y_ << ", " << front_.z_ << ")\n";
        front_.normalize();

        side_ = Vector3(0, 0, 1) ^ front_;
        std::cout << "side(" << side_.x_ << ", " << side_.y_ << ", " << side_.z_ << ")\n";
        side_.normalize();

        //up_ = side_ ^ front_;
        up_ = front_ ^ side_;
        std::cout << "up(" << up_.x_ << ", " << up_.y_ << ", " << up_.z_ << ")\n";
        up_.normalize();

        vp_center_ = pos + front_ * fov;

        width_ = std::tan(alpha_/2) * fov * 2;
        height_ = std::tan(beta_/2) * fov * 2;

        pixal_ = height_ / height;
    }

    std::vector<Point3> Camera::getPixels() {
        auto pixels = std::vector<Point3>();

        auto p = vp_center_ + (up_ * (height_ / 2)) + (side_ * -(width_ / 2));

        p = p + (up_ * -(pixal_ / 2)) + (side_ * (pixal_ / 2));
        auto pf = p;

        for (auto i = 0; i < height_ / pixal_; i++) {
            for (auto j = 0; j < width_ / pixal_; j++) {
                pixels.push_back(p);

                p = p + pixal_ * side_;
            }
            pf = pf - up_ * pixal_;
            p = pf;
        }

        std::cout << "height:" << height_ << '\n';
        std::cout << "width:" << width_ << '\n';
        std::cout << "pixal:" << pixal_ << "\n\n";

        return pixels;
    }


    // ENGINE
    Engine::Engine(Camera camera) : camera_(std::move(camera)), image_({static_cast<int>(std::round(camera_.width_ / camera_.pixal_)), static_cast<int>(std::round(camera_.height_ / camera_.pixal_))})
    {}

	Color Engine::raycast(Point3 ray_point, Vector3 ray_director, int depth) {
        double paramin = 0.;
		Color res = {0, 0, 0};

		for (const auto &object : scene_.objects_) {
			auto param = object->intersection(ray_point, ray_director);

			//std::cout << "param:" << param << std::endl;

			if (param != -1) {
				//std::cout << "salut" << std::endl;
				if (paramin == 0)
					paramin = param;
			
				if (param > paramin)
					continue;

				paramin = param;

				auto surface_p = ray_point + ray_director * param;
				auto rapport = object->get_material(surface_p);
				auto normal = object->compute_normal(surface_p);

				//auto reflection_director = (normal * (2. * (normal * source_director))) - source_director;
				auto reflection_director = ray_director - (normal * (2. * (normal * ray_director)));
				reflection_director.normalize();

				// COLOR ACCUMULATOR
				double r = 0;
				double g = 0;
				double b = 0;

				// OBJECT COLOR DECOMPOSITION
				double r_true = static_cast<double>(rapport.color.r) / 255.;
				double g_true = static_cast<double>(rapport.color.g) / 255.;
				double b_true = static_cast<double>(rapport.color.b) / 255.;

            	//std::cout << "OBJECT(" << r << ", " << g << ", " << b << ")\n";

				for (const auto &source : scene_.sources_) {
					auto source_director = source->pos_ - surface_p;
					auto source_norm = source_director.norm();
					source_director.normalize();

					if (source_director * normal > 0) {
						// DIFFUSE ILLUMINATION
						double attenuation = (100. * source->power_) / (1.0 + 0.09 * source_norm + 0.032 * source_norm * source_norm);

						r += r_true * rapport.kd * (normal * source_director) * (static_cast<double>(source->color_.r) / 255.) * attenuation;
						g += g_true * rapport.kd * (normal * source_director) * (static_cast<double>(source->color_.g) / 255.) * attenuation;
						b += b_true * rapport.kd * (normal * source_director) * (static_cast<double>(source->color_.b) / 255.) * attenuation;

						// SPECULAR ILLUMINATION
						auto specular_scal = std::max(reflection_director * source_director, 0.);

						r += rapport.ks * (static_cast<double>(source->color_.r) / 255.) * std::pow(specular_scal, rapport.ns) * (attenuation / 2);
						g += rapport.ks * (static_cast<double>(source->color_.g) / 255.) * std::pow(specular_scal, rapport.ns) * (attenuation / 2);
						b += rapport.ks * (static_cast<double>(source->color_.b) / 255.) * std::pow(specular_scal, rapport.ns) * (attenuation / 2);
					}
				}

				// REFLECTION ILLUMINATION
				if (rapport.ks > 0. && depth > 0) {
					auto reflection = raycast(surface_p + normal * 0.001, reflection_director, depth - 1);

					double rr = static_cast<double>(reflection.r) / 255.;
					double gr = static_cast<double>(reflection.g) / 255.;
					double br = static_cast<double>(reflection.b) / 255.;
					
					r += rapport.ks * rr;	
					g += rapport.ks * gr;	
					b += rapport.ks * br;
				}
						
				// // AMBIANCE ILLUMINATION
				double ra = static_cast<double>(scene_.ambilight_.r) / 255.;
				double ga = static_cast<double>(scene_.ambilight_.g) / 255.;
				double ba = static_cast<double>(scene_.ambilight_.b) / 255.;

				r += ra * r_true * rapport.kd;
				g += ga * g_true * rapport.kd;
				b += ba * b_true * rapport.kd;
						
				// CLAMPING
				r = std::min(r, 1.) * 255.;
				g = std::min(g, 1.) * 255.;
				b = std::min(b, 1.) * 255.;
						
				rapport.color.r = static_cast<uint8_t>(std::round(r));
				rapport.color.g = static_cast<uint8_t>(std::round(g));
				rapport.color.b = static_cast<uint8_t>(std::round(b));

				res = rapport.color;
			}
		}

		return res;
	}

    void Engine::run() {
        std::vector<Point3> pixels = camera_.getPixels();

        std::cout << "scene objects:" << scene_.objects_.size() << '\n';
        
        // int pix = 0;
        // int pix255 = 0;

        for (std::size_t i = 0; i < pixels.size(); i++) {
            //double paramin = 0.;

			auto ray_director = pixels[i] - camera_.getPos();
			ray_director.normalize();

			auto the_pixel = raycast(camera_.getPos(), ray_director, 5);

			image_.pixels[i] = the_pixel;
        }

        std::cout << "image lol:" << image_.height_ * image_.width_ << '\n';
        std::cout << "points plan image:" << pixels.size() << std::endl;

        image_.save("ray.ppm");
    }

}
