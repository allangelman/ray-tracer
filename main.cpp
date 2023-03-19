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

hittable_list samovar() {
    hittable_list objects;
    hittable_list world;
    mesh m;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto glass = make_shared<dielectric>(1.5);
    auto metalic = make_shared<metal>(color(.72, .45, .2), 0.3);
    auto metalicwall = make_shared<mirror>(color(.30, .45, .23));

    objects.add(make_shared<yz_rect>(0, 900, -700, 900, 900, green));
    objects.add(make_shared<yz_rect>(0, 900, -700, 900, 0, red));
    objects.add(make_shared<xz_rect>(200, 443, -500, 282, 899, light));

    objects.add(make_shared<xz_rect>(0, 900, -700, 900, 0, white));
    objects.add(make_shared<xz_rect>(0, 900, -700, 900, 900, white));
    objects.add(make_shared<xy_rect>(0, 900, 0, 900, 900, white));
    objects.add(make_shared<xy_rect>(0, 900, 0, 900, -700, white));

    // adding triangles from obj file -- good result and fast
    auto tri = m.loadget(metalic);
    for (int i = 0; i < tri.size(); i++) {
        objects.add(make_shared<translate>(tri[i], vec3(400,250,370)));
    }

    world.add(make_shared<bvh_node>(objects, 0.0, 1.0));

    return world;
}

hittable_list bunny() {
    hittable_list objects;
    hittable_list world;
    mesh m;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.27, .51, .71));
    auto orange = make_shared<lambertian>(color(1.0, .29, .31));
    auto purple = make_shared<lambertian>(color(0.28, .24, .55));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto glass = make_shared<dielectric>(1.5);
    auto metalic = make_shared<metal>(color(.72, .45, .2), 0.3);
    auto mirrormat = make_shared<mirror>(color(.30, .45, .23));

    objects.add(make_shared<xz_rect>(213, 343, 827, 932, 554, light));

    objects.add(make_shared<yz_rect>(0, 555, 0, 1500, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 1500, 0, orange));
    objects.add(make_shared<xz_rect>(0, 555, 0, 1500, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 1500, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 1500, purple));

    // adding triangles from obj file -- good result and fast
    auto tri = m.loadget(glass);
    for (int i = 0; i < tri.size(); i++) {
        objects.add(make_shared<translate>(tri[i], vec3(250,250,870)));
    }

    world.add(make_shared<bvh_node>(objects, 0.0, 1.0));

    return world;
}

hittable_list shapes() {
    hittable_list objects;
    hittable_list world;
    mesh m;

    auto red   = make_shared<lambertian>(color(.55, .15, .25));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.27, .51, .71));
    auto orange = make_shared<lambertian>(color(1.0, .29, .31));
    auto purple = make_shared<lambertian>(color(0.28, .24, .55));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto purplelight = make_shared<diffuse_light>(color(3, 7, 10));
    auto glass = make_shared<dielectric>(1.5);
    auto metalic = make_shared<metal>(color(.72, .45, .2), 0.3);
    auto yellow = make_shared<lambertian>(color(.72, .45, .2));
    auto mirrormat = make_shared<mirror>(color(.30, .05, .23));

    objects.add(make_shared<xz_rect>(163, 393, 777, 982, 554, light));
    // objects.add(make_shared<xy_rect>(300, 400, 200, 300, -100, purplelight));

    objects.add(make_shared<yz_rect>(0, 555, 0, 1500, 555, purple));
    objects.add(make_shared<yz_rect>(0, 555, 0, 1500, 0, mirrormat));
    objects.add(make_shared<xz_rect>(0, 555, 0, 1500, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 1500, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 1500, yellow));

    objects.add(make_shared<sphere>(vec3(100,270,600), 55, metalic));
    objects.add(make_shared<box>(vec3(40,0,540), vec3(160,215,660), red));

    objects.add(make_shared<sphere>(vec3(260,390,930), 40, glass));
    shared_ptr<hittable> box1 = make_shared<box>(vec3(200,0,870), vec3(320,350,990), metalic);
    // box1 = make_shared<translate>(box1, vec3(1,0,1));
    // box1 = make_shared<rotate_y>(box1, 50.0);
    objects.add(box1);


    objects.add(make_shared<sphere>(vec3(330,310,200), 70, glass));
    objects.add(make_shared<box>(vec3(250,0,120), vec3(410,240,280), orange));

    world.add(make_shared<bvh_node>(objects, 0.0, 1.0));

    return world;
}

hittable_list cornell_box() {
    hittable_list objects;
    hittable_list world;
    mesh m;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto glass = make_shared<dielectric>(1.5);
    auto metalic = make_shared<metal>(color(.72, .45, .2), 0.3);
    auto metalicwall = make_shared<mirror>(color(.30, .45, .23));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    // adding triangles from obj file -- good result and fast
    auto tri = m.loadget(glass);
    for (int i = 0; i < tri.size(); i++) {
        objects.add(make_shared<translate>(tri[i], vec3(400,250,370)));
    }

    world.add(make_shared<bvh_node>(objects, 0.0, 1.0));

    return world;
}


int main() {

    // Image
    // const auto aspect_ratio = 16.0 / 9.0;
    const auto aspect_ratio = 3.0 / 2.0;
    // const auto aspect_ratio = 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 3000;
    const int max_depth = 25;
    color background(0,0,0);

    //cornell_box_basic
    // auto world = cornell_box();
    // auto lookfrom = point3(278, 278, -800);
    // auto lookat = point3(278, 278, 0);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.2;
    // auto vfov = 40.0;

    //samovar
    // auto world = samovar();
    // auto lookfrom = point3(300, 140, -600);
    // auto lookat = point3(380, 280, 0);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 981.0;
    // auto aperture = 2.0;
    // auto vfov = 50.0;

    //bunny
    // auto world = bunny();
    // auto lookfrom = point3(265, 450, -200);
    // auto lookat = point3(310, 380, 200);
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 1088.63;
    // auto aperture = 0.2;
    // auto vfov = 40.0;

    // shapes
    auto world = shapes();
    auto lookfrom = point3(265, 450, -250);
    auto lookat = point3(180, 350, 200);
    vec3 vup(0,1,0);
    // auto dist_to_focus = 1181.53;
    auto dist_to_focus = 884.37;
    // auto dist_to_focus = 10.0;
    auto aperture = 30.0;
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
