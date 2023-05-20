#ifndef MESH_H
#define MESH_H

#include "hittable.h"
#include "triangle.h"

#include <memory>
#include <vector>
#include <iostream>

using std::shared_ptr;
using std::make_shared;

class mesh : public hittable {
    public:
        mesh() {}
        mesh(shared_ptr<material> m) {
            load(m);
        }

        std::vector<shared_ptr<triangle>> get_triangles(shared_ptr<material> m) {
            load(m);
            return triangles;
        }

        void load(shared_ptr<material> m) {
            const int MAX_BUFFER_SIZE = 4096;
            char buffer[MAX_BUFFER_SIZE];
            while (std::cin.getline(buffer, MAX_BUFFER_SIZE)){
                std::stringstream ss(buffer);

                std::string s;
                ss >> s;
                if (s == "v") {
                    point3 v;
                    ss >> v[0] >> v[1] >> v[2];
                    vertices.push_back(v);

                }

                if (s == "vn") {
                    vec3 n;
                    ss >> n[0] >> n[1] >> n[2];
                    normals.push_back(n);
                }

                if (s == "f") {
                    std::vector<unsigned> f; //adg cfi... ac df gi
                    std::vector<std::string> strings;
                    std::string f1;
                    std::string f2;
                    std::string f3;
                    ss >> f1;
                    ss >> f2;
                    ss >> f3;
                    strings.push_back(f1);
                    strings.push_back(f2);
                    strings.push_back(f3);
                    for (std::string i: strings){
                        std::string delimiter = "/";
                        size_t pos = 0;
                        std::string token;
                        int count = 0;
                        while ((pos = i.find(delimiter)) != std::string::npos) {
                            token = i.substr(0, pos);

                            //this pushes adg
                            if (count%2 == 0){
                                f.push_back(stoul(token,nullptr,0));
                            }
                            i.erase(0, pos + delimiter.length());


                            count++;
                        }
                        //this pushes cfi
                        f.push_back(stoul(i,nullptr,0));
                    }
                    faces_info.push_back(f);
                }
            }

            for (int i = 0; i < faces_info.size(); ++i) {
                auto vert0Index = faces_info[i][0] - 1 ;
                auto vert1Index = faces_info[i][2] - 1;
                auto vert2Index = faces_info[i][4] - 1 ;
                auto normal0Index = faces_info[i][1] - 1;
                auto normal1Index = faces_info[i][3] - 1;
                auto normal2Index = faces_info[i][5] - 1;
                auto tri = make_shared<triangle>(vertices[vert0Index], vertices[vert1Index], vertices[vert2Index], m, normals[normal0Index], normals[normal1Index], normals[normal2Index]);
                triangles.push_back(tri);

            }
         }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_data& data) const override;

        virtual bool bounding_box(
            double time0, double time1, aabb& output_box) const override;

    public:
        std::vector<shared_ptr<triangle>> triangles;
        std::vector<point3> vertices;
        std::vector<vec3> normals;
        //each inner vector is lengeth 6 because triangle have 3 vertices
        //and is in the form of v_index, n_index, v_index, n_index, v_index, n_index
        //the outer vector is the length of the number of triangles
        std::vector<std::vector<unsigned>> faces_info;
};

bool mesh::hit(const ray& r, double t_min, double t_max, hit_data& data) const {
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : triangles) {
        if (object->hit(r, t_min, closest_so_far, data)) {
            hit_anything = true;
            closest_so_far = data.t;
        }
    }

    return hit_anything;

}

bool mesh::bounding_box(double time0, double time1, aabb& output_box) const {
    if (triangles.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : triangles) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif
