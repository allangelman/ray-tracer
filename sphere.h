#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), material_pointer(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_data& rec) const override;

    public:
        point3 center;
        double radius;
         shared_ptr<material> material_pointer;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_data& data) const {
    vec3 originToCenter = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(originToCenter, r.direction());
    // auto half_b = b/2.0;
    auto c = dot(originToCenter, originToCenter) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-b - sqrtd ) / (2.0*a);
    if (root < t_min || t_max < root) {
        root = (-b + sqrtd ) / (2.0*a);
        if (root < t_min || t_max < root)
            return false;
    }

    data.t = root;
    data.hit_point = r.at(data.t);
    vec3 outward_normal = (data.hit_point - center) / radius;
    data.set_face_normal(r, outward_normal);
    data.material_pointer = material_pointer;

    return true;
}

#endif
