#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"
#include "Eigen/Dense"

using Eigen::Matrix3f;
using Eigen::Vector3f;


class triangle : public hittable {
    public:
        triangle() {}
        triangle(point3 point_a, point3 point_b, point3 point_c, shared_ptr<material> m) : point_a(point_a), point_b(point_b), point_c(point_c), material_pointer(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_data& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        point3 point_a;
        point3 point_b;
        point3 point_c;
        shared_ptr<material> material_pointer;
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_data& data) const {

    Matrix3f A;
    A(0) = point_a[0] - point_b[0];
	A(1) = point_a[1] - point_b[1];
	A(2) = point_a[2] - point_b[2];
	A(3) = point_a[0] - point_c[0];
	A(4) = point_a[1] - point_c[1];
	A(5) = point_a[2] - point_c[2];
	A(6) = r.direction()[0];
	A(7) = r.direction()[1];
	A(8) = r.direction()[2];

    Vector3f B = Vector3f(point_a[0] - r.origin()[0], point_a[1] - r.origin()[1], point_a[2] - r.origin()[2]);

    Vector3f x = A.inverse() * B;

    if ((x[0] + x[1] > 1) || (x[2] <= t_min) || (x[0] < 0) || (x[1] < 0)){
        return false;
    }

    // if (x[2] < data.t) not sure if we need this

    if (x[2] < data.t) {
        vec3 triangle_vec_1 = point_b - point_a;
        vec3 triangle_vec_2 = point_c - point_a;

        data.t = x[2];
        data.hit_point = r.at(data.t);
        vec3 outward_normal = cross(triangle_vec_1, triangle_vec_2);
        data.set_face_normal(r, outward_normal);
        data.material_pointer = material_pointer;

        return true;
    }

    return false;

    // return false;

    // vec3 originToCenter = r.origin() - center;
    // auto a = dot(r.direction(), r.direction());
    // auto b = 2.0 * dot(originToCenter, r.direction());
    // // auto half_b = b/2.0;
    // auto c = dot(originToCenter, originToCenter) - radius*radius;
    // auto discriminant = b*b - 4*a*c;

    // if (discriminant < 0) return false;
    // auto sqrtd = sqrt(discriminant);

    // // Find the nearest root that lies in the acceptable range.
    // auto root = (-b - sqrtd ) / (2.0*a);
    // if (root < t_min || t_max < root) {
    //     root = (-b + sqrtd ) / (2.0*a);
    //     if (root < t_min || t_max < root)
    //         return false;
    // }

    // data.t = root;
    // data.hit_point = r.at(data.t);
    // vec3 outward_normal = (data.hit_point - center) / radius;
    // data.set_face_normal(r, outward_normal);
    // data.material_pointer = material_pointer;

    // return true;
}

bool triangle::bounding_box(double time0, double time1, aabb& output_box) const {
    auto  min_x = std::min({point_a[0], point_b[0], point_c[0]});
    auto  min_y = std::min({point_a[1], point_b[1], point_c[1]});
    auto  min_z = std::min({point_a[2], point_b[2], point_c[2]});
    auto  max_x = std::max({point_a[0], point_b[0], point_c[0]});
    auto  max_y = std::max({point_a[1], point_b[1], point_c[1]});
    auto  max_z = std::max({point_a[2], point_b[2], point_c[2]});
    output_box = aabb(vec3(min_x, min_y, min_z), vec3(max_x, max_y, max_z));
    return true;
}

#endif
