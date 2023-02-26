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
        triangle(point3 point_a, point3 point_b, point3 point_c, shared_ptr<material> m, vec3 normal = vec3(0,0,0)) : point_a(point_a), point_b(point_b), point_c(point_c), material_pointer(m), normal(normal) {
        };

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_data& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;



    public:
        point3 point_a;
        point3 point_b;
        point3 point_c;
        vec3 normal;
        shared_ptr<material> material_pointer;
};

// bool triangle::hit(const ray& r, double t_min, double t_max, hit_data& data) const {
//     // using barycentric
//     // look at 6.837 slides
//     Matrix3f A;
//     A(0) = point_a[0] - point_b[0];
// 	A(1) = point_a[1] - point_b[1];
// 	A(2) = point_a[2] - point_b[2];
// 	A(3) = point_a[0] - point_c[0];
// 	A(4) = point_a[1] - point_c[1];
// 	A(5) = point_a[2] - point_c[2];
// 	A(6) = r.direction()[0];
// 	A(7) = r.direction()[1];
// 	A(8) = r.direction()[2];

//     Vector3f B = Vector3f(point_a[0] - r.origin()[0], point_a[1] - r.origin()[1], point_a[2] - r.origin()[2]);

//     Vector3f x = A.inverse() * B;

//     if ((x[0] + x[1] > 1) || (x[2] <= t_min) || (x[0] < 0) || (x[1] < 0)){
//         return false;
//     }

//     auto bo = t_max == infinity;

//     // without this conditional causes image that is named trianglebug.ppm
//     if (x[2] < data.t || t_max == infinity) {
//         // std::cerr << "x[2]: " << x[2] << "\n";
//         vec3 triangle_vec_1 = point_a - point_b;
//         vec3 triangle_vec_2 = point_c - point_b;

//         data.t = x[2];
//         data.hit_point = r.at(data.t);
//         if (normal[0] == 0 && normal[1] == 0 && normal[2] ==0 ){
//             vec3 outward_normal = cross(triangle_vec_2, triangle_vec_1);
//             data.set_face_normal(r, outward_normal);
//         }
//         else{
//             data.set_face_normal(r, normal);
//         }
//         data.material_pointer = material_pointer;

//         return true;
//     }
//     else

//     return false;
// }


// bool triangle::hit(const ray& r, double t_min, double t_max, hit_data& data) const {
//     vec3 diff = r.origin() - point_a;
//     vec3 edge1 = point_b - point_a;
//     vec3 edge2 = point_c - point_a;
//     vec3 normal = cross(edge1, edge2);

//     float DdN = dot(r.direction(), normal);
//     float sign = 0.0;
//     if (DdN > 0.0)
//     {
//         sign = 1.0;
//     }

//     else if (DdN < 0)
//     {
//         sign = -1.0;
//         DdN = -DdN;
//     }
//     else
//     {
//         // Ray and triangle are parallel.
//         return false;
//     }

//     float DdQxE2 = sign * dot(r.direction(), cross(diff, edge2));
//     if (DdQxE2 >= 0.0)
//     {
//         float DdE1xQ = sign * dot(r.direction(), cross(edge1, diff));
//         if (DdE1xQ >= 0.0)
//         {
//             if (DdQxE2 + DdE1xQ <= DdN)
//             {
//                 // Line intersects triangle, check whether ray does.
//                 float QdN = -sign * dot(diff, normal);
//                 if (QdN >= 0.00001)
//                 {
//                     // if (QdN < data.t || t_max == infinity) {
//                     // std::cerr << "QdN: " << QdN << "\n";
//                     // Ray intersects triangle.
//                     data.t = QdN;
//                     data.hit_point = r.at(QdN);
//                     if (normal[0] == 0 && normal[1] == 0 && normal[2] ==0 ){
//                         vec3 outward_normal = cross(edge2, edge1);
//                         data.set_face_normal(r, outward_normal);
//                     }
//                     else{
//                         data.set_face_normal(r, normal);
//                     }
//                     // data.set_face_normal(r, normal);
//                     data.material_pointer = material_pointer;
//                     return true;
//                     // }
//                 }
//             }
//         }
//     }

//     return false;
// }


bool triangle::hit(const ray& r, double t_min, double t_max, hit_data& data) const {
    const float EPSILON = 0.0000001;

    float a,f,u,v;
    vec3 edge1 = point_b - point_a;
    vec3 edge2 = point_c - point_a;
    auto h = cross(r.direction(), edge2);
    a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    auto s = r.origin() - point_a;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    auto q = cross(s, edge1);
    v = f * dot(r.direction(),q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if (t > EPSILON) // ray intersection
    {
        data.t = t;
        data.hit_point = r.at(t);
        if (normal[0] == 0 && normal[1] == 0 && normal[2] ==0 ){
            vec3 outward_normal = cross(edge2, edge1);
            data.set_face_normal(r, outward_normal);
        }
        else{
            data.set_face_normal(r, normal);
        }
        // data.set_face_normal(r, normal);
        data.material_pointer = material_pointer;
        auto outIntersectionPoint = r.origin() + r.direction() * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;



    return false;
}


bool triangle::bounding_box(double time0, double time1, aabb& output_box) const {
    auto min_x = std::min({point_a[0], point_b[0], point_c[0]});
    auto min_y = std::min({point_a[1], point_b[1], point_c[1]});
    auto min_z = std::min({point_a[2], point_b[2], point_c[2]});
    auto max_x = std::max({point_a[0], point_b[0], point_c[0]});
    auto max_y = std::max({point_a[1], point_b[1], point_c[1]});
    auto max_z = std::max({point_a[2], point_b[2], point_c[2]});

    output_box = aabb(vec3(min_x, min_y, min_z), vec3(max_x, max_y, max_z));
    return true;
}

#endif
