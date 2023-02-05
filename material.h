#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "hittable.h" // not sure if includeing this is right

struct hit_data;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_data& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_data& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.hit_normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.hit_normal;


            scattered = ray(rec.hit_point, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_data& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.hit_normal);
            scattered = ray(rec.hit_point, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.hit_normal) > 0);
        }

    public:
        color albedo;
};

#endif
