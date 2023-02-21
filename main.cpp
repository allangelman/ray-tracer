#include "utility.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include "material.h"
// #include "hittable.h"
#include "bvh.h"
#include "box.h"
#include "mesh.h"

#include <iostream>

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_data data;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, data))
        return background;

    ray scattered;
    color attenuation;
    color emitted = data.material_pointer->emitted();

    if (!data.material_pointer->scatter(r, data, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

hittable_list random_scene_bvh() {
    hittable_list world;
    hittable_list boxes;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    boxes.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    boxes.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    boxes.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    boxes.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    boxes.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    boxes.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    boxes.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world.add(make_shared<bvh_node>(boxes, 0.0, 1.0));

    return world;
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    world.add(make_shared<triangle>(point3(4, 1, 0),point3(4, 1, 0), point3(4, 1, 0), material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list triangle_scene() {
    hittable_list world;
    hittable_list triangles;
    auto ground_material = make_shared<lambertian>(color(0.9, 0.5, 0.5));
    // world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
    // world.add(make_shared<sphere>(point3(-1000,0,-1000), 1000, ground_material));



    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));

    auto material4 = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material1 = make_shared<dielectric>(1.5);
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material3));
    // world.add(make_shared<sphere>(point3(0, 0, 0), 1.0, material3));
    // world.add(make_shared<sphere>(point3(3, 1, 2), 1.0, material3));
    // world.add(make_shared<triangle>(point3(4, 1, 0),point3(4, 2, 0), point3(5, 1, 0), material2));
    // world.add(make_shared<triangle>(point3(6, 1, 0),point3(6, 2, 0), point3(7, 1, 0), material3));

    // world.add(make_shared<triangle>(point3(-3.14912, -3.14912, 0),point3(3.14912, -3.14912, 0), point3(-3.14912, 3.14912, 1.0), material3));
    // world.add(make_shared<triangle>(point3(-3.14912, 3.14912, 0),point3(3.14912, -3.14912, 0), point3(3.14912, 3.14912, 0), material3));

    // triangles.add(make_shared<triangle>(point3(-3.14912, -3.14912, 0),point3(3.14912, -3.14912, 0), point3(-3.14912, 3.14912, 1.0), material3));
    // triangles.add(make_shared<box>(point3(-3.14912, -3.14912, 0),point3(3.14912, 3.14912, 1.0), material3));
    // triangles.add(make_shared<triangle>(point3(-3.14912, 3.14912, 0),point3(3.14912, -3.14912, 1.0), point3(3.14912, 3.14912, 0), material3));
    // triangles.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material3));

    // triangles.add(make_shared<mesh>("hi", material3));
    // world.add(make_shared<bvh_node>(triangles, 0.0, 1.0));


    // world.add(make_shared<mesh>("hi", material3));

    return world;
}

hittable_list cornell_box() {
    hittable_list objects;
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));


    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    // objects.add(make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white));
    // objects.add(make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white));
    // objects.add(make_shared<sphere>(point3(320, 170, 370), 100.0, green));
    objects.add(make_shared<triangle>(point3(120, 20, 200),point3(400, 200, 100), point3(400, 22, 300), green));
    // objects.add(make_shared<translate>(make_shared<mesh>("hi", red), vec3(320,170,370)));

    // objects.add(make_shared<mesh>("hi", red));
    // objects.add(make_shared<translate>(make_shared<mesh>("hi", red), vec3(320,170,370)));

    world.add(make_shared<bvh_node>(objects, 0.0, 1.0));

    return world;
}

hittable_list simple_light() {
    hittable_list objects;
    auto ground_material = make_shared<lambertian>(color(0.9, 0.5, 0.5));
    objects.add(make_shared<sphere>(point3(-1000,0,-1000), 1000, ground_material));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, white));
    objects.add(make_shared<triangle>(point3(-3.14912, 3.14912, 0),point3(3.14912, -3.14912, 2.0), point3(3.14912, 3.14912, 1.0), green));
    objects.add(make_shared<sphere>(point3(-1,3,0), 3, white));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    objects.add(make_shared<xy_rect>(2, 3, 1, 3, -2, difflight));
    // objects.add(make_shared<xz_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));
    hittable_list objects;

    // const int boxes_per_side = 20;
    // for (int i = 0; i < boxes_per_side; i++) {
    //     for (int j = 0; j < boxes_per_side; j++) {
    //         auto w = 100.0;
    //         auto x0 = -1000.0 + i*w;
    //         auto z0 = -1000.0 + j*w;
    //         auto y0 = 0.0;
    //         auto x1 = x0 + w;
    //         auto y1 = random_double(1,101);
    //         auto z1 = z0 + w;

    //         boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
    //     }
    // }

    auto blue = make_shared<lambertian>(color(.05, .05, .95));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    // boxes1.add(make_shared<box>(point3(-200,-200,-200), point3(200,200,200), blue));
    // boxes1.add(make_shared<triangle>(point3(120, 20, 200),point3(400, 200, 100), point3(400, 22, 300), green));


    // boxes1.add(make_shared<yz_rect>(0, 555, 0, 555, 555, white));
    // boxes1.add(make_shared<yz_rect>(0, 555, 0, 555, 0, white));
    boxes1.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    // boxes1.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    // boxes1.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto glass = make_shared<dielectric>(1.5);
    boxes1.add(make_shared<translate>(make_shared<mesh>("hi", red), vec3(300, 150, 145)));

    // objects.add(make_shared<triangle>(point3(0, 100, 145),point3(0, 300, 400), point3(100, 22, 300), red));


    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    // auto center1 = point3(400, 400, 200);
    // auto center2 = center1 + vec3(30,0,0);

    // objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    // objects.add(make_shared<sphere>(
    //     point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    // ));

    // auto cube = make_shared<mesh>("hi", red);

    // objects.add(make_shared<translate>(make_shared<bvh_node>(cube, 0, 1), vec3(100,270,395)));
    // objects.add(make_shared<bvh_node>(cube, 0, 1));

    return objects;
}

hittable_list bvh_test_scene() {

    //TODO MAYBE ADD BOXES

    hittable_list objects;

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            vec3(-100,270,395)
        )
    );

    return objects;
}

int main() {

    // Image
    // const auto aspect_ratio = 16.0 / 9.0;
    // const auto aspect_ratio = 3.0 / 2.0;
    const auto aspect_ratio = 1.0;
    const int image_width = 50;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 5;
    color background(0,0,0);


    // SPHERE SCENE
    // auto world = random_scene();
    // auto world = triangle_scene();
    // point3 lookfrom(20,2,20);
    // point3 lookat(0,0,0);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.1;
    // auto vfov = 20.0;

    // BVH SCENE
    // auto world = random_scene_bvh();
    // auto world = bvh_test_scene();
    // point3 lookfrom(478, 278, -600);
    // point3 lookat(278, 278, 0);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.1;
    // auto vfov = 40.0;

    //cornell_box
    // auto world = cornell_box();
    // auto lookfrom = point3(278, 278, -800);
    // auto lookat = point3(278, 278, 0);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.1;
    // auto vfov = 40.0;

    //simple_scene
    // auto world = simple_light();
    // auto lookfrom = point3(26,3,6);
    // auto lookat = point3(0,2,0);
    // auto vfov = 20.0;
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.1;

    //final_scene
    auto world = final_scene();
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    vec3 vup(0,1,0);
    background = color(0,0,0);
    auto lookfrom = point3(478, 278, -600);
    auto lookat = point3(278, 278, 0);
    auto vfov = 40.0;


    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

    // Render

    //This line is important for ppm images I think
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::cerr << "\rScanlines remaining: " << image_height << ' ' <<j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
