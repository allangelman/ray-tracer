#ifndef MESH_H
#define MESH_H

#include "hittable.h"
#include "triangle.h"

#include <memory>
#include <vector>
#include <iostream>
// #include <fstream>
// #include <iostream>
// #include <sstream>

// using namespace std;

using std::shared_ptr;
using std::make_shared;

class mesh : public hittable {
    public:
        mesh() {}
        mesh(const std::string &filename, shared_ptr<material> m) {
            load(filename, m);
            for (int i = 0; i < vertices.size(); ++i) {
                std::cerr << "verts: " << vertices[i] << "\n";
                std::cerr << "normals: " << normals[i] << "\n";

            }
            for (int i = 0; i < faces_info.size(); ++i) {
                std::cerr << "faces: " << faces_info[i][0] << ' ' << faces_info[i][1] << ' ' << faces_info[i][2] << ' ' << faces_info[i][3] << ' ' << faces_info[i][4] << ' ' << faces_info[i][5]<< "\n";
            }
        }

        // void clear() { objects.clear(); }
        void load(const std::string &filename, shared_ptr<material> m) {
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
                                //  std::cerr << "debug: " << token << "\n";
                                f.push_back(stoul(token,nullptr,0));
                            }
                            i.erase(0, pos + delimiter.length());


                            count++;
                        }
                        //this pushes cfi
                        // std::cerr << "debug2: " << i << "\n";
                        f.push_back(stoul(i,nullptr,0));
                    }
                    // std::cerr << "faces: " << f[0] << f[1] << f[2] << f[3]  << f[4]  << f[5] << "\n";
                    faces_info.push_back(f);
                }
            }

            for (int i = 0; i < faces_info.size(); ++i) {
                // std::cerr << "new face: " << i << "\n";
                auto vert0 = faces_info[i][0] - 1 ;
                auto vert1 = faces_info[i][2] - 1;
                auto vert2 = faces_info[i][4] - 1 ;
                std::cerr << "VERTS: " << vert0 << vert1 << vert2<< "\n";
                auto tri = make_shared<triangle>(vertices[vert0], vertices[vert1], vertices[vert2], m);
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
    hit_data temp_data;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    // std::cerr << "HIT: " << r.orig << "\n";
    // std::cerr << "MESSHHHHTRIANGLESIZE: " <<  triangles.size()<< "\n";
    for (const auto& object : triangles) {
        if (object->hit(r, t_min, closest_so_far, temp_data)) {
            std::cerr << "MESHHIT: " << r.orig << "\n";
            hit_anything = true;
            closest_so_far = temp_data.t;
            data = temp_data;
        }
    }

    return hit_anything;

}

bool mesh::bounding_box(double time0, double time1, aabb& output_box) const {
    // if (objects.empty()) return false;

    // aabb temp_box;
    // bool first_box = true;

    // for (const auto& object : objects) {
    //     if (!object->bounding_box(time0, time1, temp_box)) return false;
    //     output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
    //     first_box = false;
    // }

    return true;
}

#endif
