#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "hittable.h" // not sure if includeing this is right

struct hit_data;

class material {
    public:
        virtual color emitted() const {
            return color(0,0,0);
        }
        virtual bool scatter(
            const ray& r_in, const hit_data& data, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_data& data, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = data.hit_normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = data.hit_normal;


            scattered = ray(data.hit_point, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color albedo;
};

class metal : public material {
    public:
       metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_data& data, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), data.hit_normal);
            scattered = ray(data.hit_point, reflected + fuzz*random_in_unit_sphere());
            // scattered = ray(data.hit_point, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), data.hit_normal) > 0);
        }

    public:
        color albedo;
        double fuzz;
};

class mirror : public material {
    public:
       mirror(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_data& data, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), data.hit_normal);
            scattered = ray(data.hit_point, reflected);
            // scattered = ray(data.hit_point, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), data.hit_normal) > 0);
        }

    public:
        color albedo;
};

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_data& data, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = data.front_face ? (1.0/ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, data.hit_normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, data.hit_normal);
            else
                direction = refract(unit_direction, data.hit_normal, refraction_ratio);

            scattered = ray(data.hit_point, direction);
            return true;
        }

    public:
        double ir; // Index of Refraction

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

class diffuse_light : public material  {
    public:
        diffuse_light(color c) : emit(c) {}

        virtual bool scatter(
            const ray& r_in, const hit_data& rec, color& attenuation, ray& scattered
        ) const override {
            return false;
        }

        //potentially remove u and v?
        virtual color emitted() const override {
            return emit;
        }

    public:
        color emit;
};

#endif
