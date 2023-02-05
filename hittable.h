#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "utility.h"

class material;

struct hit_data {
    point3 hit_point;
    vec3 hit_normal;
    shared_ptr<material> material_pointer;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        hit_normal = front_face ? outward_normal :-outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_data& data) const = 0;
};

#endif
